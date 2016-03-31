#include <windows.h>   
#include <stdlib.h>
#include <time.h>
#include "defines.h"
#include "EnumKare.h"
#include "EnumMap.h"

using namespace std;

char*			szApplicationName = "������";
char*			szWindowClassName = "PFclass";

EnumMap* EMap;
EnumKare* Kare;

LRESULT CALLBACK WindowProc(HWND hwnd,
	UINT msg,
	WPARAM wparam,
	LPARAM lparam)
{
	//���ڵ��豸����
	HDC				hdc;
	PAINTSTRUCT     ps;
	//���ƴ��������С
	static int cxClient, cyClient;
	//��������
	static HDC		hdcBackBuffer;
	static HBITMAP	hBitmap;
	static HBITMAP	hOldBitmap;
	switch (msg)
	{
	case WM_CREATE:
	{
		RECT rect;
		srand((unsigned)time(NULL));
		GetClientRect(hwnd, &rect);

		EMap = new EnumMap();
		// ����GA��
		Kare = new EnumKare(CROSSOVER_RATE, MUTATION_RATE, POP_SIZE, CHROMO_LENGTH, GENE_LENGTH);
		
		cxClient = rect.right;
		cyClient = rect.bottom;

		//��������������
		hdcBackBuffer = CreateCompatibleDC(NULL);

		HDC hdc = GetDC(hwnd);

		hBitmap = CreateCompatibleBitmap(hdc, cxClient, cyClient);

		ReleaseDC(hwnd, hdc);

		hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);

	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);

		//fill our backbuffer with white
		BitBlt(hdcBackBuffer, 0, 0, cxClient, cyClient, NULL, NULL, NULL, WHITENESS);

		//render the map and best route
		if (Kare->Started())
		{
			Kare->Render(cxClient, cyClient, hdcBackBuffer);
		}
		else {
			Kare->Render(cxClient, cyClient, hdcBackBuffer); //EMap->Render(cxClient, cyClient, hdcBackBuffer);
		}

		//now blit backbuffer to front
		BitBlt(hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY);

		ReleaseDC(hwnd, hdc);

		EndPaint(hwnd, &ps);
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wparam)
		{
		case VK_UP:
		{
			EMap->MoveUp();
		}
		break;
		case VK_DOWN:
		{
			EMap->MoveDwon();
		}
		break;
		case VK_LEFT:
		{
			EMap->MoveLeft();
		}
		break;
		case VK_RIGHT:
		{
			EMap->MoveRight();
		}
		break;
		case VK_RETURN:
		{
			Kare->Run(hwnd);
		}
		break;
		default:
			break;
		}
		//this will call WM_PAINT 
		//�����ػ洰����Ϣ
		InvalidateRect(hwnd, NULL, TRUE);
		UpdateWindow(hwnd);
	}
	break;
	case WM_DESTROY:
	{

		// kill the application, this sends a WM_QUIT message 
		PostQuitMessage(0);

	}
	break;
	}
	return (DefWindowProc(hwnd, msg, wparam, lparam));
}

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE hprevinstance,
	LPSTR lpcmdline,
	int ncmdshow) 
{
	WNDCLASSEX winclass;
	HWND	   hwnd;
	MSG		   msg;

	// first fill in the window class stucture
	//������д������
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;//ָ��������Ϣ������
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = NULL;
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = szWindowClassName;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	//����˵�

	// register the window class
	//ע�ᴰ��
	if (!RegisterClassEx(&winclass))
		return 0;

	// create the window
	//��������
	if (!(hwnd = CreateWindowEx(NULL,
		szWindowClassName,
		szApplicationName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL,
		NULL,
		hinstance,
		NULL)))
		return 0;

	ShowWindow(hwnd, ncmdshow);
	UpdateWindow(hwnd);

	//enter the message loop
	//������Ϣ����ѭ��
	bool bDone = false;

	while (!bDone)
	{

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				// Stop loop if it's a quit message
				//������˳�����Ϣ����ֹѭ��
				bDone = true;
			}

			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}


		if (Kare->Started())
		{
			//update the gun
			//��ʼ����
			Kare->Epoch();

			//this will call WM_PAINT 
			//�����ػ洰����Ϣ
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
		}

	}//end while��ֹѭ��


	UnregisterClass(szWindowClassName, winclass.hInstance);


	return 0;
}