#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <iostream>
//has to be linked with libws3_32.a

using namespace std;

int uruchom( WSADATA * data )
{
    if( WSAStartup( MAKEWORD( 2 , 2 ) , data ) != 0 )
    {
        cout << "WSAStartup failed.\n";
        return 1;
    }
    if( LOBYTE((*data).wVersion) != 2 || HIBYTE((*data).wVersion) != 2 )
    {
        cout << "Failed to load version 2.2\n";
        return 1;
    }
    return 0;
}

int stworz( SOCKET& sock )
{
    sock = socket( AF_INET , SOCK_STREAM , 0 );
    if( sock == INVALID_SOCKET )
    {
        cout << "Failed to create socket\n";
        WSACleanup();
        return 1;
    }
    return 0;
}

int ustaw_adres( sockaddr_in * adres , int numer_portu )
{
    memset( adres , 0 , sizeof( (*adres) ));
    (*adres).sin_family = AF_INET;
    (*adres).sin_addr.s_addr = inet_addr( "127.0.0.1" );
    (*adres).sin_port = htons(numer_portu);
    return 0;
}

int przypisz( SOCKET & sock , sockaddr_in * adres )
{
    if( bind(sock, (SOCKADDR *)adres , sizeof((*adres))) == SOCKET_ERROR )
    {
        cout << "Failed to bind()\n";
        closesocket( sock );
        return 1;
    }
    return 0;
}

int sluchaj( SOCKET & sock )
{
    if( listen( sock, 1) == SOCKET_ERROR )
    {
        cout << "Error listening on socket.\n";
    }
    return 0;
}

int akceptuj( SOCKET & acces , SOCKET & sock , sockaddr_in * adres , int * len )
{
    acces = SOCKET_ERROR;
    while( acces == SOCKET_ERROR )
    {
        acces = accept( sock , (struct sockaddr *)adres , len );
    }
    return 0;
}

int odbierz( SOCKET & sock , char * bufor , int dl_buf )
{
    int odb = recv( sock , bufor, dl_buf, 0);
    if( odb != SOCKET_ERROR)
    {
        if(odb == 0 || odb == WSAECONNRESET )
        {
            cout << "Connectrion closed \n";
            return 2;
        }
        if( odb < 0 )
        {
            cout << "bytes less than zero\n" << WSAGetLastError();
            return 1;
        }
        return 0;
    }
    else{ return 3; }
}

int main()
{
    WSADATA wsaData;
    uruchom( &wsaData );
    SOCKET mainSocket;
    stworz(mainSocket);
    sockaddr_in adres_serwera , adres_klienta;
    ustaw_adres( &adres_serwera , 30000 );
    przypisz( mainSocket , &adres_serwera );
    sluchaj( mainSocket );
    SOCKET acceptSocket;
    int klient_length = sizeof(adres_klienta);
    cout << "Waiting for a client to connect...\n";
    akceptuj( acceptSocket , mainSocket , &adres_klienta , &klient_length );
    cout << "Client connected.\n";


    int wyslanych;
    //int odebranych = SOCKET_ERROR;
    char wysbufor[ 32 ] = "Serwer says hello";
    char odbbufor[32] = "";

//    wyslanych = send( acceptSocket , wysbufor , strlen( wysbufor ), 0);
//    printf( "Sent bytes: %d\n" , wyslanych );

//    int odb = 3;
//
//    while( odb == 3 )
//    {
//        odb = odbierz( acceptSocket , odbbufor , 32 );
//        switch( odb )
//        {
//        case 0:
//            cout << "Recv: " << odbbufor << endl;
//            break;
//        case 1:
//            cout << "negative bytes" << endl;
//            return 1;
//            break;
//        case 2:
//            cout << "closed" << endl;
//            break;
//        }
//    }
    bool truth = true;
    while(truth)
    {
        int a;
        cout << "type:\n1 to sent.\n2 to recieve.\n3 to shut down.";
        cin >> a;
        switch( a )
        {
        case 1:
                wyslanych = send( acceptSocket , wysbufor , strlen( wysbufor ), 0);
                printf( "Sent bytes: %d\n" , wyslanych );
                break;
        case 2:
            {
            int odb = 3;
            while( odb == 3 )
            {
                odb = odbierz( acceptSocket , odbbufor , 32 );
                switch( odb )
                {
                case 0:
                    cout << "Recv: " << odbbufor << endl;
                    break;
                case 1:
                    cout << "negative bytes" << endl;
                    return 1;
                    break;
                case 2:
                    cout << "closed" << endl;
                    break;
                }
            }
            break;
            }
        case 3:
            shutdown( acceptSocket , SD_SEND );
            closesocket( mainSocket );
            closesocket( acceptSocket );
            WSACleanup();
            truth = false;
            break;
        }
    }

    cout << "Press any key to exit:";
    cin.get();

    return 0;
}
