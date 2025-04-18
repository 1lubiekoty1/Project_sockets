#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include "C:\Users\Dell\Desktop\biblioteka_soc\skarpety.h"

int skpt::uruchom( WSADATA * data , std::ostream& wyjscie)
{
    if( WSAStartup( MAKEWORD( 2 , 2 ) , data ) != 0 )
    {
        wyjscie << "WSAStartup zakonczony niepowodzeniem.\n";
        return 1;
    }
    if( LOBYTE((*data).wVersion) != 2 || HIBYTE((*data).wVersion) != 2 )
    {
        wyjscie << "Nie udalo sie zaladowac wersji 2.2. \n";
        return 1;
    }

    return 0;

}
skpt::Skarpeta::Skarpeta( const char * ip_adres , int numer_portu )
{
    stworz();
    ustaw_adres( ip_adres , numer_portu );
}
skpt::Skarpeta::Skarpeta( )
{
    //nic tu nie ma ;)
}
int skpt::Skarpeta::stworz()
{
    skarpeta = socket( AF_INET , SOCK_STREAM , 0 );
    if( skarpeta == INVALID_SOCKET )
    {
        wyjscie << "Nie udalo sie stworzyc skarpety\n";
        WSACleanup();
        return 1;
    }
    return 0;
}
int skpt::Skarpeta::ustaw_adres( const char * adres_ip , int numer_portu )
{
    memset( &moj_adres , 0 , sizeof( moj_adres ));
    moj_adres.sin_family = AF_INET;
    moj_adres.sin_addr.s_addr = inet_addr( adres_ip );
    moj_adres.sin_port = htons(numer_portu);
    return 0;
}
int skpt::Skarpeta::odbierz( char * bufor , int dl_buf )
{
    // 0 - odebrano , 1 - error , 2 - zamknieto , 3 - SOCKET_ERROR
    int odb = recv( skarpeta , bufor, dl_buf, 0);
    if( odb != SOCKET_ERROR)
    {
        if(odb == 0 || odb == WSAECONNRESET )
        {
            std::cout << "Connectrion closed \n";
            return 2;
        }
        if( odb < 0 )
        {
            wyjscie << "bytes less than zero\n" << WSAGetLastError();
            return 1;
        }
        return 0;
    }
    else{ return 3; }
}
int skpt::Skarpeta::zakoncz()
{
    shutdown(skarpeta , SD_BOTH);
    closesocket( skarpeta );
    return 0;
}
int skpt::Skarpeta::wyslij( char * bufor , int dl_buf )
{
    int wysl = 0;
    wysl = send( skarpeta , bufor , dl_buf , 0);
    return wysl;
}
int skpt::Skarpeta_Serwer::przypisz()
{
    if( bind(skarpeta, (SOCKADDR *)(&moj_adres) , sizeof(moj_adres)) == SOCKET_ERROR )
    {
        wyjscie << "Nie udalo sie przypisac\n";
        closesocket( skarpeta );
        return 1;
    }
    return 0;
}
int skpt::Skarpeta_Serwer::sluchaj()
{
    if( listen( skarpeta , 1) == SOCKET_ERROR )
    {
        wyjscie << "Blad podczas sluchania\n";
    }
    return 0;
}
int skpt::Skarpeta_Serwer::akceptuj( SOCKET & acces , sockaddr_in * adres , int * len )
{
    acces = SOCKET_ERROR;
    while( acces == SOCKET_ERROR )
    {
        acces = accept( skarpeta , (struct sockaddr *)adres , len );
    }
    return 0;
}
int skpt::Skarpeta_Klient::polacz()
{
    if( connect( skarpeta , (SOCKADDR * ) & moj_adres , sizeof( moj_adres ) ) == SOCKET_ERROR )
    {
        wyjscie << "Failed to connect.\n";
        WSACleanup();
        return 1;
    }
    return 0;
}

//class Pytanie
//{
//public:
//    enum Odpowiedz { BM, BU, OM, OU };
//    std::vector<Odpowiedz> odpowiedzi;
//
//    int get_amount( Odpowiedz o)
//    {
//        int r = 0;
//        for ( auto x: odpowiedzi)
//        {
//            r += (x == o);
//        }
//        return r;
//    }
//};
//
//class Oceniacz {
//public:
//    virtual int ocen(const Pytanie &p) const = 0{
//        return 0;
//    }
//};
//
//class OceniaczPojWyb : public Oceniacz
//{
//public:
//    virtual int ocen( const Pytanie &p) const override {
//        return 1;
//    }
//};
//
//int main()
//{
//
//    OceniaczPojWyb opw;
//    Oceniacz *o = &opw;
//    Pytanie p;
//    std::cout << opw.ocen(p) << "\n";
//}
