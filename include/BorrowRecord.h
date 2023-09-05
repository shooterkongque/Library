#include <iostream>
#include <string>
#pragma once
class BorrowRecord
{
public:
	BorrowRecord();
	~BorrowRecord();
public:
	int m_nBorrowId;					//借阅ID
	int m_nBookId;					//图书ID
	int m_nUserId;					//用户ID
	std::string m_tBorrowDate;				//借阅时间
	std::string m_tShouldReturnDate;		//应还时间
	std::string m_tReturnDate;				//实际归还时间
	int m_nContinue;					//续借次数
};

