#include <windows.h>
#include <stdbool.h>
#include <time.h>

#define screenX 1200
#define screenY 600

//globals
bool isRunning = true;
BITMAPINFO bitmapinfo;
int buffer[screenX * screenY];
HWND hwnd;
HDC hdc;

#include "./defs.h"
#include "./utils.h"
#include "./render.h"
#include "./colission.h"

Player p1, p2, ball;

void gameInit() {
	
	p1.y = (screenY - 100)/2;
	p1.x = 20;
	p1.width = 10;
	p1.height = 100;
	p1.speedY = 0.8f;
	p1.color = 0xf54242;
	
	p2.y = (screenY - 100)/2;
	p2.x = screenX - 30;
	p2.width = 10;
	p2.height = 100;
	p2.speedY = 0.8f;
	p2.color = 0x4257f5;
	
	ball.x = screenX/2;
	ball.y = screenY/2;
	ball.speedX = 0;
	ball.speedY = 0;
	ball.width = 12;
	ball.height = 12;
	ball.color = 0xffffff;
}

void gameUpdate(float delta) {
	
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		isRunning = false;
		return;
	}
	
	//init game
	if (GetAsyncKeyState(VK_SPACE))
	{
		if (!ball.speedX)
		{
		    ball.speedX = 0.5f;
		    ball.speedY = 0.5f;	
		}
	}
	
	//player1 controls
	if (GetAsyncKeyState('W'))
	{
		p1.y += p1.speedY * delta;
	}
	if (GetAsyncKeyState('S'))
	{
		p1.y -= p1.speedY * delta;
	}
	
	//player2 controls
	if (GetAsyncKeyState(VK_UP))
	{
		p2.y += p2.speedY * delta;
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		p2.y -= p2.speedY * delta;
	}
	
	//clamp
	p1.y = fclamp(15, p1.y, screenY - p1.height - 40);
	p2.y = fclamp(15, p2.y, screenY - p2.height - 40);
	
	//move ball
	ball.x += ball.speedX * delta;
	ball.y += ball.speedY * delta;
	
	//border colission
	if (ball.y >= screenY - 40)
	{
		ball.speedY *= -1;
		ball.y = screenY - 41;
	}
	if (ball.y <= ball.height + 3) 
	{
		ball.speedY *= -1;
		ball.y = ball.height + 3 + 1;
	}
	if (ball.x >= screenX)
	{
		ball.x = screenX/2, ball.y = screenY/2;
		ball.speedX = 0, ball.speedY = 0;
	}
	if (ball.x <= 0)
	{
		ball.x = screenX/2, ball.y = screenY/2;
		ball.speedX = 0, ball.speedY = 0;
	}
	
	//player colission
	if (CheckColissionRec(p1, ball))
	{
		if (ball.speedX < 0) ball.speedX *= -1;
		ball.x = p1.x + 15;
	}
	if (CheckColissionRec(p2, ball))
	{
		if (ball.speedX > 0) ball.speedX *= -1;
		ball.x = p2.x - 15;
	}
	
	//render
	ClearBackground(0x1e2d33);
	{
		DrawRec(p1.x, 	p1.y, 	p1.width, 	p1.height, 	 p1.color);
		DrawRec(p2.x, 	p2.y, 	p2.width, 	p2.height, 	 p2.color);
		DrawRec(ball.x, ball.y, ball.width, ball.height, ball.color);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg)
    {
    	case WM_CREATE: {
    		
		hdc = GetWindowDC(hwnd);

		bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapinfo.bmiHeader.biWidth = screenX;
		bitmapinfo.bmiHeader.biHeight = screenY;
		bitmapinfo.bmiHeader.biPlanes = 1;
		bitmapinfo.bmiHeader.biBitCount = 32;
		bitmapinfo.bmiHeader.biCompression = BI_RGB;
    		
    		gameInit();
			break;
		}
    	
    	case WM_CLOSE: {
    		DestroyWindow(hwnd);
    		isRunning = false;
			break;
		}
		
		case WM_DESTROY: {
			PostQuitMessage(0);
			isRunning = false;
			break;
		}

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int main(void) {
    MSG msg;
    WNDCLASSEX wc = {0};

    wc.cbSize 		= sizeof(WNDCLASSEX);
    wc.lpszClassName 	= "MyCustomPongClass";
    wc.lpfnWndProc 	= WndProc;
    wc.hIconSm       	= (HICON)LoadImage(0, ("icon.ico"), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);

    RegisterClassEx(&wc);
	
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "MyCustomPongClass",
        "Pong",
        WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, screenX, screenY,
        NULL, NULL, NULL, NULL);
    
    ShowWindow(hwnd, 1);
    
    float delta, now, last = 0;
	
    while (isRunning)
    {
	if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	now = clock();
	delta = now - last;
	last = now;

	if (GetForegroundWindow() == hwnd) 
	{
		gameUpdate(delta);

	    StretchDIBits(hdc, 0, 0, screenX, screenY,
			  0, 0, screenX, screenY, buffer,
		      	  &bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
	}

	Sleep(1);
    }
	
	return 0;
}
