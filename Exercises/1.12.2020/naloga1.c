// cc -pthread -lrt -o naloga1 naloga1.c
// Pogoj za konec niti je globalno stevilo poskusov
// Ni pogojnih spremeljivk


#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <omp.h> 

// Stevilo threadov.
#define N_THR 12
#define P 100000              // koliko poskusov naredi vsaka nit prede pososdbi globalno stanje
#define P_MAX 10000000000     // pogoj za iztop niti: globalno število poskusov
#define R 1  

// Globalne spremeljivke
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;



// Shranjevanje rezultatov
double PI_glo;              // skupni izracunani Pi
unsigned long Ns = 0;      // skupno stevilo poskusov
unsigned long Nz = 0;      // skupno stevilo zadetkov

// double pow(double x, double y);

// Thread function
void *funkcija1(void *arg)
{
    int tid;
    tid = (int)(uintptr_t)arg;          // casting because of warnings
    struct timespec myTime;
    clock_gettime(CLOCK_REALTIME,&myTime); // program prevedi s stikalom -lrt
    unsigned int seed = myTime.tv_nsec;
    int N;      // stevilo poskusov
    int Z;      // stevilo zadetkov
    double x,y;

    while (Ns < P_MAX)
    {
        Z = 0;
        for (N = 0; N < P; N++)
        {
            x = ((double) rand_r(&seed)) / RAND_MAX;
            y = ((double) rand_r(&seed)) / RAND_MAX;

            if ((pow(x,2) + pow(y,2)) <= R )
            {
                Z++;
            }
            
        }
        pthread_mutex_lock( &mut);
        Ns += N;
        Nz += Z;
        PI_glo = 4.0 * Nz / Ns;
        pthread_mutex_unlock( &mut); 
        
    }
    
    printf("KONEC, nit %d ... PI = %f (P = %lu)\n", tid, PI_glo, Ns);

    return NULL;

    // return ((void *)1);     // Pravieln pristop, ampak dela tud brez castanja

}

int main()
{
    pthread_t tid[N_THR];
    void *ret_status;  //return status

    // measure runtime
   double startTime = omp_get_wtime();

    for (int i = 0; i < N_THR; i++)
    {
        // Poiter na funckijo je vedno kar ime funckije.
        if ( pthread_create( &tid[i], NULL, funkcija1,  (void*)(unsigned long long)(i)) ) {  // casting because of warnings
            printf("Napaka: pthread_create");
            exit(1);
        }
    }
    
    // Joinamo, da bo pocakal.
    for (int i = 0; i < N_THR; i++){
        if ( pthread_join(tid[i], NULL) )
        {
            printf("Napaka: pthread_join");
            exit(1);
            
            

        }
    }


    PI_glo = 4.0 * Nz / Ns;

    // stop measuring
    double stopTime = omp_get_wtime();

    printf("++++ PI = %f, cas: %f sekund (prava vrednost = %f)\n", PI_glo, stopTime - startTime, M_PI);


    return 0;
        
}