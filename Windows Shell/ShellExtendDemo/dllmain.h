// dllmain.h : 模块类的声明。

class CShellExtendDemoModule : public ATL::CAtlDllModuleT< CShellExtendDemoModule >
{
public :
	DECLARE_LIBID(LIBID_ShellExtendDemoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SHELLEXTENDDEMO, "{21B7BC15-2A82-4D0A-97AE-CB697F54E35C}")
};

extern class CShellExtendDemoModule _AtlModule;
