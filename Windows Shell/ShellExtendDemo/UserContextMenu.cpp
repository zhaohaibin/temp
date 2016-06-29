// UserContextMenu.cpp : CUserContextMenu µÄÊµÏÖ

#include "stdafx.h"
#include "UserContextMenu.h"


// CUserContextMenu


HRESULT STDMETHODCALLTYPE CUserContextMenu::Initialize( 
	/* [annotation][unique][in] */ __in_opt PCIDLIST_ABSOLUTE pidlFolder,
	/* [annotation][unique][in] */ __in_opt IDataObject *pdtobj,
	/* [annotation][unique][in] */ __in_opt HKEY hkeyProgID )
{
	++i;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CUserContextMenu::QueryContextMenu( 
	/* [annotation][in] */ __in HMENU hmenu,
	/* [annotation][in] */ __in UINT indexMenu, 
	/* [annotation][in] */ __in UINT idCmdFirst, 
	/* [annotation][in] */ __in UINT idCmdLast, 
	/* [annotation][in] */ __in UINT uFlags )
{
	++i;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CUserContextMenu::InvokeCommand( 
	/* [annotation][in] */ __in CMINVOKECOMMANDINFO *pici )
{
	++i;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CUserContextMenu::GetCommandString( 
	/* [annotation][in] */ __in UINT_PTR idCmd, 
	/* [annotation][in] */ __in UINT uType, 
	/* [annotation][in] */ __reserved UINT *pReserved, 
	/* [annotation][out] */ __out_awcount(!(uType & GCS_UNICODE), cchMax) LPSTR pszName, 
	/* [annotation][in] */ __in UINT cchMax )
{
	++i;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CUserContextMenu::HandleMenuMsg( 
	/* [annotation][in] */ __in UINT uMsg, 
	/* [annotation][in] */ __in WPARAM wParam,
	/* [annotation][in] */ __in LPARAM lParam )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CUserContextMenu::HandleMenuMsg2(
	/* [annotation][in] */ __in UINT uMsg, 
	/* [annotation][in] */ __in WPARAM wParam, 
	/* [annotation][in] */ __in LPARAM lParam, 
	/* [annotation][out] */ __out_opt LRESULT *plResult )
{
	return S_OK;
}
