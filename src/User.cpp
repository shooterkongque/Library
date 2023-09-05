#include <iostream>
#include "User.h"
using std::cout;
User::User() {}
User::User(int id, std::string name, std::string pass, int role):m_nID(id),m_strName(name),
m_strPassword(pass),m_nRole(role)
{
	this->m_nID = id;
	this->m_strName = name;
	this->m_strPassword = pass;
	this->m_nRole = role;
}
User::~User(){}

