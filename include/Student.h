#pragma once
#include <stdlib.h>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <ctype.h>
#include <time.h>
#include "User.h"
#include "Book.h"
#include "TimeUtil.h"
#include "DBUtil.h"
#include "User.h"
#include "BorrowRecord.h"

class Student:public User
{
public:
	Student();
	~Student();
	void ShowMenu();
	void QueryBook();
	void BorrowBook();
	bool ReturnBook();
	bool ShowMyBorrowRecord();
	bool RenewBook();
public:
	TimeUtil m_timeUtil;
	DBUtil m_dbUtil;
};

