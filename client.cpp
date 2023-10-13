#include "client.h"

int CitanieZoServera(char buffer[512],int n, int sockfd)
{
    bzero(buffer,512);
    n = read(sockfd, buffer, 511);
    if (n < 0)
    {
        perror("Error reading from socket");
    }
    printf("%s",buffer);
    int cislo = atoi(buffer);
    return cislo;
}

void* zobrazovanieMravca(void * dat)
{
    DATC* data = (DATC*) dat;
    while (data->ckoniec ==nullptr) {
        //sleep(2);
        CitanieZoServera(data->cbuffer,data->cn,data->csockfd);
    }
}

int hodnota;
int kontrolaIf;
int citaniePodmienka;

int PosielanieNaServerDynamicke(char buffer[256],int n, int sockfd)
{
    bzero(buffer,256);
    fgets(buffer, 255, stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("Error writing to socket");
    }
    int cislo = atoi(buffer);
    hodnota = cislo;
    return cislo;
}

int client(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    char buffer[512];

    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        return 1;
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(atoi(argv[2]));


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 3;
    }

    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error connecting to socket");
        return 4;
    }
    int kontrolaPredGenerovanim;


    //NacitanieZoSuboru
    CitanieZoServera(buffer,n,sockfd);
    citaniePodmienka = PosielanieNaServerDynamicke(buffer,n,sockfd);

    if(citaniePodmienka  == 1)
    {
        //Počet mravcov
        kontrolaPredGenerovanim = CitanieZoServera(buffer,n,sockfd);
        printf("Pocet mravcov = Kontrola pred\n");
        int pocetMravcovFor = CitanieZoServera(buffer,n,sockfd);
        //printf("Pocet mravcov = %d\n",pocetMravcovFor);
        //Generovanie pozicie mravcov
        for (int i = 0; i < 1; ++i) {
            //Výška mravca
            CitanieZoServera(buffer,n,sockfd);
            PosielanieNaServerDynamicke(buffer,n,sockfd);
            //Šírka mravca
            CitanieZoServera(buffer,n,sockfd);
            PosielanieNaServerDynamicke(buffer,n,sockfd);
        }
        kontrolaPredGenerovanim = CitanieZoServera(buffer,n,sockfd);
        //CHYBA Stisknutie - Ukončenie
    }
    else if(citaniePodmienka  ==2)
    {
        kontrolaPredGenerovanim = CitanieZoServera(buffer,n,sockfd);

    }
    else if(citaniePodmienka  == 3)
    {
        //Výška
        int vyska;
        CitanieZoServera(buffer,n,sockfd);
        vyska = PosielanieNaServerDynamicke(buffer,n,sockfd);
        //Šírka
        int sirka;
        CitanieZoServera(buffer,n,sockfd);
         sirka = PosielanieNaServerDynamicke(buffer,n,sockfd);
        //Počet mravcov
        int pocetM;
        CitanieZoServera(buffer,n,sockfd);
        pocetM = PosielanieNaServerDynamicke(buffer,n,sockfd);
        //int pocetMravcovFor = hodnota;
        //Typ generovania plochy
        int typG;
        //printf("------\n");
        CitanieZoServera(buffer,n,sockfd);
        typG = PosielanieNaServerDynamicke(buffer,n,sockfd);
        //printf("----\n");
        //kontrolaPredGenerovanim = 1;
        kontrolaPredGenerovanim = CitanieZoServera(buffer,n,sockfd);
        //printf("-------\n");
        //printf("Komtrola pred generovanim = %d\n", kontrolaPredGenerovanim);
        //printf("kontrola = %d\n",kontrolaPredGenerovanim);
        if(kontrolaPredGenerovanim == 1)
        {
            //printf("typG je : %d\n",typG);
            if(typG == 4) //Pokiaľ sa voli random
            {
                //PosielanieNaServerDynamicke(buffer,n,sockfd);
                int pocetCiernychFor = CitanieZoServera(buffer,n,sockfd);
                //printf("Hodnota je je: %i\n",pocetCiernychFor);
                for (int i = 0; i < pocetCiernychFor; ++i) {
                    //Výška súradnice čiernej
                    CitanieZoServera(buffer,n,sockfd);
                    PosielanieNaServerDynamicke(buffer,n,sockfd);
                    //Šírka súradnice čiernej
                    CitanieZoServera(buffer,n,sockfd);
                    PosielanieNaServerDynamicke(buffer,n,sockfd);
                }
            }
            for (int i = 0; i < pocetM; ++i) {
                //printf("cakanie na vysku\n");
                //Výška mravca
                CitanieZoServera(buffer,n,sockfd);
                PosielanieNaServerDynamicke(buffer,n,sockfd);
                //Šírka mravca
                //printf("cakanie na sirku\n");
                CitanieZoServera(buffer,n,sockfd);
                PosielanieNaServerDynamicke(buffer,n,sockfd);
            }
        }
    }
    int kontrolaIf = CitanieZoServera(buffer,n,sockfd);
    if(kontrolaIf == 0 && kontrolaPredGenerovanim == 1)
    {

        //UloženieDoSuboru
        CitanieZoServera(buffer,n,sockfd);
        PosielanieNaServerDynamicke(buffer,n,sockfd);
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex,nullptr);
        pthread_t pthread1;
        DATC data = {.ckoniec = nullptr,.cbuffer=buffer,.cn = n, .csockfd = sockfd};
        pthread_create(&pthread1,nullptr, zobrazovanieMravca, &data);
        //pthread_join(pthread1, nullptr);

        char hodnotaScanner;
        //scanf("%c",&hodnotaScanner);


        pthread_mutex_lock(&mutex);
        scanf("%s",&data.ckoniec);
        //data.ckoniec = nullptr;
        //printf("Prijali %c\n",hodnotaScanner);
        pthread_mutex_unlock(&mutex);
        PosielanieNaServerDynamicke(buffer,n,sockfd);

        pthread_mutex_destroy(&mutex);

    } else {
        printf("\nZle zadane data");
        return 0;
    }

    close(sockfd);
    return 0;
}
