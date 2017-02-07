// AlarmApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "AlarmApp.h"
#include <fstream>
#include <vector>
#include <string>
#include <commdlg.h>
#include <ctime>
#include "C:\Users\David\Documents\Visual Studio 2015\MyHeaders\GUI.h"

#define MAX_LOADSTRING 100
#define IDM_OPENDIALOGBOX 12345
#define IDM_SAVEDIALOGBOX 12346

SYSTEMTIME TheCurrentTime;
WCHAR CurrentTime[10][7];

int HoldIntervals = 0, BeepInterval = 20;

std::vector<AddEditButtons>EditButton;
std::vector<AddPushButtons>PushButton;

//Compare to see if both WCHAR are equal
bool charEqual(WCHAR a[7], WCHAR b[7]) {

	for (int i = 0; i < 7; i++)
		if (a[i] != b[i])
			return false;

	return true;
}

//Convert string into wstring
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

//Read .txt file 
void ReadAndTransferToEditBox(wchar_t* fileName) {
	std::ifstream _outf(fileName);
	std::string _tempStr;
	int _tempInt = 1;

	//While there is still text
	while (!_outf.eof()) {
		_outf >> _tempStr;
		if (_tempInt >= 10)
			break;
		
		//convert string from text into wstring 
		std::wstring tempStr2 = s2ws(_tempStr);
		
		//Place each text into WCHAR array
		wsprintf(CurrentTime[_tempInt], __T("%s"), tempStr2.c_str());
		
		_tempInt++;
	}
	_outf.close();

	//Loop through WCHAR array and set text for each editbox
	for (int a = 1; a < 10; a++)
		SendMessage(EditButton[a].GUIHandle, WM_SETTEXT, (WPARAM)0, (LPARAM)CurrentTime[a]);
}

wchar_t filename[MAX_PATH];
//The Open Dialog box
int OpenFileName(HWND hwndOwner, wchar_t *filename) {
	OPENFILENAME ofn = { 0 };

	ofn.lStructSize = sizeof(ofn);
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST;
	ofn.hInstance = GetModuleHandle(0);
	ofn.hwndOwner = hwndOwner;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFile = filename;
	ofn.nFilterIndex = 1;
	//Change the lpstrFilter and lpstrDefExt variables to an appropriate file type for your application.
	ofn.lpstrFilter = L"Text file\0*.txt\0Any file\0*.*\0";
	ofn.lpstrDefExt = L"txt";

	return GetOpenFileName(&ofn);
}

