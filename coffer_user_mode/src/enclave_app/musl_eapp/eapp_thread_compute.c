#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *thread(void *ptr) {
    int i, j;
    long long sum = 0;

    for (i = 0; i < 10000; i++) {
        for (j = 0; j < 100000; j++) {
            sum += i * j;
        }
    }

    printf("Sum: %lld\n", sum);
}

#define CNT 10
int main(int argc, char **argv) {
  // create the thread objs
  pthread_t threads[CNT];
  long num[CNT];
  
  for (long i = 0; i < CNT; i++) {
    num[i] = i;
    pthread_create(&threads[i], NULL, *thread, (void *)&num[i]);
  }

  for (long i = 1; i < CNT; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}
