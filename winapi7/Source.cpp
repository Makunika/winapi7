// --- Обязательный включаемый файл
#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <string>
#include <commctrl.h>

#include "resource.h"

// --- Описание функции главного окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct TREE
{
	int f;	// определяет угол наклона (вид изображения)
	int x, y; //координаты корня дерева (местоположение)
	int h; // высота основного ствола дерева (размер)
	COLORREF color; // цвет изображения
};


// --- Глобальные переменные
HINSTANCE hInst; 		// Дескриптор экземпляра приложения
WCHAR ClassName[] = L"Window"; 		// Название класса окна
WCHAR AppTitle[] = L"Application Win32"; 	// Заголовок главного окна
UINT IdTimer = 0;
HWND hWndMain;
HWND hDlg = NULL;

int dTimer = 10000;

TREE tr {};

void init(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	tr.f = 0;
	tr.h = rand() % 20 + 20;
	tr.x = rand() % ((rect.right - 6 * tr.h) - 6 * tr.h) + 6 * tr.h;
	tr.y = rand() % (rect.bottom - 6 * tr.h) + 6 * tr.h;
	tr.color = RGB(rand() % 255, rand() % 255, rand() % 255);
}

void init(HWND hWnd, int f, int h, int x, int y)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	tr.f = f;
	tr.h = h;
	if (x < 6 * tr.h)
	{
		x = 6 * tr.h;
	}
	else if (x > rect.right - 6 * tr.h)
	{
		x = rect.right - 6 * tr.h;
	}

	tr.x = x;

	if (y < 6 * tr.h)
	{
		y = 6 * tr.h;
	}
	else if (y > rect.bottom)
	{
		y = rect.bottom;
	}
	
	tr.y = y;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;
	MSG msg;
	hInst = hInstance;
	
	if ((hWndMain = FindWindow(ClassName, NULL)) != NULL)
	{
		if (IsIconic(hWndMain)) ShowWindow(hWndMain, SW_RESTORE);
		SetForegroundWindow(hWndMain);
		return FALSE;
	}
	srand(time(NULL));
	
	memset(&wc, 0, sizeof(wc));
	wc.lpszClassName = ClassName;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	RegisterClass(&wc);

	hWndMain = CreateWindow(
		ClassName, 			// Имя класса окон
		AppTitle,			// Заголовок окна 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, 		// Стиль окна
		CW_USEDEFAULT,			// X-координаты 
		CW_USEDEFAULT,			// Y-координаты 
		CW_USEDEFAULT,			// Ширина окна
		CW_USEDEFAULT,			// Высота окна
		NULL,			// Дескриптор окна-родителя
		NULL,			// Дескриптор меню окна
		hInst,		// Дескриптор экземпляра приложения
		NULL);		// Дополнительная информация
	
	if (!hWndMain)
	{
		MessageBox(NULL,
			L"Create: error", AppTitle, MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	ShowWindow(hWndMain, nCmdShow);
	
	init(hWndMain);
	
	UpdateWindow(hWndMain);

	SetTimer(hWndMain, IdTimer, dTimer, (TIMERPROC)NULL);
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (hDlg == NULL || !IsDialogMessage(hDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

#define PI 3.14156

void ugolok(HDC hDC, int* X[], int* Y[], int* UG[],
	int* N, int dl, int ug0, int ug,
	int* X0, int* Y0, int* i); // рабочая функция

void tree(HDC hDC, struct TREE tr) // функция вывода дерева
{
	int x0, y0, *X, *Y, ug, i, k, *UG, ug0, m, n, kvet, hvet, N;
	i = 0;
	k = 0;
	m = 1;
	ug0 = 28 + tr.f;
	// инициализации
	N = (k + 1);
	// текущее число элементов массивов - хранилищ координат и углов
	X = (int*)malloc(N * sizeof(int));
	Y = (int*)malloc(N * sizeof(int));
	UG = (int*)malloc(N * sizeof(int));

	// создание объектов GDI (функция CreatePen)
	HPEN hpen = CreatePen(PS_SOLID, 1, tr.color);

	// установка атрибутов контекста отображения (функция SelectObject)
	HGDIOBJ oldPen = SelectObject(hDC, hpen);

	srand(time(NULL));

	// ствол дерева
	ug = UG[k] = ug0 * 3.2;
	x0 = X[k] = tr.x; 	y0 = Y[k] = tr.y;
	ugolok(hDC, &X, &Y, &UG, &N, tr.h, ug0, ug, &x0, &y0, &i);

	do // рисуем само дерево
	{
		hvet = rand() % 2 + 2; //параметр уменьшения ветки
		if (hvet == 2) kvet = 128 * 2;
		else kvet = 64 * 2;
		if (tr.h > hvet) tr.h -= hvet;
		else break;
		for (n = 1; n <= m; n++)
		{
			k++;
			ug = UG[k] + tr.f;
			x0 = X[k]; y0 = Y[k];
			ugolok(hDC, &X, &Y, &UG, &N, tr.h, ug0, ug, &x0, &y0, &i);
		}
		m = m * 2;
	} while (m <= kvet);

	// восстановление атрибутов контекста отображения
	SelectObject(hDC, oldPen);

	// удаление созданых объектов GDI
	DeleteObject(hpen);

	free(X);
	free(Y);
	free(UG);
}

// вывод веток, выходящих из вершины
void ugolok(HDC hDC, int* X[], int* Y[], int* UG[], int* N, int dl, int ug0, int ug, int* X0, int* Y0, int* i)
{
	int x0 = *X0, y0 = *Y0, x1, y1, w; double grad = PI / 180.;
	if (*i == 0) w = 1; else w = 2;
	for (int a = 1; a <= w; a++)
	{
		(*i)++;
		if ((*i) + 1 > *N)
			// необходимо выделить дополнительную память
		{
			*N = (*i) + 1;
			*X = (int*)realloc(*X, (*N) * sizeof(int));
			*Y = (int*)realloc(*Y, (*N) * sizeof(int));
			*UG = (int*)realloc(*UG, (*N) * sizeof(int));
		}
		(*UG)[*i] = ug + ug0;
		(*X)[*i] = x1 = x0 + cos(ug * grad) * dl;
		(*Y)[*i] = y1 = y0 - sin(ug * grad) * dl;

		// вывод очередной линии от (x0,y0) до (x1,y1)
		// (функции MoveToEx и LineTo)
		MoveToEx(hDC, x0, y0, NULL);
		LineTo(hDC, x1, y1);\
		ug -= ug0 * 2 + (rand() % 26 - 15);
	}
}

// --- Функция окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		HDC hDC;
		PAINTSTRUCT ps;
		RECT r;
		GetClientRect(hWnd, &r);
		hDC = BeginPaint(hWnd, &ps);

		
		tree(hDC, tr);
			
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		//InvalidateRect(hWnd, NULL, true);
		break;
	}
	case WM_TIMER:
	{
		InvalidateRect(hWnd, NULL, true);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			case IDM_CONF:
			{
				if (DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc))
				{
					InvalidateRect(hWnd, NULL, true);
				}
			}
		}
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0l;
}

UINT APIENTRY MyCCHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	static UINT uWM_ChooseColorOK = 0U;

	switch (uiMsg)
	{
	case WM_INITDIALOG:
		uWM_ChooseColorOK = RegisterWindowMessage(COLOROKSTRING);
		break;
	default:
		if (uWM_ChooseColorOK && uiMsg == uWM_ChooseColorOK)
		{
			COLORREF crResColor = ((LPCHOOSECOLOR)lParam)->rgbResult;
			if (GetRValue(crResColor) >= 150)
			{
				MessageBox(NULL, L"Красного больше 150 не должно быть", L"Ошибка", MB_OK);
				return TRUE;
			}
		}
	}

	return FALSE;
}



