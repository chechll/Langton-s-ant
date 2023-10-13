#include "server.h"

using namespace std;

int nacitanieDoSuboruHodnta;
int zapisDoSuboruHodnota;
int ukoncenieHodnota;

int server(int argc, char *argv[])
{
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    char buffer[512];

    if (argc < 2)
    {
        fprintf(stderr,"usage %s port\n", argv[0]);
        return 1;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket address");
        return 2;
    }

    listen(sockfd, 5);
    cli_len = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        return 3;
    }

    //Main Kód
        int sirka, vyska, pocetMravcov, pocetZM, typGenerovaniaPlohy, pocetChyb = 0, typCitania;
        DAT data = {.typ = 0, .pocetMravcov = pocetMravcov, .vyska = vyska, .sirka = sirka} ;
        PosielanieNaCLientStaticke("Chcete načítať zo súboru Ulozenie.txt:\n1, Plochu\n2,Plochu a Mravcov\n3,Ani jedno\n",n,newsockfd);
        nacitanieDoSuboruHodnta = CitanieZoClienta(buffer,n,newsockfd);
        printf("Nacitanie do suboru je: %i\n",nacitanieDoSuboruHodnta);
        citanieZoSuboru1(&data);
        typCitania = nacitanieDoSuboruHodnta;
        printf("Som von z metódy načítaniezo súboru a typ citania je: %i\n",typCitania);
        if(typCitania == 3 )
        {
            srand( time(0));
            PosielanieNaCLientStaticke("Zadaj vysku:\n",n,newsockfd);
            vyska = CitanieZoClienta(buffer,n,newsockfd);
            printf("V premenj výška je hodnota: %i\n",vyska);

            PosielanieNaCLientStaticke("Zadaj šírku:\n",n,newsockfd);
            sirka = CitanieZoClienta(buffer,n,newsockfd);
            printf("V premenj šírka je hodnota: %i\n",sirka);

            PosielanieNaCLientStaticke("Zadaj počet mravcov:\n",n,newsockfd);
            pocetMravcov = CitanieZoClienta(buffer,n,newsockfd);
            printf("V premenj početmravcov je hodnota: %i\n",pocetMravcov);

            PosielanieNaCLientStaticke("Výber generovania plochy\n 1) Úplne biela\n 2) Úplne čierna\n 3) Random\n 4) Ručné zadanie čiernej bunky\n",n,newsockfd);
            typGenerovaniaPlohy = CitanieZoClienta(buffer,n,newsockfd);
            printf("V premenj typ generovania plochy je hodnota: %i\n",typGenerovaniaPlohy);
            pocetZM = pocetMravcov;

        }
        if ((vyska > 5 && vyska < 16 && sirka > 5 && sirka < 16 && pocetMravcov > 0 && pocetMravcov < 4 && typGenerovaniaPlohy < 5 && typGenerovaniaPlohy > 0) || typCitania == 1 || typCitania == 2) {
            PosielanieNaCLientStaticke("1",n,newsockfd);
            sleep(1);

            if (typCitania == 1 || typCitania == 2) {
                vyska = data.vyska;
                sirka = data.sirka;
                pocetMravcov = data.pocetMravcov;
            }
            int **ploha = new int* [vyska];
            for (int i = 0; i < vyska; i++) {
                ploha[i] = new int [sirka];
            }
            int **poziciiMravcov = new int *[pocetMravcov];
            for (int i = 0; i < pocetMravcov; i++) {
                poziciiMravcov[i] = new int [5];
            }
            if (typCitania == 3) {
                for (int i = 0; i < vyska; i++) {
                    for (int j = 0; j < sirka; j++) {
                        switch (typGenerovaniaPlohy) {
                            case 1:
                            case 4:
                                ploha[i][j] = 0;
                                break;
                            case 2:
                                ploha[i][j] = 1;
                                break;
                            case 3:
                                ploha[i][j] = rand() % 2;
                                break;
                            default:
                                printf("Neocakavana chyba");
                        }
                    }
                }
                if (typGenerovaniaPlohy == 4) {
                    int pocetBuniek, x1, x2;
                    PosielanieNaCLientStaticke("Zadaj počet čierných buniek:\n",n,newsockfd);
                    pocetBuniek = CitanieZoClienta(buffer,n,newsockfd);
                    printf("V premenj počet čierných buniek je hodnota: %i\n",pocetBuniek);
                    if (pocetBuniek >= 0 && pocetBuniek <= (vyska * sirka)) {
                        for (int i = 0; i < pocetBuniek; ++i) {
                            PosielanieNaCLientStaticke("Zadaj poziciu čiernej bunky po vyske:\n",n,newsockfd);
                            x1 = CitanieZoClienta(buffer,n,newsockfd);
                            printf("V čiernej bunke výška má hodnotu: %i\n",x1);

                            PosielanieNaCLientStaticke("Zadaj poziciu čiernej bunky po šírke:\n",n,newsockfd);
                            x2 = CitanieZoClienta(buffer,n,newsockfd);
                            printf("V čiernej bunke šírká má hodnotu: %i\n",x2);
                            if (x1 < 0 || x1 >= vyska || x2 < 0 || 2 >= sirka) {
                                pocetChyb++;
                                break;
                            } else {
                                ploha[x1][x2] = 1;
                            }
                        }
                    } else {
                        pocetChyb++;
                    }
                }

            } else if (typCitania == 1 || typCitania == 2) {
                data.ploha = ploha;
                data.poziciiMravcov = poziciiMravcov;
                printf("Hodnota sirka je: %i\n",sirka);
                citanieZoSuboru2(&data,typCitania);
            }
            if (typCitania == 1 || typCitania == 3) {
                if (pocetChyb == 0) {
                    for (int i = 0; i < pocetMravcov; i++) {
                        PosielanieNaCLientStaticke("Zadaj poziciu po výške mravca\n",n,newsockfd);
                        poziciiMravcov[i][0] = CitanieZoClienta(buffer,n,newsockfd);
                        printf("Pozicia mravca výška je:%i\n",poziciiMravcov[i][0]);

                        PosielanieNaCLientStaticke("Zadaj poziciu po šírke mravca\n",n,newsockfd);
                        poziciiMravcov[i][1] = CitanieZoClienta(buffer,n,newsockfd);
                        printf("Pozicia mravca šírka je:%i\n",poziciiMravcov[i][1]);
                        poziciiMravcov[i][2] = 1;
                        poziciiMravcov[i][3] = 1;
                        poziciiMravcov[i][4] = 0;
                        if (poziciiMravcov[i][0] < 0 || poziciiMravcov[i][0] >= vyska || poziciiMravcov[i][1] < 0 ||
                            poziciiMravcov[i][1] >= sirka ||
                            prieverka (poziciiMravcov[i][0], poziciiMravcov[i][1], pocetMravcov, poziciiMravcov) != 1) {
                            pocetChyb++;
                        }
                    }
                }
            }



            if (pocetChyb == 0) {
                printf("Vyska: %d\n",vyska);
                printf("Sirka: %d\n",sirka);
                printf("Zive Mravce: %d\n",data.pocetZM);
                PosielanieNaCLientStaticke("0",n,newsockfd);
                sleep(1);
                pthread_mutex_t mutex;

                pthread_mutex_init(&mutex,nullptr);
                //printf("som tu\n");
                if(typCitania == 3) {
                    data = {.pocetMravcov=pocetMravcov, .vyska=vyska, .sirka=sirka, .ploha=ploha, .poziciiMravcov=poziciiMravcov, .pocetZM=pocetZM, .cykl = NULL, .n=n, .newsockfd =newsockfd, .mutex=&mutex};
                } else if (typCitania == 1) {
                    data = {.poziciiMravcov=poziciiMravcov, .cykl = NULL, .n=n, .newsockfd =newsockfd, .mutex=&mutex};
                } else if (typCitania == 2) {
                    data.cykl = NULL;
                    data.n = n;
                    data.newsockfd = newsockfd;
                    data.mutex = &mutex;
                }
                PosielanieNaCLientStaticke("Chcete ulozit do ulozenie.txt:\n 1) plohu     2) plochu a mravcov     3) nic\n",n,newsockfd);
                zapisDoSuboruHodnota = CitanieZoClienta(buffer,n,newsockfd);
                zapisDoSuboru(&data);



                pthread_t pthread1;
                //pthread_t pthread2;

                pthread_create(&pthread1,nullptr, langtonovMravec, &data);
                //pthread_join(pthread1, nullptr);


                pthread_mutex_lock(data.mutex);
                data.cykl = CitanieZoClienta(buffer,n,newsockfd);
                pthread_mutex_unlock(data.mutex);


                pthread_mutex_destroy(&mutex);
            } else {
                PosielanieNaCLientStaticke("1",n,newsockfd);
            }


            for (int i = 0; i < pocetMravcov; i++) {
                delete[] poziciiMravcov[i];
            }
            delete[] poziciiMravcov;

            for (int i = 0; i < vyska; i++) {
                delete[] ploha[i];
            }
            delete[] ploha;

        } else {
            PosielanieNaCLientStaticke("0",n,newsockfd);
            printf("Zle zadane data");
        }
        //Koniec MAIN

    close(newsockfd);
    close(sockfd);

    return 0;
}

