#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <iostream>
#include "C:\Users\Dell\Desktop\biblioteka_soc\skarpety.h"
//has to be linked with libws3_32.a


using namespace std;

int main()
{
    WSADATA wsaData;
    skpt::uruchom( &wsaData , std::cout );
    skpt::Skarpeta_Klient client( "127.0.0.1" , 30000 );
//    hostent *server = nullptr;
//    if(znajdz_serwer( &server , "localhost") == 0)
//    {
//        //cout << server ;//<< "    " << WSAGetLastError() << endl;
//    }
//
//    memmove( (char *)&adres_serwera.sin_addr.s_addr , (char *)server->h_addr , server->h_length );
//    cout << server->h_name << endl;
//    cout << *(server->h_addr_list) << endl;
    if( client.polacz() != 0)
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
                //wyslanych = send( mainSocket , wysbufor , strlen( wysbufor ), 0);
                wyslanych = client.wyslij( wysbufor , 31 );
                printf( "Sent bytes: %d\n" , wyslanych );
                break;
        case 2:
            {
            int odb = 3;
            while( odb == 3 )
            {
                memset( odbbufor , 0 , 32 * sizeof( char ));
                odb = client.odbierz( odbbufor , 31 );
                switch( odb )
                {
                case 0:
                    odbbufor[31] = '\0';
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
            client.zakoncz();
            truth = false;
            break;
        }
    }
    WSACleanup();
    cout << "Press any key to exit:";
    cin.get();

    return 0;
}
