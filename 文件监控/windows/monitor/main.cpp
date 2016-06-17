#include<stdio.h>  
#include<conio.h>  
#include<windows.h>  
#include <tchar.h>
#include<Shlobj.h>  
#pragma comment(lib,"Shell32.lib")  
using namespace std;  

long __stdcall WindowProc(HWND hwnd,unsigned int uMsg,PIDLIST_ABSOLUTE*wParam,long lParam)  
{  
	static wchar_t Buffer[2048];static HANDLE hStdOut;unsigned long count;  
	if(uMsg==WM_USER+123)  
	{  
		switch(lParam)  
		{  
		case SHCNE_RENAMEITEM:wcscpy(Buffer,L"SHCNE_RENAMEITEM:");break;  
		case SHCNE_CREATE:wcscpy(Buffer,L"SHCNE_CREATE:");break;  
		case SHCNE_DELETE:wcscpy(Buffer,L"SHCNE_DELETE:");break;  
		case SHCNE_MKDIR:wcscpy(Buffer,L"SHCNE_MKDIR:");break;  
		case SHCNE_RMDIR:wcscpy(Buffer,L"SHCNE_RMDIR:");break;  
		case SHCNE_MEDIAINSERTED:wcscpy(Buffer,L"SHCNE_MEDIAINSERTED:");break;  
		case SHCNE_MEDIAREMOVED:wcscpy(Buffer,L"SHCNE_MEDIAREMOVED:");break;  
		case SHCNE_DRIVEREMOVED:wcscpy(Buffer,L"SHCNE_DRIVEREMOVED:");break;  
		case SHCNE_DRIVEADD:wcscpy(Buffer,L"SHCNE_DRIVEADD:");break;  
		case SHCNE_NETSHARE:wcscpy(Buffer,L"SHCNE_NETSHARE:");break;  
		case SHCNE_NETUNSHARE:wcscpy(Buffer,L"SHCNE_NETUNSHARE:");break;  
		case SHCNE_ATTRIBUTES:wcscpy(Buffer,L"SHCNE_ATTRIBUTES:");break;  
		case SHCNE_UPDATEDIR:wcscpy(Buffer,L"SHCNE_UPDATEDIR:");break;  
		case SHCNE_UPDATEITEM:wcscpy(Buffer,L"SHCNE_UPDATEITEM:");break;  
		case SHCNE_SERVERDISCONNECT:wcscpy(Buffer,L"SHCNE_SERVERDISCONNECT:");break;  
		case SHCNE_UPDATEIMAGE:wcscpy(Buffer,L"SHCNE_UPDATEIMAGE:");break;  
		case SHCNE_DRIVEADDGUI:wcscpy(Buffer,L"SHCNE_DRIVEADDGUI:");break;  
		case SHCNE_RENAMEFOLDER:wcscpy(Buffer,L"SHCNE_RENAMEFOLDER:");break;  
		case SHCNE_FREESPACE:wcscpy(Buffer,L"SHCNE_FREESPACE:");break;  
		}  
		if(SHGetPathFromIDListW(*wParam,Buffer+wcslen(Buffer)))  
		{  
			WriteConsoleW(hStdOut,Buffer,wcslen(Buffer),&count,0);  
			if(SHGetPathFromIDListW(wParam[1],Buffer))  
			{  
				WriteConsoleW(hStdOut,L"->",2,&count,0);  
				wcscat(Buffer,L"\n");  
				WriteConsoleW(hStdOut,Buffer,wcslen(Buffer),&count,0);  
			}else WriteConsoleW(hStdOut,L"\n",1,&count,0);  
		}  
		return 0;  
	}else if(uMsg==WM_CREATE)  
	{  
		hStdOut=GetStdHandle(STD_OUTPUT_HANDLE);  
		return 0;  
	}else  
		return DefWindowProcW(hwnd,uMsg,(WPARAM)wParam,lParam);  
}  

int wmain(int argc, _TCHAR* argv[])  
{  
	static WNDCLASSEXW wc={sizeof(WNDCLASSEXW),0,(WNDPROC)WindowProc,0,0,0,0,0,0,0,L"a",0};  
	wc.hInstance=GetModuleHandleW(0);  
	HWND hwnd=CreateWindowExW(0,(wchar_t*)RegisterClassExW(&wc),0,0,0,0,0,0,HWND_MESSAGE,0,0,0);  
	static SHChangeNotifyEntry shcne={0,1};  
	SHGetFolderLocation(0,CSIDL_DESKTOP,0,0,const_cast<_ITEMIDLIST**>(&shcne.pidl));  
	ULONG id=SHChangeNotifyRegister(hwnd,SHCNRF_InterruptLevel|SHCNRF_ShellLevel|SHCNRF_RecursiveInterrupt,SHCNE_ALLEVENTS,WM_USER+123,1,&shcne);  
	if(!id)return 1;  
	MSG msg;  
	while(GetMessageW(&msg,0,0,0))DispatchMessageW(&msg);  
	return 0;  
}  