int prieverka(int i, int j, int pM, int** pzM) {
    int pMnP = 0;
    int pocetKrokov = 0;
    for (int x = 0; x < pM; x++) {
        if(pzM[x][0] == i && pzM[x][1] == j && pzM[x][3] == 1){
            if (pMnP == 0) {
                pocetKrokov = pzM[x][4];
            }
            pMnP++;
            printf("pMnP = %d pocetKrokov = %d\n ", pMnP, pocetKrokov);
            if (pMnP > 1 && pzM[x][4] == pocetKrokov) {
                pMnP--;
                printf("-pMnP = %d\n", pMnP);
            }
        }
    }

    return pMnP;
}

void zapisDoSuboru(DAT *data) {
    ofstream MyFile("Ulozenie.txt");
    int typ = zapisDoSuboruHodnota;
    printf("Pri ukladaní vybral možnosť: %i\n",typ);
    if(typ == 1 || typ == 2) {
        MyFile << data->sirka << "\n" << data->vyska << "\n" << data->pocetMravcov << "\n";
        for (int i = 0; i < data->vyska; ++i) {
            for (int j = 0; j < data->sirka; ++j) {
                MyFile << data->ploha[i][j] << "\n";
            }
        }
        if (typ == 2) {
            for (int i = 0; i < data->pocetMravcov; i++) {
                MyFile <<  data->poziciiMravcov[i][0] << "\n" << data->poziciiMravcov[i][1] << "\n";
            }
        }
    }
    MyFile.close();
}

