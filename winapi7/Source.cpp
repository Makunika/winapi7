// --- Обязательный включаемый файл
#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <string>

// --- Описание функции главного окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// --- Глобальные переменные
HINSTANCE hInst; 		// Дескриптор экземпляра приложения
WCHAR ClassName[] = L"Window"; 		// Название класса окна
WCHAR AppTitle[] = L"Application Win32"; 	// Заголовок главного окна
UINT IdTimer = 0;

int WINAPI WinMain(HINSTANCE hInstance,
	// Дескриптор экземпляра приложения
	HINSTANCE hPrevInstance, // В Win32 всегда равен NULL
	LPSTR lpCmdLine,
	// Указатель на командную строку. Он // позволяет
	// приложению получать данные из командной строки.
	int nCmdShow
	// Определяет, как приложение первоначально 
	// отображается на дисплее: пиктограммой
	// (nCmdShow = SW_SHOWMINNOACTIVE) 
	// или в виде открытого окна 					//(nCmdShow = SW_SHOWNORMAL).
)
{
	WNDCLASS wc; 	// Структура для информации о классе окна
	HWND hWnd; 	// Дескриптор главного окна приложения
	MSG msg; 	// Структура для хранения сообщения
// Сохраняем дескриптор экземпляра приложения в глобальной
// переменной, чтобы при необходимости воспользоваться им в
// функции окна.
	hInst = hInstance;

	// --- Проверяем, было ли приложение запущено ранее.
	// Воспользуемся функцией FindWindow, которая позволяет
	// найти окно верхнего 
	// уровня по имени класса или по заголовку окна:
	// HWND FindWindow(LPCTSTR lpClassName,
	// LPCTSTRlpWindowName);
	// Через параметр lpClassName передается указатель на
	// текстовую строку, в которую необходимо записать имя
	// класса искомого окна. На базе одного и того же класса 
	// можно создать несколько окон. Если необходимо найти 
	// окно с заданным заголовком, то имя заголовка следует
	// передать через параметр lpWindowName. Если же подойдет 
	// любое окно, то параметр lpWindowName может иметь
	// значение NULL.
	if ((hWnd = FindWindow(ClassName, NULL)) != NULL)
	{
		// Пользователь может не помнить, какие приложения уже
		// запущены, а какие нет. Когда он запускает приложение, 
		// то ожидает, что на экране появится его главное окно.
		// Поэтому, если приложение было запущено ранее,
		// целесообразно активизировать и выдвинуть на передний
		// план его главное окно. Это именно то, к чему приготовился
		// пользователь.
		if (IsIconic(hWnd)) ShowWindow(hWnd, SW_RESTORE);
		SetForegroundWindow(hWnd);

		// Найдена работающая копия - работа новой копии
		// прекращается.
		return FALSE;
	}
	srand(time(NULL));
	// --- Работающая копия не найдена - функция WinMain
	// приступает к инициализации. Заполнение структуры
	// WNDCLASS для регистрации класса окна.
	memset(&wc, 0, sizeof(wc));
	wc.lpszClassName = ClassName;		// Имя класса окон
	wc.lpfnWndProc = (WNDPROC)WndProc;
	// Адрес оконной функции
	wc.style = CS_HREDRAW | CS_VREDRAW;	// Стиль класса 
	wc.hInstance = hInstance;		// Экземпляр приложения
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	// Пиктограмма для окон
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	// Курсор мыши для окон
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// Кисть для окон
	wc.lpszMenuName = NULL;			// Ресурс меню окон
	wc.cbClsExtra = 0;			// Дополнительная память
	wc.cbWndExtra = 0;			// Дополнительная память
	// Pегистрация класса окна.
	RegisterClass(&wc);

	// Создаем главное окно приложения.
	hWnd = CreateWindow(
		ClassName, 			// Имя класса окон
		AppTitle,			// Заголовок окна 
		WS_OVERLAPPEDWINDOW, 		// Стиль окна
		CW_USEDEFAULT,			// X-координаты 
		CW_USEDEFAULT,			// Y-координаты 
		CW_USEDEFAULT,			// Ширина окна
		CW_USEDEFAULT,			// Высота окна
		NULL,			// Дескриптор окна-родителя
		NULL,			// Дескриптор меню окна
		hInst,		// Дескриптор экземпляра приложения
		NULL);		// Дополнительная информация
	if (!hWnd)
	{
		// Окно не создано, выдаем предупреждение.
		MessageBox(NULL,
			L"Create: error", AppTitle, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	// Отображаем окно.
	ShowWindow(hWnd, nCmdShow);

	// Обновляем содержимое клиентской области окна.
	UpdateWindow(hWnd);


	// Запускаем цикл обработки очереди сообщений.
	// Функция GetMessage получает сообщение из очереди, 
	// выдает false при выборке из очереди сообщения WM_QUIT
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


struct TREE
{
	int f;	// определяет угол наклона (вид изображения)
	int x, y; //координаты корня дерева (местоположение)
	int h; // высота основного ствола дерева (размер)
	COLORREF color; // цвет изображения
};

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

// --- Функция окна
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