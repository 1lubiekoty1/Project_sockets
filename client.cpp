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

//int wyslij()
//{
//
//}

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

int ustaw_adres( sockaddr_in * adres , int numer_portu )
{
    memset( adres , 0 , sizeof( (*adres) ));
    (*adres).sin_family = AF_INET;
    (*adres).sin_addr.s_addr = inet_addr( "127.0.0.1" );
    (*adres).sin_port = htons(numer_portu);
    return 0;
}

int polacz( SOCKET & sock , sockaddr_in * adres )
{
    if( connect( sock , (SOCKADDR * ) adres , sizeof( (*adres) ) ) == SOCKET_ERROR )
    {
        cout << "Failed to connect.\n";
        WSACleanup();
        return 1;
    }
    return 0;
}

int znajdz_serwer( hostent ** serwer , const char * nazwa )
{
    (*serwer) = gethostbyname( "localhost" );
    //cout << (*serwer);
    if(*serwer == nullptr )
    {
        cout << "Failed to find server" << endl;
        return 1;
    }
    return 0;
}

int main()
{
    WSADATA wsaData;
    uruchom( &wsaData );
    SOCKET mainSocket;
    stworz( mainSocket );
    sockaddr_in adres_serwera;
//    hostent *server = nullptr;
//    if(znajdz_serwer( &server , "localhost") == 0)
//    {
//        //cout << server ;//<< "    " << WSAGetLastError() << endl;
//    }
//
//    memmove( (char *)&adres_serwera.sin_addr.s_addr , (char *)server->h_addr , server->h_length );
//    cout << server->h_name << endl;
//    cout << *(server->h_addr_list) << endl;
    ustaw_adres( &adres_serwera , 30000 );
    if(polacz( mainSocket , &adres_serwera ) != 0)
    {
        return 1;
    }
    cout << "Connected to server\n";

    int wyslanych;
    char wysbufor[ 32 ] = "Client says hello";
    char odbbufor[32] = "";
    bool truth = true;
    while(truth)
    {
        int a;
        cout << "type:\n1 to sent.\n2 to recieve.\n3 to shut down.";
        cin >> a;
        switch( a )
        {
        case 1:
                wyslanych = send( mainSocket , wysbufor , strlen( wysbufor ), 0);
                printf( "Sent bytes: %d\n" , wyslanych );
                break;
        case 2:
            {
            int odb = 3;
            while( odb == 3 )
            {
                odb = odbierz( mainSocket , odbbufor , 32 );
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
            shutdown( mainSocket , SD_SEND );
            closesocket( mainSocket );
            WSACleanup();
            truth = false;
            break;
        }
    }





    cout << "Press any key to exit:";
    cin.get();

    return 0;
}
