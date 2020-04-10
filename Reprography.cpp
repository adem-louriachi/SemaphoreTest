/**
 *
 * @file    GamingBar.cpp
 *
 * @author  Adem LOURIACHI
 *
 * @date    05/04/2020
 *
 * @version 1.0
 *
 * @brief   Mise en situation d'un fonctionnement d'un bar gaming
 *
 **/

#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>

using namespace std;

int customers = 0;
int NB_PC;
bool  noMoreCustomers = false;
sem_t mutex;
sem_t Customer;
sem_t Reprograph;
sem_t CustomerDone;
sem_t ReprographDone;

void ragequit() {
    cout << "Désolé, vous ne pouvez pas rentrer. Revenez demain." << endl;
    cout << "C'EST LA 3EME REPROGRAPHIE QUI ME DIT CA BORDEL !!" << endl;
}

void isprinting() {
    srand(time(NULL));
    int i = rand() % 3 + 1;
    cout << i << endl;
    sleep(i);
}

void *reprograph(void *) {
    while(noMoreCustomers == false) {
        if (customers == 0) cout << "Le reprographe charge l'imprimante en papier (eco) puis pars ranger la réserve." << endl;
        sem_wait(&Customer);
        if (noMoreCustomers == false) {
            sem_post(&Reprograph);
            cout << "Bonjour, passez-moi votre clé USB s'il-vous-plaît" << endl;
            sem_wait(&CustomerDone);
            cout << "Et ça imprime, et ça imprime, lalalalalaireuh..." << endl;
            isprinting();
            sem_post(&ReprographDone);
            cout << "C'est fini. Ca vous fera 12 millions d'euros par feuille. Chèque ou espèces ? :)" << endl << "Le client, choqué, tombe dans les pommes et est evacué du magasin en civière." << endl;
        }
    }
    cout << "Ahhh, voilà, une bonne journée de finie !" << endl;
}

void *customer(void *) {
    sem_wait(&mutex);
    if (customers == NB_PC) {
        sem_post(&mutex);
        ragequit();
    }
    customers += 1;
    sem_post(&mutex);
    sem_post(&Customer);
    sem_wait(&Reprograph);
    sem_post(&CustomerDone);
    sem_wait(&ReprographDone);
    sem_wait(&mutex);
    customers -= 1;
    sem_post(&mutex);
}

int main() {
    NB_PC = 4;
    string var;
    getline (cin, var);
    unsigned numCustomers = atoi(var.c_str());
    
    sem_init(&mutex,0, 1);
    sem_init(&Customer, 0, 0);
    sem_init(&Reprograph, 0, 0);
    sem_init(&CustomerDone, 0, 0);
    sem_init(&ReprographDone, 0, 0);
    
    pthread_t reproThr;
    pthread_t customersThr [numCustomers];
    
    pthread_create(&reproThr, NULL, reprograph, NULL);
        for (int i = 0; i < numCustomers; ++i) { // pour chaque patients...
            pthread_create(&customersThr[i], NULL, customer, (void*)& i); // ...on lance le thread de patients
        }
        for (int i = 0; i < numCustomers; ++i) { // pour chaque patients...
            pthread_join(customersThr[i], NULL); // ...on attend que leurs threads se finissent
        }
        // une fois que tous les patients ont été vus par le docteur, la journée du médecin est finie
        noMoreCustomers = true;
        sem_post(&Customer);
        pthread_join(reproThr, NULL);

        return 0;
    
    
    
}