void citanieZoSuboru1(DAT *data) {
    printf("Som v metode nacitanie\n");
    string line;
    ifstream myfile ("Ulozenie.txt");
    int typ = nacitanieDoSuboruHodnta;
    //printf("Pri načítaní vybral možnosť: %i\n",typ);
    if((typ == 1 || typ == 2)) {
        if (myfile.is_open()) {

            getline(myfile, line);
            data->vyska = stoi(line);
            cout << data->vyska << '\n';

            getline(myfile, line);
            data->sirka = stoi(line);
            cout << data->sirka << '\n';

            getline(myfile, line);
            data->pocetMravcov = stoi(line);
            data->pocetZM = data->pocetMravcov;
            cout << data->pocetMravcov << '\n';

            myfile.close();
        } else cout << "Nedá sa otvoriť SÚBOR !";
    }
}

void citanieZoSuboru2(DAT *data, int typ) {
    string line;
    ifstream myfile ("Ulozenie.txt");
    if((typ == 1 || typ == 2)) {
        if (myfile.is_open()) {

            getline (myfile, line);
            int random =  stoi(line);
            getline (myfile, line);
            int random2 =  stoi(line);
            getline (myfile, line);
            int random3=  stoi(line);

            for (int i = 0; i < data->vyska; ++i) {
                for (int j = 0; j < data->sirka; ++j) {
                    getline (myfile, line);
                    data->ploha[i][j] = stoi(line);
                }
            }
            if (typ == 2) {
                for (int i = 0; i < data->pocetMravcov; i++) {
                    getline (myfile, line);
                    data->poziciiMravcov[i][0] = stoi(line);

                    getline (myfile, line);
                    data->poziciiMravcov[i][1] = stoi(line);
                    data->poziciiMravcov[i][2] = 1;
                    data->poziciiMravcov[i][3] = 1;
                    data->poziciiMravcov[i][4] = 0;
                }

            }
            myfile.close();

        } else cout << "Nedá sa otvoriť SÚBOR !";
    }
}

void vymenaPozicii (int** ploha, int** poziciiMravcov, int i, int pM, int* pocetZM) {
    if (prieverka(poziciiMravcov[i][0],poziciiMravcov[i][1],pM, poziciiMravcov) == 1) {
        if (ploha[poziciiMravcov[i][0]][poziciiMravcov[i][1]] == 1) {
            ploha[poziciiMravcov[i][0]][poziciiMravcov[i][1]] = 0;
        } else if (ploha[poziciiMravcov[i][0]][poziciiMravcov[i][1]] == 0) {
            ploha[poziciiMravcov[i][0]][poziciiMravcov[i][1]] = 1;
        }
    } else {
        poziciiMravcov[i][3] = 0;
        pocetZM--;
    }

}

