#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *thread(void *ptr) {
  printf("Thread - 0x%lx\n\r", *(long *)ptr);
  fflush(stdout);
  return ptr;
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