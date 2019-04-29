/**
 * @file process.c
 * @author Boris Strbak (xstrba05)
 *
 * Obsahuje implementaciu funkcii z kniznice
 * process.h
 */

#include "process.h"
#include "shared.h"

void get_on_molo(int index, const char* who)
{

    while(1)
    {

        if(shmem->present >= shmem->molo_cap)
        {

            //opusta queue
            sem_wait(m_sem->mutex);
            MSG(++shmem->action_counter, who, index, "leaves queue", shmem->pr_hacker, shmem->pr_serf);
            sem_post(m_sem->mutex);

            //sleep
            usleep((rand() % shmem->comeback_time) * 1000);

            //skusa sa dostat spat do queue
            sem_wait(m_sem->mutex);
            MSG(++shmem->action_counter, who, index, "is back", shmem->pr_hacker, shmem->pr_serf);
            sem_post(m_sem->mutex);

        } else break;

    }

}

int check_board(const char* who)
{

    if(!shmem->can_sail
        || !(shmem->present >= 4)
        || (shmem->serf_on + shmem->hacker_on == 4)) return 0;
    if(strcmp(who, "HACK") == 0)
    {

        //hacker
        if(shmem->serf_on == 3
            ||(shmem->serf_on == 1 && shmem->hacker_on == 2)) return 0;

    } else {

        //serf
        if(shmem->hacker_on == 3
            ||(shmem->hacker_on == 1 && shmem->serf_on == 2)) return 0;

    }

    return 1;

}

int get_on_board(const char* who)
{

    strcmp(who, "HACK") ? shmem->serf_on++ : shmem->hacker_on++;

    if(shmem->serf_on + shmem->hacker_on == 4)
        shmem->can_sail = 0;

    return shmem->serf_on + shmem->hacker_on;

}

void cpt_leaves(int index, const char* who)
{

    usleep((rand() % shmem->sail_time)*1000); //sleep

    sem_post(m_sem->leave);

    // int semval;  //Debugging
    // sem_getvalue(m_sem->leave, &semval);
    // printf("cpt sem opens leave %d\n", semval);

    sem_wait(m_sem->cptl);
    sem_wait(m_sem->mutex);

    MSG(++shmem->action_counter, who, index, "captain exits", shmem->pr_hacker, shmem->pr_serf);
    shmem->hacker_on = 0;
    shmem->serf_on = 0;
    shmem->can_sail = 1;

    sem_post(m_sem->mutex);
    sem_init(m_sem->boat, 1, 4);

}

void ncpt_leaves(int index, const char* who)
{

    sem_wait(m_sem->leave);
    sem_wait(m_sem->mutex);

    MSG(++shmem->action_counter, who, index, "member exits", shmem->pr_hacker, shmem->pr_serf);
    strcmp(who, "HACK") ? shmem->serf_on-- : shmem->hacker_on--;

    if(shmem->hacker_on + shmem->serf_on == 1)
        sem_post(m_sem->cptl);
    else sem_post(m_sem->leave);
    sem_post(m_sem->mutex);

}

void init_sailor(int index, const char* who)
{

    int can_get_on = 0;
    int order = 0;

    sem_wait(m_sem->mutex);
    //critical section starts
    fprintf(shmem->destf, "%ld:\t %s %d:\t starts\n", ++shmem->action_counter, who, index);
    sem_post(m_sem->mutex);

    //skusa sa dostat na molo
    get_on_molo(index, who);

    //je na mole
    sem_wait(m_sem->mutex);
    MSG(++shmem->action_counter, who, index, "waits",
        strcmp(who, "HACK") ? shmem->pr_hacker : ++shmem->pr_hacker,
        strcmp(who, "SERF") ? shmem->pr_serf : ++shmem->pr_serf);
    shmem->present++;
    sem_post(m_sem->mutex);

    //vytvara skupinu na plavbu
    while(!can_get_on)
    {

        sem_wait(m_sem->mutex);
        // printf("%s %d checking boat\n", who, index);
        can_get_on = check_board(who);
        sem_post(m_sem->mutex);

    }

    //get on boat
    sem_wait(m_sem->boat);
    sem_wait(m_sem->mutex);

    order = get_on_board(who);

    sem_post(m_sem->mutex);

    if(order == 1)
    {

        // printf("captain waiting for sail\n");
        sem_wait(m_sem->sail);
        sem_wait(m_sem->mutex);

        shmem->present -= 4;
        shmem->pr_serf -= shmem->serf_on;
        shmem->pr_hacker -= shmem->hacker_on;
        MSG(++shmem->action_counter, who, index, "boards",
        strcmp(who, "HACK") ? shmem->pr_hacker : shmem->pr_hacker,
        strcmp(who, "SERF") ? shmem->pr_serf : shmem->pr_serf);

        sem_post(m_sem->mutex);

    } else if(order == 4) {

        // printf("signaling captain to sail\n");
        sem_post(m_sem->sail);

    }

    if(order == 1) cpt_leaves(index, who);
    else if(order > 1) ncpt_leaves(index, who);

    exit(0);

}

void init_processes(const char* who)
{


    srand((unsigned) time(NULL));    //randomize
    for(unsigned int i = 1; i <= shmem->persons_num; i++)
    {

        pid_t sailor = fork();
        if(sailor == 0)
        {

            //get max SERF or HACKER time depending on who
            int max_delay = strcmp(who, "HACK") ? shmem->serf_time : shmem->hacker_time;
            int delay = rand() % max_delay; //nahodne cislo v danom rozmedzi
            if(max_delay > 0) usleep(delay*1000); //sleep
            init_sailor(i, who);

        }
        else if(sailor < 0)
        {

            //error
            exit(1);

        }

    }

    WAIT_CHILD;
    exit(0);

}
