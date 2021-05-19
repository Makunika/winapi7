// --- Обязательный включаемый файл
#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <string>

#include "resource.h"

// --- Описание функции главного окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndPopUpProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


// --- Глобальные переменные
HINSTANCE hInst; 		// Дескриптор экземпляра приложения
WCHAR ClassName[] = L"Window"; 		// Название класса окна
WCHAR AppTitle[] = L"Application Win32"; 	// Заголовок главного окна
UINT IdTimer = 0;
HWND hWndMain;
HWND hDlg = NULL;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc, wcPopup;
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

	memset(&wcPopup, 0, sizeof(wcPopup));
	wcPopup.lpszClassName = L"popup";
	wcPopup.lpfnWndProc = (WNDPROC)WndPopUpProc;
	wcPopup.style = CS_HREDRAW | CS_VREDRAW;
	wcPopup.hInstance = hInstance;
	wcPopup.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcPopup.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcPopup.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcPopup.cbClsExtra = 0;
	wcPopup.cbWndExtra = 0;
	RegisterClass(&wcPopup);

	
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
	
	UpdateWindow(hWndMain);
	
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

HWND hbtnOk, hbtnOpenFile;
HWND hListBox;

LRESULT CALLBACK WndPopUpProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		hbtnOk = CreateWindow(L"button", L"Ок",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			10, 370, 120, 30, hWnd, 0, hInst, NULL);
		ShowWindow(hbtnOk, SW_SHOWNORMAL);
		hbtnOpenFile = CreateWindow(L"button", L"Открыть файл",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			140, 370, 120, 30, hWnd, 0, hInst, NULL);
		ShowWindow(hbtnOpenFile, SW_SHOWNORMAL);
		hListBox = CreateWindow(L"listbox", NULL,
			WS_CHILD | WS_VISIBLE | LBS_STANDARD |
			LBS_WANTKEYBOARDINPUT | LBS_DISABLENOSCROLL,
			10, 10, 810, 350,
			hWnd, 0, hInst, NULL);
		ShowWindow(hListBox, SW_SHOWNORMAL);
	}
	case WM_COMMAND:
	{
		if (lParam == (LPARAM)hbtnOk)
		{
			DestroyWindow(hWnd);
		}
		else if (lParam == (LPARAM)hbtnOpenFile)
		{
			WCHAR filesAll[1024];
			WCHAR filesTitle[1024];
			
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = NULL;
			ofn.lpstrTitle = L"Выбери файлики";
			ofn.lpstrFile = filesAll;
			ofn.lpstrFile[0] = '\0';
			ofn.lpstrFileTitle = filesTitle;
			ofn.lpstrFileTitle[0] = '\0';
			ofn.nMaxFileTitle = sizeof(filesTitle);
			ofn.nMaxFile = sizeof(filesAll);
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER;
			ofn.lStructSize = sizeof(ofn);
			if (GetOpenFileName(&ofn) == TRUE)
			{
				wchar_t* ptr = ofn.lpstrFile;
				ptr[ofn.nFileOffset - 1] = 0;
				std::wstring dir(ptr);
				ptr += ofn.nFileOffset;
				while (*ptr)
				{
					SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)(L"Папка: " + dir + L", файл: " + std::wstring(ptr)).c_str());
					ptr += (lstrlenW(ptr) + 1);
				}
			
			}
		}
		break;
	}
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0l;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			case IDM_OPENFILE:
			{
				HWND hWndPopUp = CreateWindow(
					L"popup", 			// Имя класса окон
					AppTitle,			// Заголовок окна 
					WS_POPUPWINDOW | WS_VISIBLE | WS_CAPTION, 		// Стиль окна
					CW_USEDEFAULT,			// X-координаты 
					CW_USEDEFAULT,			// Y-координаты 
					850,			// Ширина окна
					450,			// Высота окна
					hWnd,			// Дескриптор окна-родителя
					NULL,			// Дескриптор меню окна
					hInst,		// Дескриптор экземпляра приложения
					NULL);		// Дополнительная информация

				if (!hWndPopUp)
				{
					MessageBox(NULL,
						L"Create: error", AppTitle, MB_OK | MB_ICONSTOP);
					return FALSE;
				}
				ShowWindow(hWndPopUp, SW_NORMAL);
				UpdateWindow(hWndPopUp);
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
				MessageBox(hdlg, L"Красного больше 150 не должно быть", L"Ошибка", MB_OK);
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
			
			EndDialog(hWnd, IDOK);
			
			return TRUE;
		}
		case IDCANCEL:
		{
			EndDialog(hWnd, IDCANCEL);
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
			cc.Flags = CC_FULLOPEN | CC_RGBINIT | CC_ENABLEHOOK;
			cc.lpfnHook = (LPCCHOOKPROC) MyCCHookProc;

			if (ChooseColor(&cc))
			{
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