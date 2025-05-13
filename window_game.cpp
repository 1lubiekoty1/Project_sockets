#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <ctime>
#include <cstdlib>

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

using namespace std;

LRESULT CALLBACK WindowProc(HWND hWnd , UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd , uMsg, wParam, lParam);
}

class Window
{
public:
    Window( int width_ , int height_ ) : m_hInstance(GetModuleHandle(nullptr))
    {
        const wchar_t* CLASS_NAME = L"Example My WIndow Class";

        WNDCLASS wndClass = {};
        wndClass.lpszClassName = CLASS_NAME;
        wndClass.hInstance = m_hInstance;
        wndClass.hIcon = LoadIcon( NULL , IDI_WINLOGO);
        wndClass.hCursor = LoadCursor( NULL , IDC_ARROW);
        wndClass.lpfnWndProc = WindowProc;

        RegisterClass(&wndClass);

        DWORD style = WS_CAPTION | WS_MAXIMIZEBOX | WS_SYSMENU ;

        int width = width_;
        int height = height_;

        RECT rect;
        rect.left = 250;
        rect.top = 250;
        rect.right = rect.left + width;
        rect.bottom = rect.top + height;

        AdjustWindowRect(&rect , style, false);

        m_hWnd = CreateWindowEx( 0 , CLASS_NAME , L"PONG" ,style, rect.left , rect.top , width , height , NULL , NULL , m_hInstance , NULL );

        ShowWindow(m_hWnd , SW_SHOW);

    }

    Window(const Window&) = delete;
    Window operator =(const Window&) = delete;

    ~Window()
    {
        const wchar_t* CLASS_NAME = L"Example My WIndow Class";

        UnregisterClass(CLASS_NAME, m_hInstance);
    }

