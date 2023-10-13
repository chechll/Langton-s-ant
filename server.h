#ifndef SEMESTRALNA_PRACA_SERVER_H
#define SEMESTRALNA_PRACA_SERVER_H
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cstdbool>
#include <malloc.h>
#include <ctime>
#include <fstream>
#include <iostream>

typedef struct data {
    int typ;
    int pocetMravcov;
    int vyska;
    int sirka;
    int **ploha;
    int **poziciiMravcov;
    int pocetZM;
    int cykl;
    int n;
    int newsockfd;
    pthread_mutex_t* mutex;
} DAT;

int server(int argc, char *argv[]);
int prieverka (int i, int j, int pM, int **pzM);

void zapisDoSuboru(DAT *data);

void citanieZoSuboru1(DAT *data);

void citanieZoSuboru2(DAT *data, int typ);

void vymenaPozicii (int **ploha, int** poziciiMravcov, int i, int pM, int* pocetZM);

void* langtonovMravec(void* dat);

int CitanieZoClienta(char buffer[256],int n, int newsockfd);

void PosielanieNaCLientStaticke(char sprava[512],int n, int newsockfd);

#endif //SEMESTRALNA_PRACA_SERVER_H
