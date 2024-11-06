#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#define N 100000000
void *task(long *num) {
  long i, j, k;
  long long sum = 0;

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      for (k = 0; k < N; k++) {
        sum += (i + j) * k;
      }
    }
  }
    printf("Thread-%lu: ends task - Sum: %lld\n", *num, sum);
    return NULL;
}

#define CNT 8
int main(int argc, char **argv) {
  // create the thread objs
  pthread_t threads[CNT];
  long num[CNT];

  for (long i = 0; i < CNT; i++) {
    num[i] = i;
    pthread_create(&threads[i], NULL, *task, (void *)&num[i]);
  }

  for (long i = 0; i < CNT; i++) {
    printf("pthread_join with threads[%u] = %u\n", i, threads[i]);
    pthread_join(threads[i], NULL);
  }

  return 0;
}
