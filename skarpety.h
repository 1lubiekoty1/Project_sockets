#ifndef _SKARPETY_LIB_JM_
#define _SKARPETY_LIB_JM_

#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>
#include <winsock2.h>
namespace skpt
{
int uruchom(WSADATA * data, std::ostream & wyjscie);

class Skarpeta
{
public:
    Skarpeta();
    Skarpeta( const char * ip_adres , int numer_portu );
    SOCKET skarpeta;
    std::ostream& wyjscie = std::cout;
    sockaddr_in moj_adres;
    int stworz();
    int ustaw_adres( const char * adres_ip , int numer_portu );
    int odbierz( char * bufor , int dl_buf );
    int zakoncz();
    int wyslij( char * bufor , int dl_buf );
};

class Skarpeta_Serwer : public Skarpeta
{
public:
    using Skarpeta::Skarpeta;
    int przypisz();
    int sluchaj();
    int akceptuj( SOCKET & acces , sockaddr_in * adres , int * len );
};

class Skarpeta_Klient : public Skarpeta
{
public:
    using Skarpeta::Skarpeta;

    int polacz( );
};
}
#endif // _SKARPETY_LIB_JM_
