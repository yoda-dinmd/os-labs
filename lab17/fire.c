/* Build: gcc -Wall -Wextra -pthread -o fire fire.c
 * Run:   ./fire      (two threads)
 *        ./fire 4    (four threads)
 */
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#define N 1000000

static long counter;

static void *work(void *arg)
{
    int id = *(int *)arg;
    printf("thread %d sees counter = %ld\n", id, counter);
    for (int i = 0; i < N; i++)
        counter++;  /* Intentional race: read, add, then write. */
    return NULL;
}

int main(int argc, char *argv[])
{
    int workers = 2;
    if (argc == 2 && strcmp(argv[1], "4") == 0) {
        workers = 4;
    } else if (argc != 1 && (argc != 2 || strcmp(argv[1], "2") != 0)) {
        fprintf(stderr, "Usage: %s [2|4]\n", argv[0]);
        return 1;
    }

    pthread_t threads[4];
    int ids[] = {1, 2, 3, 4};
    for (int i = 0; i < workers; i++) {
        int error = pthread_create(&threads[i], NULL, work, &ids[i]);
        if (error != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(error));
            return 1;
        }
    }

    for (int i = 0; i < workers; i++) {
        int error = pthread_join(threads[i], NULL);
        if (error != 0) {
            fprintf(stderr, "pthread_join: %s\n", strerror(error));
            return 1;
        }
    }

    long expected = (long)workers * N;
    printf("expected: %ld\n", expected);
    printf("counter: %ld\n", counter);
    printf("lost updates: %ld\n", expected - counter);
    return 0;
}