void* langtonovMravec(void *dat) {
    DAT* data = (DAT*) dat;
    while (data->cykl == NULL && data->pocetZM > 0) {
        for (int i = 0; i < data->vyska; i++) {
            for (int j = 0; j < data->sirka; j++) {
                if (prieverka(i, j, data->pocetMravcov, data->poziciiMravcov) > 0) {
                    PosielanieNaCLientStaticke("x ",data->n,data->newsockfd);
                    //printf("x ");
                } else {
                    if(data->ploha[i][j] == 1)
                    {
                        PosielanieNaCLientStaticke("1 ",data->n,data->newsockfd);
                    }else if(data->ploha[i][j] == 0)
                    {
                        PosielanieNaCLientStaticke("0 ",data->n,data->newsockfd);
                    }
                    //printf("%i ", data->ploha[i][j]);
                }
            }
            PosielanieNaCLientStaticke("\n",data->n,data->newsockfd);
            //printf("\n");
        }
        PosielanieNaCLientStaticke("-------------------------\n",data->n,data->newsockfd);
        //printf("-------------------------\n");
        for (int i = 0; i < data->pocetMravcov; i++) {
            data->poziciiMravcov[i][4]++;
            if (data->poziciiMravcov[i][3] == 1) {
                if ((data->ploha[data->poziciiMravcov[i][0]][data->poziciiMravcov[i][1]] == 0 && data->poziciiMravcov[i][2] == 1) ||
                    (data->ploha[data->poziciiMravcov[i][0]][data->poziciiMravcov[i][1]] == 1 && data->poziciiMravcov[i][2] == 3)) {
                    data->poziciiMravcov[i][2] = 2;
                    if( data->poziciiMravcov[i][1] != (data->sirka - 1)) {
                        vymenaPozicii(data->ploha, data->poziciiMravcov, i, data->pocetMravcov, &data->pocetZM);
                        data->poziciiMravcov[i][1]++;
                    } else {
                        data->poziciiMravcov[i][3] = 0;
                        data->pocetZM--;
                    }
                } else if ((data->ploha[data->poziciiMravcov[i][0]][data->poziciiMravcov[i][1]] == 1 && data->poziciiMravcov[i][2] == 1) ||
                           (data->ploha[data->poziciiMravcov[i][0]][data->poziciiMravcov[i][1]] == 0 && data->poziciiMravcov[i][2] == 3)) {
                    data->poziciiMravcov[i][2] = 4;
                    if (data->poziciiMravcov[i][1] != 0) {
                        vymenaPozicii(data->ploha, data->poziciiMravcov, i, data->pocetMravcov, &data->pocetZM);
                        data->poziciiMravcov[i][1]--;
                    } else {
                        data->poziciiMravcov[i][3] = 0;
                        data->pocetZM--;
                    }
                } else if ((data->ploha[data->poziciiMravcov[i][0]][data->poziciiMravcov[i][1]] == 0 && data->poziciiMravcov[i][2] == 2) ||
                           (data->ploha[data->poziciiMravcov[i][0]][data->poziciiMravcov[i][1]] == 1 && data->poziciiMravcov[i][2] == 4)) {
                    data->poziciiMravcov[i][2] = 3;
                    if (data->poziciiMravcov[i][0] != (data->vyska - 1)) {
                        vymenaPozicii(data->ploha, data->poziciiMravcov, i, data->pocetMravcov, &data->pocetZM);
                        data->poziciiMravcov[i][0]++;
                    } else {
                        data->poziciiMravcov[i][3] = 0;
                        data->pocetZM--;
                    }
                } else {
                    if (data->poziciiMravcov[i][0] != 0) {
                        data->poziciiMravcov[i][2] = 1;
                        vymenaPozicii(data->ploha, data->poziciiMravcov, i, data->pocetMravcov, &data->pocetZM);
                        data->poziciiMravcov[i][0]--;
                    } else {
                        data->poziciiMravcov[i][3] = 0;
                        data->pocetZM--;
                    }
                }
            }
        }
        sleep(3);
    }
}

int CitanieZoClienta(char buffer[256],int n, int newsockfd)
{
    bzero(buffer,256);
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
    }
    int cislo = atoi(buffer);
    return cislo;
}

void PosielanieNaCLientStaticke(char sprava[512],int n, int newsockfd) {
    n = write(newsockfd, sprava, strlen(sprava));
    if (n < 0) {
        perror("Error writing to socket");
    }
}