//图书类
#include <string>
#include <ctime>
#pragma once
class Book
{
public:
	Book();
	Book(int nBookID, std::string strBookName, std::string strAuthor, std::string strISBN, std::string strPub, time_t inDate, int nTotalNum, int nLeftNum);
	int GetBookID();
	void SetBookID(int nID);
	std::string GetBookName();
	void SetBookName(std::string strName);
	std::string GetAuthor();
	void SetAuthor(std::string strAuthor);
	std::string GetISBN();
	void SetISBN(std::string strISBN);
	std::string GetPub();
	void SetPub(std::string strPub);
	std::string GetInDate();
	void SetInDate(std::string inDate);
	int GetTotalNum();
	void SetTotalNum(int nTotalNum);
	int GetLeftNum();
	void SetLeftNum(int nLeftNum);
	~Book();
private:
	int    m_nBookID;			//ID
	std::string m_strBookName;		//名称
	std::string m_strAuthor;			//作者
	std::string m_strISBN;			//ISBN
	std::string m_strPub;			//出版社
	std::string m_inDate;			//入库日期
	int    m_nTotalNum;			//总量
	int    m_nLeftNum;			//余量s
};

