#include <stdlib.h>
#include <windows.h>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <ctype.h>
#include <time.h>
#include "User.h"
#include "Book.h"
#include "TimeUtil.h"
#include "DBUtil.h"
#pragma once
class Manager:public User
{
public:
	Manager();
	~Manager();
	void ShowMenu();
	bool AddBook();
	bool DisplayAllBook();
	void QueryBook();
	bool DeleteBook();
	bool DiaplayAllBorrowRecord();
	bool AddUser();
	bool DisplayAllUser();
	TimeUtil m_timeUtil;
	DBUtil m_dbUtil;
};

