/**
 * @file proj2.c
 * @author Boris Strbak (xstrba05)
 *
 * Subor obsahujuic main funkciu.
 * Program riesi river-crossing problem
 * pomocou semaforou.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "shared.h"
#include "process.h"

/**
 * @brief Chybova sprava pri zlom pocte argumentov
 */
#define WRONG_NUM_ARGS_ERR(s) printf("You entered wrong number of paramters. Expected %d parameters.\n", s)
/**
 * @brief Chybova sprava pri zle zadanom argumente
 */
#define WRONG_ARG_ERR(s) printf("You entered wrong %d. parameter.\n", s)
/**
 * @brief Sprava ako spustit napovedu
 */
#define HELP printf("\nUse --help or -h to get help.\n");

/**
 * Funkcia vypisuje ako pouzivat program
 * @return 0
 */
int usage()
{

    printf("\nVzorove pouzitie programu: \n\n");
    printf("./proj2 P H S R W C\n\n");
    printf("[P] -> Počet osob generovaných v každé kategorii. Bude vytvořeno P hackers a P serfs.\n\tP >= 2 && (P %% 2) == 0\n");
    printf("[H] -> Maximální hodnota doby (v milisekundách), po které je generován nový proces hackers.\n\tH >= 0 && H <= 2000.\n");
    printf("[S] -> Maximální hodnota doby (v milisekundách), po které je generován nový proces serfs.\n\tS >= 0 && S <= 2000.\n");
    printf("[R] -> Maximální hodnota doby (v milisekundách) plavby.\n\tR >= 0 && R <= 2000.\n");
    printf("[W] -> Maximální hodnota doby (v milisekundách), po které se osoba vrací zpět na molo (pokud bylo před tím molo plné).\n\tW >= 20 && W <= 2000.\n");
    printf("[C] -> Kapacita mola.\n\tC >= 5\n");
    printf("Všechna čísla jsou celá čísla.\n\n");

    return 0;

}

/**
 * funkcia na vypis chuby pri praci s pamatou
 * @return 1
 */
int memory_err()
{

    printf("Chyba pri praci s pamatou.\n");
    HELP;
    sh_clear();
    return 1;

}

/**
 * funkcia na vypis chuby pri praci s procesmi
 * @return 1
 */
int process_err()
{

    printf("Chyba pri praci s procesmi.\n");
    HELP;
    sh_clear();
    return 1;

}

/**
 * @brief Funkci na ukladanie a kontrolu argumentov
 * Uklada argumenty do premennych pomocou ukazatelov
 * a kontroluje ich spravnost
 *
 * @param n pocet argumentov
 * @param items argumenty
 * @param p pocet generovanych osob
 * @param h maximalni doba, po ktere je generován novy proces hackers
 * @param s maximalni doba, po ktere je generován novy proces serfs
 * @param r maximalni doba plavby
 * @param w maximalni doba, po ktere se osoba raci zpet na molo
 * @param c kapacita mola
 * @return 1 ak nastala chyba a 0 ak nenastala
 */
int check_args(int n, char* items[], unsigned long int *p, unsigned short *h,
                unsigned short *s, unsigned short *r, unsigned short *w, unsigned long int *c)
{

    char *endptr; //string for storing not numeric part of args
    int err = 0; //kontrola chyby

    if(n != 7)
	{

        WRONG_NUM_ARGS_ERR(6);
        return 1;

	}

    //p by mal byt integer vacsi ako 1 a delitelny 2
    *p = strtoul(items[1], &endptr, 10);
    if(*p < 2 || *p % 2 != 0 || *endptr != 0)
    {

        WRONG_ARG_ERR(1);
        err = 1;

    }

    //h by mal byt integer vacsi alebo rovny ako 0
    //a mansi alebo rovny ako 2000
    *h = strtoul(items[2], &endptr, 10);
    if(*h > 2000 || *endptr != 0)
    {

        WRONG_ARG_ERR(2);
        err = 1;

    }

    //s by mal byt integer vacsi alebo rovny ako 0
    //a mansi alebo rovny ako 2000
    *s = strtoul(items[3], &endptr, 10);
    if(*s > 2000 || *endptr != 0)
    {

        WRONG_ARG_ERR(3);
        err = 1;

    }

    //r by mal byt integer vacsi alebo rovny ako 0
    //a mansi alebo rovny ako 2000
    *r = strtoul(items[4], &endptr, 10);
    if(*r > 2000 || *endptr != 0)
    {

        WRONG_ARG_ERR(4);
        err = 1;

    }

    //w by mal byt integer vacsi alebo rovny ako 20
    //a mansi alebo rovny ako 2000
    *w = strtoul(items[5], &endptr, 10);
    if(*w < 20 || *w > 2000 || *endptr != 0)
    {

        WRONG_ARG_ERR(5);
        err = 1;

    }

    //c by mal byt integer vacsi alebo rovny ako 5
    *c = strtoul(items[6], &endptr, 10);
    if(*c < 5 || *endptr != 0)
    {

        WRONG_ARG_ERR(6);
        err = 1;

    }

    if(err)
    {

        HELP;
        return 1;

    }

    return 0;

}// check_args


/**
 * Hlavna funkcia programu, ktora kontroluje
 * spravnost zadnych parametrov, Inicializuje
 * zdielanu pamat a semafory a procesy, ktore
 * inicializuju procesy jednotlivych osob
 *
 * @param argc pocet argumentov predanych programu
 * @param argv pole jednotlivych argumentov
 * @return 1 ak nastala chyba a 0 ak nenastala
 */
int main(int argc, char* argv[])
{

    unsigned long int persons_num,  //pocet generovaných osob
                    molo_cap;       //kapacita mola
    unsigned short hacker_time,   // cas generovania hackers
                serf_time,      //     -- || --    serfs
                sail_time,      // cas plavby
                comeback_time;  //cas navratu na molo

    if(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) return usage();

	if(check_args(argc, argv, &persons_num, &hacker_time,
                    &serf_time, &sail_time,
                    &comeback_time, &molo_cap) == 1) return 1;

    srand((unsigned) time(NULL));    //randomize

    if(init_sem() == 1)
    {

        printf("Could not open semafore\n");
        sh_clear();
        return 1;

    }

    if(init_shmem() == 1) return memory_err();

    shmem->persons_num = persons_num;
    shmem->molo_cap = molo_cap;
    shmem->hacker_time = hacker_time;
    shmem->serf_time = serf_time;
    shmem->sail_time = sail_time;
    shmem->comeback_time = comeback_time;
    shmem->action_counter = 0;
    shmem->present = 0;
    shmem->pr_serf = 0;
    shmem->pr_hacker = 0;
    shmem->can_sail = 1;
    shmem->serf_on = 0;
    shmem->hacker_on = 0;

    pid_t hacker = fork();

    if(hacker == 0)
    {

        init_processes("HACK");
        exit(0);

    } else if(hacker > 0)
    {

        // main
        pid_t serf = fork();

        if(serf == 0)
        {

            init_processes("SERF");
            exit(0);

        } else if(serf > 0)
        {

            // main
            WAIT_CHILD;
            sh_clear();
            exit(0);

        } else return process_err();
    } else return process_err();

    sh_clear();
    return EXIT_SUCCESS;

} //main