BOOL CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		HWND hwndButton = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"OK",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			10,         // x position 
			10,         // y position 
			100,        // Button width
			100,        // Button height
			hWnd,     // Parent window
			NULL,       // No menu.
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL);
		SetDlgItemText(hWnd, IDC_EDIT_F, std::to_wstring(tr.f).c_str());
		SetDlgItemText(hWnd, IDC_EDIT_X, std::to_wstring(tr.x).c_str());
		SetDlgItemText(hWnd, IDC_EDIT_Y, std::to_wstring(tr.y).c_str());
		SetDlgItemText(hWnd, IDC_EDIT_H, std::to_wstring(tr.h).c_str());
		SetDlgItemText(hWnd, IDC_EDIT_TIMER, std::to_wstring(dTimer).c_str());
		KillTimer(hWndMain, IdTimer);
		return TRUE;
	}
	case WM_CLOSE:
	{
		EndDialog(hWnd, 0);
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			std::wstring h = L"";
			GetDlgItemText(hWnd, IDC_EDIT_H, (LPWSTR)h.c_str(), 100);
			std::wstring x = L"";
			GetDlgItemText(hWnd, IDC_EDIT_X, (LPWSTR)x.c_str(), 100);
			std::wstring y = L"";
			GetDlgItemText(hWnd, IDC_EDIT_Y, (LPWSTR)y.c_str(), 100);
			std::wstring f = L"";
			GetDlgItemText(hWnd, IDC_EDIT_F, (LPWSTR)f.c_str(), 100);
			std::wstring timer = L"";
			GetDlgItemText(hWnd, IDC_EDIT_TIMER, (LPWSTR)timer.c_str(), 100);
			dTimer = std::stoi(timer);
			init(hWndMain, std::stoi(f), std::stoi(h), std::stoi(x), std::stoi(y));
			SetTimer(hWndMain, IdTimer, dTimer, (TIMERPROC)NULL);
			EndDialog(hWnd, TRUE);
			return TRUE;
		}
		case IDCANCEL:
		{
			EndDialog(hWnd, 0);
			return TRUE;
		}
		case IDC_BUTTON_COLOR:
		{
			CHOOSECOLOR cc;
			static COLORREF acrCustClr[16];
			ZeroMemory(&cc, sizeof(cc));
			cc.lStructSize = sizeof(cc);
			cc.hwndOwner = hWnd;
			cc.lpCustColors = (LPDWORD)acrCustClr;
			cc.rgbResult = tr.color;
			cc.Flags = CC_FULLOPEN | CC_RGBINIT | CC_ENABLEHOOK;
			cc.lpfnHook = MyCCHookProc;

			if (ChooseColor(&cc))
			{
				tr.color = cc.rgbResult;
			}
			return TRUE;
		}
		default:
		{
			break;
		}
		}
		
	}
	return FALSE;
}