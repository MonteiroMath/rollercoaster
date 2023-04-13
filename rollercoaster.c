#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t queue;
sem_t checkin;
sem_t boarding;
sem_t riding;
sem_t unboarding;
int count = 0;

void passengerThreadFunc(void *arg)
{

    int t_id = *((int *)arg);

    printf("Passenger %d created", t_id);
    // esperar na fila
    // esperar checkin
    // incremententar contador
    // if count == maximum - signal boarding
    //  signal checkin
    // esperar riding
    // signal unboarding
    return;
}

void carThreadFunc(void *arg)
{

    printf("Car created");
    // para cada ride, sinalizar a fila n vezes (quantidade de passageiros aceitos)
    // esperar boarding
    //  zerar contador
    // ride
    // para cada passageiro, sinalizar riding
    // esperar unboarding

    return;
}

int main(int argc, char **argv)
{

    int numPassengers, carCapacity, maxRides, i;
    int ids[numPassengers];
    pthread_t passengerThreads[numPassengers];
    pthread_t carThread;

    sem_init(&queue, 0, 0);
    sem_init(&checkin, 0, 1);
    sem_init(&boarding, 0, 0);
    sem_init(&riding, 0, 0);
    sem_init(&unboarding, 0, 0);

    if (argc != 3)
    {
        printf("Digite %s Num_arrays Num_elementos\n", argv[0]);
        exit(0);
    }

    numPassengers = atoi(argv[1]);
    carCapacity = atoi(argv[2]);
    maxRides = atoi(argv[3]);

    for (i = 0; i < numPassengers; i++)
    {
        ids[i] = i - 1;
        pthread_create(&passengerThreads[i], NULL, passengerThreadFunc, &ids[i]);
    }

    pthread_create(&carThread, NULL, carThreadFunc, NULL);

    for (i = 0; i < numPassengers; i++)
    {
        pthread_join(passengerThreads[i], NULL);
    }
    pthread_join(carThread, NULL);

    sem_destroy(&queue);
    sem_destroy(&checkin);
    sem_destroy(&boarding);
    sem_destroy(&riding);
    sem_destroy(&unboarding);

    return 0;
}
