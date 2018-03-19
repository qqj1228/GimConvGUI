// GimConvGUI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GimConvGUI.h"


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
THREADPARAM tp;
DWORD dwOption;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GIMCONVGUI, szWindowClass, MAX_LOADSTRING);

	hInst=hInstance;
	uFMOD_PlaySong(data, (void*)51355, XM_MEMORY);
	InitCommonControls();
	DialogBox(hInst,(LPCSTR)IDD_DIALOG_MAIN,NULL,(DLGPROC)DlgProc);
	uFMOD_StopSong();
	ExitProcess(NULL);
	return 0;
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	int wmId, wmEvent;
	HICON hIcon;
	OPENFILENAME ofn;
	char szFile[MAX_PATH], szFileName[MAX_PATH], *pCh, szOutFormat[5], szParam[MAX_PATH], szDwOption[MAX_PATH];
	static int iVol=64;
	static char szCurrentPath[MAX_PATH];

	switch (message)
	{
	case WM_INITDIALOG:
		GetCurrentDirectory(MAX_PATH,(LPTSTR)szCurrentPath);

		hIcon=LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
		if(hIcon)
			SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)".png(PNG File)");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)".gim(GIM File)");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)".bmp(BMP File)");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)".tga(TGA File)");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)".tm2(TIM2 File)");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)".dds(DDS File)");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)".avi(AVI File)");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_SETCURSEL, (WPARAM)0, 0);

		SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM) MAKELONG(0, 64));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)64);
		return (INT_PTR)TRUE;
	/*case WM_NOTIFY:
		switch (((LPNMHDR) lParam)->code)
		{
		case TRBN_THUMBPOSCHANGING:
			NMTRBTHUMBPOSCHANGING  *lpNMTrbThumbPosChanging = (NMTRBTHUMBPOSCHANGING*) lParam;
			uFMOD_SetVolume(lpNMTrbThumbPosChanging->dwPos);
			break;
		}
		return (INT_PTR)TRUE;*/
	case WM_HSCROLL:
		switch(LOWORD(wParam))
		{
		case SB_THUMBTRACK:
			uFMOD_SetVolume(HIWORD(wParam));
			break;
		case SB_LINELEFT:
			uFMOD_SetVolume(--iVol);
			break;
		case SB_LINERIGHT:
			uFMOD_SetVolume(++iVol);
			break;
		case SB_PAGELEFT:
			iVol-=SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_GETPAGESIZE, 0, 0);
			uFMOD_SetVolume(iVol);
			break;
		case SB_PAGERIGHT:
			iVol+=SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_GETPAGESIZE, 0, 0);
			uFMOD_SetVolume(iVol);
			break;
		}
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDC_BUTTON1:
			memset(szFile, 0, sizeof(szFile[MAX_PATH]));
			memset(szFileName, 0, sizeof(szFileName[MAX_PATH]));
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hDlg;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "All Support File\0*.gim;*.png;*.bmp;*.tga;*.tm2;*.dds;*.avi\0	\
							  Gim File\0*.gim\0													\
							  PNG File\0*.png\0													\
							  BMP File\0*.bmp\0													\
							  TGA File\0*.tga\0													\
							  TIM2 File\0*.tm2\0												\
							  DDS File\0*.dds\0													\
							  AVI File\0*.avi\0													\
							  All Files(*.*)\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = szFileName;
			ofn.nMaxFileTitle = sizeof(szFileName);
			ofn.lpstrInitialDir = NULL;
			//ofn.lpstrDefExt = "vlg";
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			if (GetOpenFileName(&ofn)!=TRUE) 
				break;
			SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), szFile);
			pCh=strrchr(szFileName, '.');
			*pCh='\0';
			SetWindowText(GetDlgItem(hDlg, IDC_EDIT3), szFileName);
			SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDC_EDIT1,EN_CHANGE), (LPARAM)GetDlgItem(hDlg, IDC_EDIT1));
			break;
		case IDC_BUTTON3:
			DialogBox(hInst, (LPCSTR)IDD_DIALOG1, NULL, (DLGPROC)OptionDlgProc);
			break;
		case IDOK:
			GetWindowText(GetDlgItem(hDlg, IDC_EDIT1), szFile, MAX_PATH);
			switch(SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_GETCURSEL, 0, 0))
			{
			case 0:
				sprintf_s(szOutFormat, 5, ".png");
				break;
			case 1:
				sprintf_s(szOutFormat, 5, ".gim");
				break;
			case 2:
				sprintf_s(szOutFormat, 5, ".bmp");
				break;
			case 3:
				sprintf_s(szOutFormat, 5, ".tga");
				break;
			case 4:
				sprintf_s(szOutFormat, 5, ".tm2");
				break;
			case 5:
				sprintf_s(szOutFormat, 5, ".dds");
				break;
			case 6:
				sprintf_s(szOutFormat, 5, ".avi");
				break;
			}
			memset(szDwOption, 0, MAX_PATH);
			if(dwOption & DXT1)
				sprintf_s(szDwOption, MAX_PATH, " -dxt1");
			else if(dwOption & DXT3)
				sprintf_s(szDwOption, MAX_PATH, " -dxt3");
			else if(dwOption & DXT5)
				sprintf_s(szDwOption, MAX_PATH, " -dxt5");
			else if(dwOption & DXT1EXT)
				sprintf_s(szDwOption, MAX_PATH, " -dxt1ext");
			else if(dwOption & DXT3EXT)
				sprintf_s(szDwOption, MAX_PATH, " -dxt3ext");
			else if(dwOption & DXT5EXT)
				sprintf_s(szDwOption, MAX_PATH, " -dxt5ext");
			else if(dwOption & IFRGBA8888)
				sprintf_s(szDwOption, MAX_PATH, " -IFrgba8888");
			else if(dwOption & IFRGBA4444)
				sprintf_s(szDwOption, MAX_PATH, " -IFrgba4444");
			else if(dwOption & IFRGBA5551)
				sprintf_s(szDwOption, MAX_PATH, " -IFrgba5551");
			else if(dwOption & IFRGBA5650)
				sprintf_s(szDwOption, MAX_PATH, " -IFrgba5650");
			else if(dwOption & INDEX4)
				sprintf_s(szDwOption, MAX_PATH, " -IFindex4");
			else if(dwOption & INDEX8)
				sprintf_s(szDwOption, MAX_PATH, " -IFindex8");
			else if(dwOption & INDEX16)
				sprintf_s(szDwOption, MAX_PATH, " -IFindex16");
			else if(dwOption & INDEX32)
				sprintf_s(szDwOption, MAX_PATH, " -IFindex32");

			if(dwOption & PFRGBA8888)
				strcat_s(szDwOption, MAX_PATH, " -PFrgba8888");
			else if(dwOption & PFRGBA4444)
				strcat_s(szDwOption, MAX_PATH, " -PFrgba4444");
			else if(dwOption & PFRGBA5551)
				strcat_s(szDwOption, MAX_PATH, " -PFrgba5551");
			else if(dwOption & PFRGBA5650)
				strcat_s(szDwOption, MAX_PATH, " -PFrgba5650");

			GetWindowText(GetDlgItem(hDlg, IDC_EDIT3), szFileName, MAX_PATH);
			sprintf_s(szParam, MAX_PATH, "%s\\gimconv.exe %s -o %s%s%s", szCurrentPath, szFile, szFileName, szOutFormat, szDwOption);

			Converter(hDlg, szFile, GetDlgItem(hDlg, IDC_EDIT2), szParam);
			break;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
		return (INT_PTR)TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		return TRUE;
	}
	return (INT_PTR)FALSE;
}

