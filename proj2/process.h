/**
 * @file process.h
 * @author Boris Strbak (xstrba05)
 *
 * Kniznica obsahujuca funkcie, vytvarajuce
 * a pracujuce s procesmi.
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

#ifndef PROCESS_H
#define PROCESS_H

/**
* Skusa dostat osobu na molo a simuluje cakanie na
* volne miesto
*
* @param index cislo osoby
* @param who identifikace osoby (HACKER alebo SERF)
*
* @return void
*/
void get_on_molo(int index, const char* who);

/**
* Caka, kym nebude mozne sa dostat na lod
*
* @param who identifikace osoby (HACKER alebo SERF)
*
* @return 1 ak je mozne ist na lod, 0 ak nie je
*/
int check_board(const char* who);

/**
 * Vstupuje na lod
 *
 * @param who identifikace osoby (HACKER alebo SERF)
 *
 * @return Cislo, identifikujuce, kolka sa dana osoba dostala na lod
 */
 int get_on_board(const char* who);

/**
 * Kapitan opusta lod, caka na signal posledneho
 * nekapitana.
 *
 * @param index cislo osoby
 * @param who identifikace osoby (HACKER alebo SERF)
 *
 * @return void
 */
void cpt_leaves(int index, const char* who);

/**
 * Clen posadky, ktory neni kapitan opusta lod
 *
 * @param index cislo osoby
 * @param who identifikace osoby (HACKER alebo SERF)
 *
 * @return void
 */
void ncpt_leaves(int index, const char* who);

/**
 * Inicializuje jednotlive osoby
 *
 * @param index cislo osoby
 * @param who identifikace osoby (HACKER alebo SERF)
 *
 * @return void
 */
void init_sailor(int index, const char* who);

/**
 * Funkcia generujuca procesi hackers a serfs
 *
 * @param who Osoba, pre ktoru robi procesy(HACKER/SERF)
 *
 * @return void
 */
void init_processes(const char* who);

#endif