    bool ProcessMessages()
    {
        MSG msg = {};

        while( PeekMessage(&msg, nullptr, 0u,0u,PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
            {
                return false;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return true;
    }
    HDC GetWndDC()
    {
        return GetDC(m_hWnd);
    }
    HWND GetHWnd()
    {
        return m_hWnd;
    }
    HINSTANCE GetWndINST()
    {
        return m_hInstance;
    }

private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
};

string choose( int a );

struct game
{
    int gap;
    int width;
    int height;
    int plate_width;
    int plate_thickness;
    int ballR;
    short int scoreA;
    short int scoreB;
    double ballVX;
    int ballVY;
    double plate_a;
    double plate_b;
    double A_s;
    double B_s;
    double ballX;
    double ballY;
    game( int w , int h , int pw , int pt , int br , int g);
    void setPlateAPos( double i );
    void setPlateBPos( double i );
    void tryMoveBall();
    void adjustScore();
    bool willTouch( double x , bool isRight );
    bool touched = false;
};

int setPlatePos( int i , int height , int plate_width  );
void draw_bitmap( string loc , int x , int y , HDC wndDC , HINSTANCE hinst )
{
    wstring wstr = wstring(loc.begin(), loc.end());
    const wchar_t* wchrt = wstr.c_str();
    HBITMAP bitmap = (HBITMAP)LoadImage( hinst , wchrt, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    HDC OurDC = CreateCompatibleDC(0);
    SelectObject(OurDC , bitmap);
    BITMAP bitmapa;
    GetObject(bitmap, sizeof(bitmapa), &bitmapa);
    BitBlt(wndDC, x, y, bitmapa.bmWidth , bitmapa.bmHeight , OurDC, 0, 0, SRCCOPY);
    DeleteDC(OurDC);
    DeleteObject(bitmap);
}

int main()
{
    srand(time(NULL));
    int i = 0  , licznik = 0 , width = 760 , height = 540 , plate_witdh = 40 , ballR = 10, gap = 30 , plate_thickness = 15;

    game my_game(width , height , plate_witdh , plate_thickness , ballR, gap );

    string location = "C:\\Users\\Dell\\Desktop\\BACA\\window_game\\" , scor1 , scor2 , image1 , image2;
    double ball_x = 0 , ball_y = 0 , plate_a = 0 , plate_b = 0;
    cout << "Creating Window\n";
    Window* pWindow = new Window( width + 6 , height + 23 );
    bool running = true;


    HDC my_window = pWindow->GetWndDC();
    HBRUSH black_brush = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH white_brush = CreateSolidBrush(RGB(255, 255, 255));
    HRGN screen = CreateRectRgn(0,0,width,height);

    HBITMAP bitmaps[11];
    for( int j = 0 ; j <= 10 ; ++j )
    {
        string loc = location + choose(j);
        wstring wstr = wstring(loc.begin(), loc.end());
        const wchar_t* wchrt = wstr.c_str();
        bitmaps[j] = (HBITMAP)LoadImage( pWindow->GetWndINST() , wchrt, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }
    HDC OurDC = CreateCompatibleDC(0);

    //HBITMAP example_bitmap = LoadBitmapA( pWindow->GetWndINST() , "C:\\Users\\Dell\\Desktop\\BACA\\window_game\\bmp_24.bmp" );
    //HBITMAP example_bitmap = (HBITMAP)LoadImage( pWindow->GetWndINST() , L"C:\\Users\\Dell\\Desktop\\BACA\\window_game\\one.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //BITMAP bitmap;
    //GetObject(example_bitmap, sizeof(bitmap), &bitmap);
    //cout << bitmap.bmHeight << " " << bitmap.bmWidth << " " << bitmap.bmPlanes << " " << bitmap.bmBitsPixel << " " << bitmap.bmType << endl;

    while(running)
    {
        if(!pWindow->ProcessMessages())
        {
            cout << "Closing Window\n";
            running = false;
        }

        POINT p;
        RECT q;
        GetWindowRect( pWindow->GetHWnd(), &q );
        GetCursorPos(&p);
        i = p.y - q.top - 20;
        licznik += 2;
        licznik %= 360;

        my_game.tryMoveBall();
        ball_x = my_game.ballX;
        ball_y = my_game.ballY;
        my_game.setPlateAPos( i );
        my_game.setPlateBPos( (height/2)*sin(licznik*3.14/180) + (height/2));
        plate_a = my_game.plate_a;
        plate_b = my_game.plate_b;
        my_game.adjustScore();

        scor1 = choose(my_game.scoreA);
        scor2 = choose(my_game.scoreB);
        image1 = location + scor1;
        image2 = location + scor2;


        //HRGN ball = CreateEllipticRgn(ball_x - ballR, ball_y - ballR, ball_x + ballR, ball_y + ballR);
        HRGN ball = CreateRectRgn(ball_x - ballR, ball_y - ballR, ball_x + ballR, ball_y + ballR);
        HRGN plateA = CreateRectRgn(0, plate_a - plate_witdh , plate_thickness , plate_a + plate_witdh );
        HRGN plateB = CreateRectRgn(width, plate_b - plate_witdh , width - plate_thickness , plate_b + plate_witdh );

        FillRgn(my_window, screen, black_brush);
        FillRgn(my_window, plateA, white_brush);
        FillRgn(my_window, plateB, white_brush);
        FillRgn(my_window, ball, white_brush);

        SelectObject(OurDC , bitmaps[ my_game.scoreA ] );
        BitBlt(my_window, width/4, 20, 50 , 50 , OurDC, 0, 0, SRCCOPY);
        SelectObject(OurDC , bitmaps[ my_game.scoreB ] );
        BitBlt(my_window, 3 * width/4 - 50 , 20, 50 , 50 , OurDC, 0, 0, SRCCOPY);

        //draw_bitmap( image1 , width/4, 20, my_window , pWindow->GetWndINST());
        //draw_bitmap( image2 , 3 * width/4 - 50 , 20, my_window , pWindow->GetWndINST());

        DeleteObject(ball);
        DeleteObject(plateA);
        DeleteObject(plateB);

        Sleep(10);
    }

    for( int j = 0 ; j <= 10 ; ++j )
    {
        DeleteObject(bitmaps[j]);
    }
    DeleteDC(OurDC);

    DeleteObject(screen);
    DeleteObject(black_brush);
    DeleteObject(white_brush);
    //DeleteObject(example_bitmap);

    delete pWindow;
    return 0;
}

string choose( int a )
{
    string numer;
    switch(a)
        {
        case 0:
            numer = "zero.bmp";
            break;
        case 1:
            numer = "one.bmp";
            break;
        case 2:
            numer = "two.bmp";
            break;
        case 3:
            numer = "three.bmp";
            break;
        case 4:
            numer = "four.bmp";
            break;
        case 5:
            numer = "five.bmp";
            break;
        case 6:
            numer = "six.bmp";
            break;
        case 7:
            numer = "seven.bmp";
            break;
        case 8:
            numer = "eight.bmp";
            break;
        case 9:
            numer = "nine.bmp";
            break;
        case 10:
            numer = "ten.bmp";
            break;
        default:
            numer = "zero.bmp";
            break;
        }
        return numer;
}


game::game( int w , int h , int pw , int pt , int br , int g)
{
    gap = g;
    width = w;
    height = h;
    plate_width = pw;
    plate_thickness = pt;
    ballR = br;
    ballVX = (rand()%2)+2;
    ballVY = (rand()%2)+2;
    plate_a = h/2;
    plate_b = h/2;
    B_s = 0;
    A_s = 0;
    ballX = w/2;
    ballY = h/2;
    scoreA = 0;
    scoreB = 0;
}

void game::setPlateAPos( double i )
{
    double prev = plate_a;

    if( i - plate_width - gap <= 0 )
    {
        plate_a = plate_width + gap;
        A_s = max(-5.0,min(prev - plate_a , 5.0));
        return;
    }
    else if( i + plate_width + gap>= height )
    {
        plate_a = height - plate_width - gap;
        A_s = max(-5.0,min(prev - plate_a , 5.0));
        return;
    }
    plate_a = i;
    A_s = max(-5.0,min(prev - plate_a , 5.0));
}

void game::setPlateBPos( double i )
{
    double prev = plate_b;
    if( i - plate_width - gap <= 0 )
    {
        plate_b = plate_width + gap;
        B_s = max(-5.0,min(prev - plate_b,5.0));
        return;
    }
    else if( i + plate_width + gap>= height )
    {
        plate_b = height - plate_width - gap;
        B_s = max(-5.0,min(prev - plate_b,5.0));
        return;
    }
    plate_b = i;
    B_s = max(-5.0,min(prev - plate_b,5.0));
}

void game::tryMoveBall()
{
    //cout<<"PlateA: " << A_s << " PlateB: "<< B_s << endl;
    double balx = ballX + ballVX;
    double baly = ballY + ballVY;
    if( balx + ballR <= width - plate_thickness && balx - ballR >= 0 + plate_thickness && baly + ballR <= height && baly - ballR >= 0)
    {
        touched = false;
        ballX = balx;
        ballY = baly;
        return;
    }
    if( balx  <= 0 )
    {
        touched = false;
        ++scoreB;
        ballVX = - ballVX;
        ballX = width / 2;
        ballY = height / 2;
        return;
    }
    else if( balx >= width )
    {
        touched = false;
        ++scoreA;
        ballVX = - ballVX;
        ballX = width / 2;
        ballY = height / 2;
        return;
    }
    else if( balx + ballR >= width - plate_thickness)
    {
        if( willTouch(baly, true) && !touched )
        {
            ballVX = - ballVX;
            ballVY = ballVY - B_s * 0.5;
            touched = true;
        }
    }
    else if( balx - ballR <= 0  + plate_thickness)
    {
        if( willTouch(baly , false) && !touched )
        {
            ballVX = - ballVX;
            ballVY = ballVY - A_s * 0.5;
            touched = true;
        }
    }
    else if( baly + ballR >= height || baly - ballR <= 0 )
    {
        ballVY = - ballVY;
    }

    balx = ballX + ballVX;
    baly = ballY + ballVY;
    ballX = balx;
    ballY = baly;
}

void game::adjustScore()
{
    if(scoreA >= 11)
    {
        scoreA = 0;
    }
    if(scoreB >= 11)
    {
        scoreB = 0;
    }
}

bool game::willTouch( double y , bool isRight )
{
    double pos;
    if(isRight)
    {
        pos = plate_b;
    }
    else
    {
        pos = plate_a;
    }
    if( y + ballR < pos - plate_width )
    {
        return false;
    }
    else if( y + ballR >= pos - plate_width && y - ballR <= pos + plate_width )
    {
        return true;
    }
    else if( y - ballR > pos + plate_width )
    {
        return false;
    }
}