void Converter(HWND hWnd, LPSTR szFile, HWND hEdit, LPSTR szParam)
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead, hWrite;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		SetWindowText(hEdit, "Error On CreatePipe()");
		return;
	}

	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	if (!CreateProcess(NULL, szParam, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		SetWindowText(hEdit, "Error on CreateProcess()");
		return;
	}
	CloseHandle(hWrite);

	tp.hWnd=hWnd;
	tp.hEdit=hEdit;
	tp.hPipeRd=hRead;
	_beginthread(ReadPipeThreadProc, 0, &tp);
}

void __cdecl ReadPipeThreadProc(void * pParam)
{
	char szBufRd[4096] = {0};
	char szBufOri[4096];
	DWORD bytesRead;
	THREADPARAM *pTp;
	int iLineCount;

	pTp=(THREADPARAM*)pParam;
	while (true)
	{
		if (ReadFile(pTp->hPipeRd, szBufRd, 4095, &bytesRead, NULL) == NULL)
			break;
		GetWindowText(pTp->hEdit, szBufOri, 4095);
		strcat_s(szBufOri, 4095, szBufRd);
		SetWindowText(pTp->hEdit, szBufOri);
		iLineCount=SendMessage(pTp->hEdit, EM_GETLINECOUNT, 0, 0);
		SendMessage(pTp->hEdit, EM_LINESCROLL, 0, iLineCount);
		//Sleep(200);
	}
}

INT_PTR CALLBACK OptionDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"default");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"rgba8888");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"rgba4444");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"rgba5551");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"rgba5650");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"index4");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"index8");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"index16");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"index32");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"dxt1");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"dxt3");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"dxt5");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"dxt1ext");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"dxt3ext");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"dxt5ext");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_SETCURSEL, (WPARAM)0, 0);
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"default");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"rgba8888");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"rgba4444");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"rgba5551");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"rgba5650");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_SETCURSEL, (WPARAM)0, 0);
		dwOption=0;
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDOK:
			switch(SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_GETCURSEL, 0, 0))
			{
			case 0:
				dwOption|=0;
				break;
			case 1:
				dwOption|=IFRGBA8888;
				break;
			case 2:
				dwOption|=IFRGBA4444;
				break;
			case 3:
				dwOption|=IFRGBA5551;
				break;
			case 4:
				dwOption|=IFRGBA5650;
				break;
			case 5:
				dwOption|=INDEX4;
				break;
			case 6:
				dwOption|=INDEX8;
				break;
			case 7:
				dwOption|=INDEX16;
				break;
			case 8:
				dwOption|=INDEX32;
				break;
			case 9:
				dwOption|=DXT1;
				break;
			case 10:
				dwOption|=DXT3;
				break;
			case 11:
				dwOption|=DXT5;
				break;
			case 12:
				dwOption|=DXT1EXT;
				break;
			case 13:
				dwOption|=DXT3EXT;
				break;
			case 14:
				dwOption|=DXT5EXT;
				break;
			}
			switch(SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0))
			{
			case 0:
				dwOption|=0;
				break;
			case 1:
				dwOption|=PFRGBA8888;
				break;
			case 2:
				dwOption|=PFRGBA4444;
				break;
			case 3:
				dwOption|=PFRGBA5551;
				break;
			case 4:
				dwOption|=PFRGBA5650;
				break;
			}
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
		return (INT_PTR)TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		return TRUE;
	}
	return (INT_PTR)FALSE;
}