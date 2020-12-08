// prevajanje:
// gcc -pthread -lrt vaja06-n1-pi.c -o vaja06-n1-pi
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>

#define ST_NITI 3
#define ST_POSKUSOV 100000
#define MAX_POSKUSOV 1000000000

pthread_mutex_t moj_mutex = PTHREAD_MUTEX_INITIALIZER;

long Ns = 0, Zs = 0;

void *racunaj_pi(void *arg) {
    struct timespec myTime;
    clock_gettime(CLOCK_REALTIME,&myTime);
    unsigned int seed = myTime.tv_nsec;

    while (1) {
        int tmp_z = 0;

        pthread_mutex_lock(&moj_mutex);

        if (Ns >= MAX_POSKUSOV) {
            pthread_mutex_unlock(&moj_mutex);

            break;
        }

        pthread_mutex_unlock(&moj_mutex);

        for (int i = 0; i < ST_POSKUSOV; i++) {             
            double x = ((double) rand_r(&seed)) / RAND_MAX; 
            double y = ((double) rand_r(&seed)) / RAND_MAX; 

            if (x * x + y * y <= 1) {
                tmp_z++;
            }
        }

        pthread_mutex_lock(&moj_mutex);

        Ns += ST_POSKUSOV;
        Zs += tmp_z;

        pthread_mutex_unlock(&moj_mutex);
    }

    pthread_exit(NULL);
}

int main (int argc, char* argv[]) {
    pthread_t id_niti[ST_NITI];
    int ret;
    void *status;
    int i;
    printf("%ld\n", sizeof(long));
    pthread_mutex_init(&moj_mutex, NULL);

    for (i = 0; i < ST_NITI; i++) {
        ret = pthread_create(&id_niti[i], NULL, racunaj_pi, (void *)(intptr_t)i);
    
        if (ret) {
            printf("Napaka (%d)\n", ret);
            
            return 1;
        }
    }

    for(i = 0; i < ST_NITI; i++) {
        pthread_join(id_niti[i], &status);
    }

    double priblizek_pi = 4*(double)Zs/(double)Ns;

    printf("Priblizek pi: %f\n", priblizek_pi);

    pthread_mutex_destroy(&moj_mutex);
    pthread_exit(NULL);
}