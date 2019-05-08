/**
 * @file shared.c
 * @author Boris Strbak (xstrba05)
 *
 * Obsahuje implementaciu funkcii kniznice shared.h
 */

#include "shared.h"
#include <errno.h>

int init_shmem(){

    shmem = NULL;
    shmem = (SHMEM*) mmap(NULL, sizeof(SHMEM), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    if(shmem == MAP_FAILED)
        return 1;
    if((shmem->destf = fopen(DESTFILE, "w")) == NULL)
        return 1;

    return 0;

}

int init_sem(){

    errno = 0;
    m_sem = NULL;

    m_sem = (SEMAPHORE*) mmap(NULL, sizeof(SEMAPHORE), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    if(m_sem == MAP_FAILED)
        return 1;

    m_sem->mutex = sem_open("mutex", O_CREAT | O_EXCL, DEFFILEMODE, 1);
    if(errno == 17) sem_unlink("mutex");
    m_sem->boat = sem_open("boat", O_CREAT | O_EXCL, DEFFILEMODE, 4);
    if(errno == 17) sem_unlink("boat");
    m_sem->leave = sem_open("leave", O_CREAT | O_EXCL, DEFFILEMODE, 0);
    if(errno == 17) sem_unlink("leave");
    m_sem->cptl = sem_open("cptl", O_CREAT | O_EXCL, DEFFILEMODE, 0);
    if(errno == 17) sem_unlink("cptl");
    m_sem->sail = sem_open("sail", O_CREAT | O_EXCL, DEFFILEMODE, 0);
    if(errno == 17) sem_unlink("sail");

    if(m_sem->mutex == SEM_FAILED
        || m_sem->boat == SEM_FAILED
        || m_sem->leave == SEM_FAILED
        || m_sem->cptl == SEM_FAILED)
        return 1;

    return 0;

}

void sh_clear()
{

    fclose(shmem->destf);
    if(shmem != NULL)
        munmap(shmem, sizeof(SHMEM));
    sem_unlink("mutex");
    sem_unlink("boat");
    sem_unlink("leave");
    sem_unlink("cptl");
    sem_unlink("sail");
    if(m_sem != NULL)
        munmap(m_sem, sizeof(SEMAPHORE));

}
