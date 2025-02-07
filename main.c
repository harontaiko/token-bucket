#include <stdio.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    int capacity;
    int tokens;
    int rate;
    time_t last_refill_time;
} TokenBucket;


void token_bucket_init(TokenBucket *bucket, int capacity, int rate) {
    bucket->capacity = capacity;
    bucket->rate = rate;
    bucket->tokens = capacity;
    bucket->last_refill_time = time(NULL);
}

void refill_tokens(TokenBucket *bucket) {
    time_t now = time(NULL);
    double elapsed_seconds = difftime(now, bucket->last_refill_time);

    int new_tokens = elapsed_seconds * bucket->rate;

    if (new_tokens > 0) {
        bucket->tokens = (bucket->tokens + new_tokens) < bucket->capacity ?
                         (bucket->tokens + new_tokens) : bucket->capacity;
        bucket->last_refill_time = now; 
    }
}

int consume_tokens(TokenBucket *bucket, int tokens_requested) {
    refill_tokens(bucket); 

    if (bucket->tokens >= tokens_requested) {
        bucket->tokens -= tokens_requested;
        return 1; 
    } else {
        return 0; 
    }
}


int main(int argc, char **argv)
{
    TokenBucket bucket;
    token_bucket_init(&bucket, 10, 2); 

    for (int i = 0; i < 5; i++) {
        if (consume_tokens(&bucket, 3)) {
            printf("Request %d: Allowed\n", i+1);
        } else {
            printf("Request %d: Denied\n", i+1);
        }
        sleep(1);
    }

    return 0;
}
