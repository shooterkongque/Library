#pragma once
#include <string>
class User
{
public:
	User();
	User(int id, std::string name, std:: string pass, int role);
	~User();
public:
	int m_nID;					    //用户ID
	std::string m_strName;				//用户名
	std::string m_strPassword;			//用户密码
	int m_nRole;					//用户角色，1表示管理员，2表示普通用户
};

