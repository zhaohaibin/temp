#ifndef ROLE_H
#define ROLE_H

#include <string>
#include <cstdint>

namespace file_role
{
	//�������˷��صģ��ļ���ɫֵ
	namespace FILE_ROLE_SERVER_VAL
	{
        static const uint64_t ROLE_HIDE   = 0;   // ���ɼ���
		static const uint64_t	ROLE_VIEWER = 17;				//�鿴��
		static const uint64_t	ROLE_EDITOR = 273;				//�༭��
		static const uint64_t	ROLE_OPERATOR = 4369;		//������
		static const uint64_t	ROLE_ADMIN = 69905;			//������
		static const uint64_t	ROLE_OWNER = 286331153;	//������
	};

	//����ϵͳʹ�õĽ�ɫֵ�����������˵�ֵת��Ϊ����ֵ
	//�Ա������ʱ�����ظ����׿���
	enum EKP_FILE_ROLE
	{
		EKP_FILE_ROLE_UNKOWN =-1,			//δ֪��ɫ
        EKP_FILE_ROLE_HIDE =0,			//���ɼ���ɫ
		EKP_FILE_ROLE_VIEWER,						//�鿴��
		EKP_FILE_ROLE_EDITOR,						//�༭��
		EKP_FILE_ROLE_OPERATOR,				//������
		EKP_FILE_ROLE_ADMIN,						//������
		EKP_FILE_ROLE_OWNER						//������
		
	};

	//��ɫʶ�𣬸��ݷ���˷��صĽ�ɫֵ��ʶ��ɱ��صĽ�ɫֵ
	EKP_FILE_ROLE role_identiy(uint64_t file_role_val);
    //��ɫʶ�𣬸��ݱ��صĽ�ɫֵ, ���ط���˵Ľ�ɫֵ.
    uint64_t server_role_identiy( file_role::EKP_FILE_ROLE client_role_val);

    std::wstring	get_desc(EKP_FILE_ROLE role);

    
};
#endif