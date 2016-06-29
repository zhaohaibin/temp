// UserContextMenu.h : CUserContextMenu ������

#pragma once
#include "resource.h"       // ������


#include <shlobj.h>
#include "ShellExtendDemo_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CUserContextMenu

class ATL_NO_VTABLE CUserContextMenu :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CUserContextMenu, &CLSID_UserContextMenu>,
	public IUserContextMenu,
	public IShellExtInit,
	public IContextMenu3
{
public:
	CUserContextMenu()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_USERCONTEXTMENU)


BEGIN_COM_MAP(CUserContextMenu)
	COM_INTERFACE_ENTRY(IUserContextMenu)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	HRESULT STDMETHODCALLTYPE Initialize( 
		/* [annotation][unique][in] */ 
		__in_opt  PCIDLIST_ABSOLUTE pidlFolder,
		/* [annotation][unique][in] */ 
		__in_opt  IDataObject *pdtobj,
		/* [annotation][unique][in] */ 
		__in_opt  HKEY hkeyProgID);

	//
	HRESULT STDMETHODCALLTYPE QueryContextMenu( 
		/* [annotation][in] */ 
		__in  HMENU hmenu,
		/* [annotation][in] */ 
		__in  UINT indexMenu,
		/* [annotation][in] */ 
		__in  UINT idCmdFirst,
		/* [annotation][in] */ 
		__in  UINT idCmdLast,
		/* [annotation][in] */ 
		__in  UINT uFlags);

	HRESULT STDMETHODCALLTYPE InvokeCommand( 
		/* [annotation][in] */ 
		__in  CMINVOKECOMMANDINFO *pici);

	HRESULT STDMETHODCALLTYPE GetCommandString( 
		/* [annotation][in] */ 
		__in  UINT_PTR idCmd,
		/* [annotation][in] */ 
		__in  UINT uType,
		/* [annotation][in] */ 
		__reserved  UINT *pReserved,
		/* [annotation][out] */ 
		__out_awcount(!(uType & GCS_UNICODE), cchMax)  LPSTR pszName,
		/* [annotation][in] */ 
		__in  UINT cchMax);

	HRESULT STDMETHODCALLTYPE HandleMenuMsg( 
		/* [annotation][in] */ 
		__in  UINT uMsg,
		/* [annotation][in] */ 
		__in  WPARAM wParam,
		/* [annotation][in] */ 
		__in  LPARAM lParam);

	HRESULT STDMETHODCALLTYPE HandleMenuMsg2( 
		/* [annotation][in] */ 
		__in  UINT uMsg,
		/* [annotation][in] */ 
		__in  WPARAM wParam,
		/* [annotation][in] */ 
		__in  LPARAM lParam,
		/* [annotation][out] */ 
		__out_opt  LRESULT *plResult);

private:
	int i;
};

OBJECT_ENTRY_AUTO(__uuidof(UserContextMenu), CUserContextMenu)
