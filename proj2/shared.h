/**
 * @file shared.h
 * @author Boris Strbak (xstrba05)
 *
 * Subor obsahujuci zdielane premenne a semafory,
 * inicializacne funkcie a cistiacu funkciu.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#ifndef SHARED_H
#define SHARED_H

/**
 * @brief cakanie na ukoncenie vsetkych potomkov
 */
#define WAIT_CHILD while(wait(NULL) != -1);

/**
 * @brief vystupny subor
 */
#define DESTFILE "proj2.out"

/**
 * @brief vystupna sprava
 */
#define MSG(A, NAME, I, msg, NH, NS) fprintf(shmem->destf, "%ld:\t %s %d:\t %s: \t %ld: %ld\n", A, NAME, I, msg, NH, NS)

/**
 * @brief Struktura obsahujuca semafory
 */
typedef struct semaphore {

    sem_t* mutex;   //pristup k zdielanym premennym
    sem_t* boat;    //lodicka
    sem_t* leave;   //kapitan hovori opustit lod
    sem_t* cptl;     //captain leaves
    sem_t* sail;     //signal pre kapitana na odplavanie

} SEMAPHORE;

/**
 * Globalne semfafory
 */
SEMAPHORE* m_sem;

/**
 * @brief Struktura obsahujuca zdielane premenne
 */
typedef struct shmem{

    unsigned long int persons_num,  //pocet generovaných osob
                    molo_cap,       //kapacita mola
                    action_counter,
                    present,
                    pr_serf,
                    pr_hacker;
    unsigned short hacker_time,   // cas generovania hackers
                serf_time,      //     -- || --    serfs
                sail_time,      // cas plavby
                comeback_time;  //cas navratu na molo
    unsigned int can_sail, // signal ci mozu nastupovat osoby
                serf_on, //pocet serfs na lodi
                hacker_on; //pocet hackerss na lodi

    FILE* destf; // vystupny subor

} SHMEM;

/**
 * Globalne zdielane premenne
 */
SHMEM* shmem;

/**
 * Inicializuje zdielnu pamat
 *
 * @return 1 ked sa podari a 0 ked sa nepodari
 */
int init_shmem();

/**
 * Inicializuje semafor
 *
 * @return 1 ked sa podari a 0 ked sa nepodari
 */
int init_sem();

/**
 * Cisti zdielanu pamat a zatvara cielovy subor a semafory
 */
void sh_clear();

#endif