//The Save Dialog box
int SaveFileName(HWND hwndOwner, wchar_t *filename) {
	OPENFILENAME ofn = { 0 };

	ofn.lStructSize = sizeof(ofn);
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST;
	ofn.hInstance = GetModuleHandle(0);
	ofn.hwndOwner = hwndOwner;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFile = filename;
	ofn.nFilterIndex = 1;
	//Change the lpstrFilter and lpstrDefExt variables to an appropriate file type for your application.
	ofn.lpstrFilter = L"Text file\0*.txt\0Any file\0*.*\0";
	ofn.lpstrDefExt = L"txt";

	return GetSaveFileName(&ofn);
}

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ALARMAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ALARMAPP));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ALARMAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ALARMAPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		//Get local time
		GetLocalTime(&TheCurrentTime);
		wsprintf(CurrentTime[0], __T("%.2i:%.2i%"), TheCurrentTime.wHour, TheCurrentTime.wMinute);

		int eWidth = 60, eHeight = 30;

		//Add gui EditBox
		EditButton.emplace_back(AddEditButtons());
		EditButton[0].CreateGUI(hWnd, 10, 10, eWidth, eHeight, CurrentTime[0]);

		EditButton.emplace_back(AddEditButtons());
		EditButton[1].CreateGUI(hWnd, 10, 45, eWidth, eHeight);

		EditButton.emplace_back(AddEditButtons());
		EditButton[2].CreateGUI(hWnd, 80, 45, eWidth, eHeight);

		EditButton.emplace_back(AddEditButtons());
		EditButton[3].CreateGUI(hWnd, 10, 80, eWidth, eHeight);

		EditButton.emplace_back(AddEditButtons());
		EditButton[4].CreateGUI(hWnd, 80, 80, eWidth, eHeight);

		EditButton.emplace_back(AddEditButtons());
		EditButton[5].CreateGUI(hWnd, 10, 115, eWidth, eHeight);

		EditButton.emplace_back(AddEditButtons());
		EditButton[6].CreateGUI(hWnd, 80, 115, eWidth, eHeight);

		EditButton.emplace_back(AddEditButtons());
		EditButton[7].CreateGUI(hWnd, 10, 150, eWidth, eHeight);

		EditButton.emplace_back(AddEditButtons());
		EditButton[8].CreateGUI(hWnd, 80, 150, eWidth, eHeight);

		EditButton.emplace_back(AddEditButtons());
		EditButton[9].CreateGUI(hWnd, 10, 185, eWidth, eHeight);

		EditButton.emplace_back(AddEditButtons());
		EditButton[10].CreateGUI(hWnd, 10, 235, eWidth, eHeight, L"500");

		EditButton.emplace_back(AddEditButtons());
		EditButton[11].CreateGUI(hWnd, 80, 235, eWidth, eHeight, L"2");

		EditButton.emplace_back(AddEditButtons());
		EditButton[12].CreateGUI(hWnd, 10, 270, eWidth, eHeight, L"20");

		EditButton.emplace_back(AddEditButtons());
		EditButton[13].CreateGUI(hWnd, 80, 270, eWidth, eHeight, L"500");


		//Add gui Buttons
		PushButton.emplace_back(AddPushButtons());
		PushButton[0].CreateGUI(hWnd, 10, 305, eWidth, 20, L"Set1");

		PushButton.emplace_back(AddPushButtons());
		PushButton[1].CreateGUI(hWnd, 80, 305, eWidth, 20, L"Set2");

		PushButton.emplace_back(AddPushButtons());
		PushButton[2].CreateGUI(hWnd, 10, 330, eWidth, 20, L"Set3");

		PushButton.emplace_back(AddPushButtons());
		PushButton[3].CreateGUI(hWnd, 10, 365, eWidth, 20, L"Set Alarm");

		PushButton.emplace_back(AddPushButtons());
		PushButton[4].CreateGUI(hWnd, 80, 365, eWidth, 20, L"Test");

		//Set Window Origin and Size
		MoveWindow(hWnd, 10, 10, 190, 450, true);

		SetTimer(hWnd, 1003, 1000, NULL);

		//Set the toolbar menu
		HMENU hMenu, hSubMenu, hSubMenu2;
		hMenu = CreateMenu();

		hSubMenu = CreatePopupMenu();
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, L"&File");
		AppendMenu(hSubMenu, MF_STRING, IDM_OPENDIALOGBOX, L"Open");
		AppendMenu(hSubMenu, MF_STRING, IDM_SAVEDIALOGBOX, L"Save");
		AppendMenu(hSubMenu, MF_STRING, IDM_EXIT, L"E&xit");
		SetMenu(hWnd, hMenu);

		break;
	}
	case WM_TIMER:
	{
		switch (wParam)
		{
		case 1003:
		{
			//Get the current time
			GetLocalTime(&TheCurrentTime);
			wsprintf(CurrentTime[0], __T("%.2i:%.2i"), TheCurrentTime.wHour, TheCurrentTime.wMinute);
			SendMessage(EditButton[0].GUIHandle, WM_SETTEXT, (WPARAM)0, (LPARAM)CurrentTime[0]);

			//the interval between beeps
			if (HoldIntervals >= BeepInterval) {
				int BeepHertz = GetDlgItemInt(hWnd, EditButton[10].gLabelID, false, false);
				int BeepCount = GetDlgItemInt(hWnd, EditButton[11].gLabelID, false, false);
				BeepInterval = GetDlgItemInt(hWnd, EditButton[12].gLabelID, false, false);
				int BeepDuration = GetDlgItemInt(hWnd, EditButton[13].gLabelID, false, false);

				//If current time is equal to target
				if (charEqual(CurrentTime[0], CurrentTime[1]) == true ||
					charEqual(CurrentTime[0], CurrentTime[2]) == true ||
					charEqual(CurrentTime[0], CurrentTime[3]) == true ||
					charEqual(CurrentTime[0], CurrentTime[4]) == true ||
					charEqual(CurrentTime[0], CurrentTime[5]) == true ||
					charEqual(CurrentTime[0], CurrentTime[6]) == true ||
					charEqual(CurrentTime[0], CurrentTime[7]) == true ||
					charEqual(CurrentTime[0], CurrentTime[8]) == true ||
					charEqual(CurrentTime[0], CurrentTime[9]) == true) {

					Beep(BeepHertz, BeepDuration);

					static int _tempCounter = 0;
					_tempCounter++;

					if (_tempCounter >= BeepCount) {
						HoldIntervals = 0;
						_tempCounter = 0;
					}
				}
			}
			HoldIntervals++;
			break;
		}
		}
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		// Parse the menu selections:

		//When click on buttons
		if (wmEvent == BN_CLICKED) {
			if (wmId == PushButton[0].gLabelID) { //Bring up Set 1
				ReadAndTransferToEditBox(L"TimeSet1.txt");
			}
			else if (wmId == PushButton[1].gLabelID) { // 2 
				ReadAndTransferToEditBox(L"TimeSet2.txt");
			}
			else if (wmId == PushButton[2].gLabelID) { // 3
				ReadAndTransferToEditBox(L"TimeSet3.txt");
			}
			else if (wmId == PushButton[3].gLabelID) { //Set Alarm
				HoldIntervals = 0;
				BeepInterval = GetDlgItemInt(hWnd, EditButton[12].gLabelID, false, false);
				for (int a = 1; a < 10; a++)
					SendMessage(EditButton[a].GUIHandle, WM_GETTEXT, (WPARAM)7, (LPARAM)CurrentTime[a]);
			}
			else if (wmId == PushButton[4].gLabelID) { //Test beep
				int BeepHertz = GetDlgItemInt(hWnd, EditButton[10].gLabelID, false, false);
				int BeepDuration = GetDlgItemInt(hWnd, EditButton[13].gLabelID, false, false);
				Beep(BeepHertz, BeepDuration);
			}
		}

		switch (wmId)
		{
		case IDM_OPENDIALOGBOX:{
			//Open text file from Open dialog box
			OpenFileName(NULL, filename);

			//Transfer text from file into edit boxes
			ReadAndTransferToEditBox(filename);
			break;
		}
		case IDM_SAVEDIALOGBOX:{
			SaveFileName(NULL, filename); //Opens a Save dialog box and stores path

			std::wofstream myfile(filename); //Using fstream to create files for now
			if (myfile.is_open()) {
				for (int a = 1; a < 10; a++) {
					SendMessage(EditButton[a].GUIHandle, WM_GETTEXT, (WPARAM)7, (LPARAM)CurrentTime[a]);
					myfile << CurrentTime[a] << "\n";
				}
				myfile.close();
			}
			else
				::MessageBox(NULL, __T("Unable to save file"), __T("Error"), NULL);

			break;
		}
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

