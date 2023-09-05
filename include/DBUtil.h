#include <string.h>
#include <winsock.h>
#include <mysql.h> 
#include <iostream>
#include <sstream>
#include <vector>
#include "User.h"
#include "Book.h"
#include "TimeUtil.h"
#include "BorrowRecord.h"
#pragma once
class DBUtil
{
public:
	DBUtil();
	~DBUtil();
	bool CheckDatabaseExists(const std::string& databaseName);
	bool CreateDatabase(const std::string& databaseName);
	bool OpenDB();
	bool CloseDB();
	bool UserEmpty();

	bool enroll(const std::string& strUserName, const std::string& strUserPWD);

	User ReadUser(std::string strUserName, std::string strUserPWD);
	bool AddBook(Book book);
	bool SelectAllBook(std::vector<Book> &books);
	bool SelectBookByName(std::string strBookName, std::vector<Book> &books);
	bool SelectBookById(int nBookId, Book &book);
	bool DeleteBookById(int nBookId);
	bool AddBorrowRecord(BorrowRecord borrowRecord);
	bool ExtendBorrowRecord(int nRecordId);
	bool SelectBorrowRecordByRecordId(BorrowRecord &borrowRecord,int nRecordId);
	bool FinishBorrowRecord(int nRecordId, int nBookId);
	bool SelectAllBorrowRecord(std::vector<BorrowRecord> &borrowRecords);
	bool SelectBorrowRecordByUserId(std::vector<BorrowRecord> &borrowRecords, int nUserId, int nType);
	User SelectUserById(int nUserId);
	bool AddUser(User user);
	bool SelectAllUser(std::vector<User> &users);
public:
	std::string szUsername;			//用户名
	std::string szPswd;				//密码
	std::string szHost;				//主机名
	std::string szDatabase;			//数据库名
	int nPort;						//端口号  
	MYSQL myCont;
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
	MYSQL_FIELD *fd;
	bool isOpen;
	TimeUtil timeUtil;
};

