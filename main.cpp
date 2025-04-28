#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <iostream>
#include <thread>
#include <mutex>
//#include <conio.h>
#include "skarpety.h"
#define LICZBA_GIER 3
//has to be linked with libws3_32.a

using namespace std;

mutex mojMutex;

void graj( skpt::Skarpeta klient1 , skpt::Skarpeta klient2 , int * miejsce , bool * zatrzymaj)
{
    char bufor1[31] = "jestes w grze!\n";
    char bufor2[31] = "koniec gry.\n";
    cout << klient1.wyslij( bufor1 , 31 ) << endl;
    cout << klient2.wyslij( bufor1 , 31 ) << endl;
    mojMutex.lock();
    (*miejsce) = 1;
    mojMutex.unlock();
    //game loop
    klient1.wyslij( bufor2 , 31 );
    klient2.wyslij( bufor2 , 31 );
    klient1.zakoncz();
    klient2.zakoncz();
    mojMutex.lock();
    (*miejsce) = 2;
    mojMutex.unlock();
    return;
}

inline int miejsce_na_liscie( int * lista , int war )
{
    for( int i = 0 ; i < LICZBA_GIER ; ++i )
    {
        if( lista[i] == war )
            return i;
    }
    return -1;
}

int main()
{
    bool Truth = true;
    bool przymusowy_koniec = false;
    int lista_watkow[LICZBA_GIER];
    for( int i = 0 ; i < LICZBA_GIER ; ++i )
    {
        lista_watkow[i] = 0;
    }
    thread lista_watkow_p[LICZBA_GIER];

    WSADATA wsaData;
    skpt::uruchom( &wsaData , cout);
    skpt::Skarpeta_Serwer server( "127.0.0.1" , 30000 );
    server.przypisz();
    server.sluchaj();

    skpt::Skarpeta kolejka[2];
    kolejka[0].skarpeta = SOCKET_ERROR;
    kolejka[1].skarpeta = SOCKET_ERROR;


    while(Truth)
    {
        if( kolejka[0].skarpeta == SOCKET_ERROR )
        {
            cout << "oczekiwanie na pierwszego klienta\n";
            server.akceptuj( kolejka[0].skarpeta , &(kolejka[0].moj_adres) , NULL );
        }
        else if( kolejka[1].skarpeta == SOCKET_ERROR )
        {
            cout << "oczekiwanie na drugiego klienta\n";
            char bufor[31] = "czekam na drugiego gracza...\n";
            kolejka[0].wyslij( bufor , 31 );

            server.akceptuj( kolejka[1].skarpeta , &(kolejka[1].moj_adres) , NULL );
        }
        else
        {
            cout << "oczekiwanie na miejsce\n";
            char bufor[31] = "czakam na wolne miejsce...\n";
            kolejka[0].wyslij( bufor , 31 );
            kolejka[1].wyslij( bufor , 31 );

            int wolne_miejsce = miejsce_na_liscie( lista_watkow , 0 );
            if( wolne_miejsce >= 0)
            {
                cout << "zajmowanie miejsca nr." << wolne_miejsce << endl;
                lista_watkow_p[wolne_miejsce] = thread( graj , kolejka[0], kolejka[1] , &(lista_watkow[wolne_miejsce]) , &przymusowy_koniec );
            }
            kolejka[0].skarpeta = SOCKET_ERROR;
            kolejka[1].skarpeta = SOCKET_ERROR;
        }


        for(int i = 0 ; i < LICZBA_GIER ; ++i )
        {
            if( lista_watkow[i] == 2)
            {
                cout << "wylanczanie gry na miejscu nr." << i << endl;
                mojMutex.lock();
                lista_watkow[i] = 0;
                mojMutex.unlock();
                lista_watkow_p[i].join();

            }
        }
//        if(kbhit())
//        {
//            char a = getch();
//            if( a == '1' )
//            {
//                przymusowy_koniec = true;
//                Truth = false;
//            }
//        }
    }

    for(int i = 0 ; i < LICZBA_GIER ; ++i )
    {
        if( lista_watkow_p[i].joinable())
        {
            lista_watkow_p[i].join();
        }
    }

    WSACleanup();
    cout << "Press any key to exit:";
    cin.get();

    return 0;
}
