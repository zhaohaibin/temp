#include "role.h"


namespace file_role
{
	file_role::EKP_FILE_ROLE role_identiy( uint64_t file_role_val )
	{
		using namespace FILE_ROLE_SERVER_VAL;
		switch(file_role_val)
		{
		case ROLE_VIEWER:
			return EKP_FILE_ROLE_VIEWER;
		case ROLE_EDITOR:
			return EKP_FILE_ROLE_EDITOR;
		case ROLE_OPERATOR:
			return EKP_FILE_ROLE_OPERATOR;
		case ROLE_ADMIN:
			return EKP_FILE_ROLE_ADMIN;
		case ROLE_OWNER:
			return EKP_FILE_ROLE_OWNER;
        case ROLE_HIDE:
            return EKP_FILE_ROLE_HIDE;
		default:
			return EKP_FILE_ROLE_UNKOWN;
		}
	}

    uint64_t server_role_identiy( file_role::EKP_FILE_ROLE client_role_val)
    {
        using namespace FILE_ROLE_SERVER_VAL;
        switch(client_role_val)
        {
        case EKP_FILE_ROLE_VIEWER:
            return ROLE_VIEWER;
        case EKP_FILE_ROLE_EDITOR:
            return ROLE_EDITOR;
        case EKP_FILE_ROLE_OPERATOR:
            return ROLE_OPERATOR;
        case EKP_FILE_ROLE_ADMIN:
            return ROLE_ADMIN;
        case EKP_FILE_ROLE_OWNER:
            return ROLE_OWNER;
        case EKP_FILE_ROLE_HIDE:
            return ROLE_HIDE;
        default:
            return ROLE_VIEWER;
        }
    }
#ifdef _WIN32
    std::wstring  get_desc(EKP_FILE_ROLE role)
    {       
        if (EKP_FILE_ROLE_VIEWER == role)
        {
            return L"查看者";
        }
        else if (EKP_FILE_ROLE_EDITOR == role)
        {
            return L"编辑者";
        }
        else if (EKP_FILE_ROLE_OPERATOR == role)
        {
            return L"操作者";
        }
        else if (EKP_FILE_ROLE_ADMIN == role)
        {
            return L"管理者";
        }
        else if (EKP_FILE_ROLE_OWNER == role)
        {
            return L"所有者";
        }
        else if (EKP_FILE_ROLE_HIDE == role)
        {
            return L"不可见";
        }
        else
            return L"未知角色";
    }
#endif
  
}