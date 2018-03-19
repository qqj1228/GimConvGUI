#pragma once

#include "resource.h"
#include "ufmod.h"
#include "xm_file.h"

#pragma comment(lib, "ComCtl32.lib")
#pragma comment(lib, "winmm.lib")

#define MAX_LOADSTRING 100
#define DXT1 0x01
#define DXT3 0x02
#define DXT5 0x04
#define DXT1EXT 0x08
#define DXT3EXT 0x10
#define DXT5EXT 0x20
#define IFRGBA8888 0x40
#define IFRGBA4444 0x80
#define IFRGBA5551 0x100
#define IFRGBA5650 0x200
#define INDEX4 0x400
#define INDEX8 0x800
#define INDEX16 0x1000
#define INDEX32 0x2000
#define PFRGBA8888 0x4000
#define PFRGBA4444 0x8000
#define PFRGBA5551 0x10000
#define PFRGBA5650 0x20000

typedef struct tagTHREADPARAM
{
	HWND hWnd;
	HWND hEdit;
	HANDLE hPipeRd;
}THREADPARAM;

// Forward declarations of functions included in this code module:
INT_PTR CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);
void				Converter(HWND hWnd, LPSTR szFile, HWND hEdit, LPSTR szParam);
void __cdecl		ReadPipeThreadProc(void * pParam);
INT_PTR CALLBACK	OptionDlgProc(HWND, UINT, WPARAM, LPARAM);