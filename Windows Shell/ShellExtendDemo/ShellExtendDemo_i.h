

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed Jun 29 10:50:03 2016
 */
/* Compiler settings for ShellExtendDemo.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ShellExtendDemo_i_h__
#define __ShellExtendDemo_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IUserContextMenu_FWD_DEFINED__
#define __IUserContextMenu_FWD_DEFINED__
typedef interface IUserContextMenu IUserContextMenu;
#endif 	/* __IUserContextMenu_FWD_DEFINED__ */


#ifndef __UserContextMenu_FWD_DEFINED__
#define __UserContextMenu_FWD_DEFINED__

#ifdef __cplusplus
typedef class UserContextMenu UserContextMenu;
#else
typedef struct UserContextMenu UserContextMenu;
#endif /* __cplusplus */

#endif 	/* __UserContextMenu_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IUserContextMenu_INTERFACE_DEFINED__
#define __IUserContextMenu_INTERFACE_DEFINED__

/* interface IUserContextMenu */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IUserContextMenu;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4F2AD4AF-817B-40E6-A59C-AEB34562B387")
    IUserContextMenu : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IUserContextMenuVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUserContextMenu * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUserContextMenu * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUserContextMenu * This);
        
        END_INTERFACE
    } IUserContextMenuVtbl;

    interface IUserContextMenu
    {
        CONST_VTBL struct IUserContextMenuVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUserContextMenu_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUserContextMenu_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUserContextMenu_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUserContextMenu_INTERFACE_DEFINED__ */



#ifndef __ShellExtendDemoLib_LIBRARY_DEFINED__
#define __ShellExtendDemoLib_LIBRARY_DEFINED__

/* library ShellExtendDemoLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_ShellExtendDemoLib;

EXTERN_C const CLSID CLSID_UserContextMenu;

#ifdef __cplusplus

class DECLSPEC_UUID("052BD344-BC59-432D-B279-F0699CB0AFDC")
UserContextMenu;
#endif
#endif /* __ShellExtendDemoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


