#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <iostream>
#include "skarpety.h"
//has to be linked with libws3_32.a

using namespace std;

int main()
{
    WSADATA wsaData;
    skpt::uruchom( &wsaData , cout);
    skpt::Skarpeta_Serwer server( "127.0.0.1" , 30000 );
    server.przypisz();
    server.sluchaj();

    skpt::Skarpeta client;
    int klient_length = sizeof( client.moj_adres );

    cout << "Waiting for a client to connect...\n";

    server.akceptuj( client.skarpeta , &(client.moj_adres) , &klient_length );

    cout << "Client connected.\n";

    server.zakoncz();

    int wyslanych;
    //int odebranych = SOCKET_ERROR;
    char wysbufor[ 32 ] = "Serwer says hello";
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
                //wyslanych = send( acceptSocket , wysbufor , strlen( wysbufor ), 0);
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
