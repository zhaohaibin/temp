#ifndef ROLE_H
#define ROLE_H

#include <string>
#include <cstdint>

namespace file_role
{
	//服务器端返回的，文件角色值
	namespace FILE_ROLE_SERVER_VAL
	{
        static const uint64_t ROLE_HIDE   = 0;   // 不可见者
		static const uint64_t	ROLE_VIEWER = 17;				//查看者
		static const uint64_t	ROLE_EDITOR = 273;				//编辑者
		static const uint64_t	ROLE_OPERATOR = 4369;		//操作者
		static const uint64_t	ROLE_ADMIN = 69905;			//管理者
		static const uint64_t	ROLE_OWNER = 286331153;	//所有者
	};

	//本地系统使用的角色值，将服务器端的值转换为本地值
	//以便有组合时，本地更容易控制
	enum EKP_FILE_ROLE
	{
		EKP_FILE_ROLE_UNKOWN =-1,			//未知角色
        EKP_FILE_ROLE_HIDE =0,			//不可见角色
		EKP_FILE_ROLE_VIEWER,						//查看者
		EKP_FILE_ROLE_EDITOR,						//编辑者
		EKP_FILE_ROLE_OPERATOR,				//操作者
		EKP_FILE_ROLE_ADMIN,						//管理者
		EKP_FILE_ROLE_OWNER						//所有者
		
	};

	//角色识别，根据服务端返回的角色值，识别成本地的角色值
	EKP_FILE_ROLE role_identiy(uint64_t file_role_val);
    //角色识别，根据本地的角色值, 返回服务端的角色值.
    uint64_t server_role_identiy( file_role::EKP_FILE_ROLE client_role_val);

    std::wstring	get_desc(EKP_FILE_ROLE role);

    
};
#endif