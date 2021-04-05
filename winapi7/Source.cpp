// --- ������������ ���������� ����
#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <string>

// --- �������� ������� �������� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// --- ���������� ����������
HINSTANCE hInst; 		// ���������� ���������� ����������
WCHAR ClassName[] = L"Window"; 		// �������� ������ ����
WCHAR AppTitle[] = L"Application Win32"; 	// ��������� �������� ����
UINT IdTimer = 0;

int WINAPI WinMain(HINSTANCE hInstance,
	// ���������� ���������� ����������
	HINSTANCE hPrevInstance, // � Win32 ������ ����� NULL
	LPSTR lpCmdLine,
	// ��������� �� ��������� ������. �� // ���������
	// ���������� �������� ������ �� ��������� ������.
	int nCmdShow
	// ����������, ��� ���������� ������������� 
	// ������������ �� �������: ������������
	// (nCmdShow = SW_SHOWMINNOACTIVE) 
	// ��� � ���� ��������� ���� 					//(nCmdShow = SW_SHOWNORMAL).
)
{
	WNDCLASS wc; 	// ��������� ��� ���������� � ������ ����
	HWND hWnd; 	// ���������� �������� ���� ����������
	MSG msg; 	// ��������� ��� �������� ���������
// ��������� ���������� ���������� ���������� � ����������
// ����������, ����� ��� ������������� ��������������� �� �
// ������� ����.
	hInst = hInstance;

	// --- ���������, ���� �� ���������� �������� �����.
	// ������������� �������� FindWindow, ������� ���������
	// ����� ���� �������� 
	// ������ �� ����� ������ ��� �� ��������� ����:
	// HWND FindWindow(LPCTSTR lpClassName,
	// LPCTSTRlpWindowName);
	// ����� �������� lpClassName ���������� ��������� ��
	// ��������� ������, � ������� ���������� �������� ���
	// ������ �������� ����. �� ���� ������ � ���� �� ������ 
	// ����� ������� ��������� ����. ���� ���������� ����� 
	// ���� � �������� ����������, �� ��� ��������� �������
	// �������� ����� �������� lpWindowName. ���� �� �������� 
	// ����� ����, �� �������� lpWindowName ����� �����
	// �������� NULL.
	if ((hWnd = FindWindow(ClassName, NULL)) != NULL)
	{
		// ������������ ����� �� �������, ����� ���������� ���
		// ��������, � ����� ���. ����� �� ��������� ����������, 
		// �� �������, ��� �� ������ �������� ��� ������� ����.
		// �������, ���� ���������� ���� �������� �����,
		// ������������� �������������� � ��������� �� ��������
		// ���� ��� ������� ����. ��� ������ ��, � ���� ������������
		// ������������.
		if (IsIconic(hWnd)) ShowWindow(hWnd, SW_RESTORE);
		SetForegroundWindow(hWnd);

		// ������� ���������� ����� - ������ ����� �����
		// ������������.
		return FALSE;
	}
	srand(time(NULL));
	// --- ���������� ����� �� ������� - ������� WinMain
	// ���������� � �������������. ���������� ���������
	// WNDCLASS ��� ����������� ������ ����.
	memset(&wc, 0, sizeof(wc));
	wc.lpszClassName = ClassName;		// ��� ������ ����
	wc.lpfnWndProc = (WNDPROC)WndProc;
	// ����� ������� �������
	wc.style = CS_HREDRAW | CS_VREDRAW;	// ����� ������ 
	wc.hInstance = hInstance;		// ��������� ����������
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	// ����������� ��� ����
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	// ������ ���� ��� ����
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// ����� ��� ����
	wc.lpszMenuName = NULL;			// ������ ���� ����
	wc.cbClsExtra = 0;			// �������������� ������
	wc.cbWndExtra = 0;			// �������������� ������
	// P���������� ������ ����.
	RegisterClass(&wc);

	// ������� ������� ���� ����������.
	hWnd = CreateWindow(
		ClassName, 			// ��� ������ ����
		AppTitle,			// ��������� ���� 
		WS_OVERLAPPEDWINDOW, 		// ����� ����
		CW_USEDEFAULT,			// X-���������� 
		CW_USEDEFAULT,			// Y-���������� 
		CW_USEDEFAULT,			// ������ ����
		CW_USEDEFAULT,			// ������ ����
		NULL,			// ���������� ����-��������
		NULL,			// ���������� ���� ����
		hInst,		// ���������� ���������� ����������
		NULL);		// �������������� ����������
	if (!hWnd)
	{
		// ���� �� �������, ������ ��������������.
		MessageBox(NULL,
			L"Create: error", AppTitle, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	// ���������� ����.
	ShowWindow(hWnd, nCmdShow);

	// ��������� ���������� ���������� ������� ����.
	UpdateWindow(hWnd);


	// ��������� ���� ��������� ������� ���������.
	// ������� GetMessage �������� ��������� �� �������, 
	// ������ false ��� ������� �� ������� ��������� WM_QUIT
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


struct TREE
{
	int f;	// ���������� ���� ������� (��� �����������)
	int x, y; //���������� ����� ������ (��������������)
	int h; // ������ ��������� ������ ������ (������)
	COLORREF color; // ���� �����������
};

#define PI 3.14156

void ugolok(HDC hDC, int* X[], int* Y[], int* UG[],
	int* N, int dl, int ug0, int ug,
	int* X0, int* Y0, int* i); // ������� �������

void tree(HDC hDC, struct TREE tr) // ������� ������ ������
{
	int x0, y0, *X, *Y, ug, i, k, *UG, ug0, m, n, kvet, hvet, N;
	i = 0;
	k = 0;
	m = 1;
	ug0 = 28 + tr.f;
	// �������������
	N = (k + 1);
	// ������� ����� ��������� �������� - �������� ��������� � �����
	X = (int*)malloc(N * sizeof(int));
	Y = (int*)malloc(N * sizeof(int));
	UG = (int*)malloc(N * sizeof(int));

	// �������� �������� GDI (������� CreatePen)
	HPEN hpen = CreatePen(PS_SOLID, 1, tr.color);

	// ��������� ��������� ��������� ����������� (������� SelectObject)
	HGDIOBJ oldPen = SelectObject(hDC, hpen);

	srand(time(NULL));

	// ����� ������
	ug = UG[k] = ug0 * 3.2;
	x0 = X[k] = tr.x; 	y0 = Y[k] = tr.y;
	ugolok(hDC, &X, &Y, &UG, &N, tr.h, ug0, ug, &x0, &y0, &i);

	do // ������ ���� ������
	{
		hvet = rand() % 2 + 2; //�������� ���������� �����
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

	// �������������� ��������� ��������� �����������
	SelectObject(hDC, oldPen);

	// �������� �������� �������� GDI
	DeleteObject(hpen);

	free(X);
	free(Y);
	free(UG);
}

// ����� �����, ��������� �� �������
void ugolok(HDC hDC, int* X[], int* Y[], int* UG[], int* N, int dl, int ug0, int ug, int* X0, int* Y0, int* i)
{
	int x0 = *X0, y0 = *Y0, x1, y1, w; double grad = PI / 180.;
	if (*i == 0) w = 1; else w = 2;
	for (int a = 1; a <= w; a++)
	{
		(*i)++;
		if ((*i) + 1 > *N)
			// ���������� �������� �������������� ������
		{
			*N = (*i) + 1;
			*X = (int*)realloc(*X, (*N) * sizeof(int));
			*Y = (int*)realloc(*Y, (*N) * sizeof(int));
			*UG = (int*)realloc(*UG, (*N) * sizeof(int));
		}
		(*UG)[*i] = ug + ug0;
		(*X)[*i] = x1 = x0 + cos(ug * grad) * dl;
		(*Y)[*i] = y1 = y0 - sin(ug * grad) * dl;

		// ����� ��������� ����� �� (x0,y0) �� (x1,y1)
		// (������� MoveToEx � LineTo)
		MoveToEx(hDC, x0, y0, NULL);
		LineTo(hDC, x1, y1);\
		ug -= ug0 * 2 + (rand() % 26 - 15);
	}
}

TREE tr{};

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

// --- ������� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		init(hWnd);
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
		InvalidateRect(hWnd, NULL, true);
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