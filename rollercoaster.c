#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t queue;
pthread_mutex_t checkin;
sem_t boarding;
sem_t riding;
sem_t unboarding;
int count = 0;
int numPassengers, carCapacity, maxRides;

void *passengerThreadFunc(void *arg)
{

    int t_id = *((int *)arg);

    printf("Passenger %d created\n", t_id);

    while (1)
    {
        // esperar na fila
        sem_wait(&queue);

        // esperar checkin
        pthread_mutex_lock(&checkin);
        // incremententar contador
        printf("Passenger %d entered the car\n", t_id);
        count++;
        // if count == maximum - signal boarding

        if (count == carCapacity)
        {
            printf("Maximum car capacity reached\n");
            sem_post(&boarding);
        }

        //  signal checkin
        pthread_mutex_unlock(&checkin);
        // esperar riding
        sem_wait(&riding);
        // signal unboarding
        printf("Passenger %d leaving the car\n", t_id);
        sem_post(&unboarding);

        printf("Passenger %d is going for a walk in the park\n", t_id);
        sleep(rand() % 5);
    }
}

void *carThreadFunc()
{

    printf("Car created\n");
    // para cada ride, sinalizar a fila n vezes (quantidade de passageiros aceitos)

    for (int i = 0; i < maxRides; i++)
    {

        printf("Releasing queue\n");
        for (int j = 0; j < carCapacity; j++)
        {
            sem_post(&queue);
        }

        // esperar boarding
        sem_wait(&boarding);

        //  zerar contador
        count = 0;
        // ride
        printf("Car is riding\n");
        sleep(2);
        printf("Car finished riding\n");

        printf("Initializing the unboarding\n");
        for (int j = 0; j < carCapacity; j++)
        {
            // para cada passageiro, sinalizar riding
            sem_post(&riding);
            // esperar unboarding
            sem_wait(&unboarding);
        }
    }

    printf("Car reached the maximum rides for the day and is closing down.\n");
}

int main(int argc, char **argv)
{

    int i = 0;

    pthread_t carThread;

    sem_init(&queue, 0, 0);
    pthread_mutex_init(&checkin, NULL);
    sem_init(&boarding, 0, 0);
    sem_init(&riding, 0, 0);
    sem_init(&unboarding, 0, 0);

    if (argc != 4)
    {
        printf("Digite %s numPassengers carCapacity maxRides\n", argv[0]);
        exit(0);
    }

    numPassengers = atoi(argv[1]);
    carCapacity = atoi(argv[2]);
    maxRides = atoi(argv[3]);

    int ids[numPassengers];
    pthread_t passengerThreads[numPassengers];

    for (i = 1; i <= numPassengers; i++)
    {
        ids[i] = i;
        pthread_create(&passengerThreads[i], NULL, passengerThreadFunc, &ids[i]);
    }

    pthread_create(&carThread, NULL, carThreadFunc, NULL);

    pthread_join(carThread, NULL);

    for (i = 1; i < numPassengers; i++)
    {
        pthread_cancel(passengerThreads[i]);
    }

    sem_destroy(&queue);
    pthread_mutex_destroy(&checkin);
    sem_destroy(&boarding);
    sem_destroy(&riding);
    sem_destroy(&unboarding);

    return 0;
}
