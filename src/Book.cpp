#include "Book.h"

Book::Book(){}

Book::Book(int nBookID, std::string strBookName, std::string strAuthor, std::string strISBN, std::string strPub, time_t inDate, int nTotalNum, int nLeftNum)
{
	m_nBookID = nBookID;
	m_strBookName = strBookName;
	m_strAuthor = strAuthor;
	m_strISBN = strISBN;
	m_strPub = strPub;
	m_inDate = inDate;
	m_nTotalNum = nTotalNum;
	m_nLeftNum = nLeftNum;
}


Book::~Book(){}

int Book::GetBookID()
{
	return m_nBookID;
}

void Book::SetBookID(int nID)
{
	m_nBookID = nID;
}

std::string Book::GetBookName()
{
	return m_strBookName;
}

void Book::SetBookName(std::string strName)
{
	m_strBookName = strName;
}

std::string Book::GetAuthor()
{
	return m_strAuthor;
}

void Book::SetAuthor(std::string strAuthor)
{
	m_strAuthor = strAuthor;
}

std::string Book::GetISBN()
{
	return m_strISBN;
}

void Book::SetISBN(std::string strISBN)
{
	m_strISBN = strISBN;
}

std::string Book::GetPub()
{
	return m_strPub;
}

void Book::SetPub(std::string strPub)
{
	m_strPub = strPub;
}

std::string Book::GetInDate()
{
	return m_inDate;
}

void Book::SetInDate(std::string inDate)
{
	m_inDate = inDate;
}

int Book::GetTotalNum()
{
	return m_nTotalNum;
}

void Book::SetTotalNum(int nTotalNum)
{
	m_nTotalNum = nTotalNum;
}

int Book::GetLeftNum()
{
	return m_nLeftNum;
}

void Book::SetLeftNum(int nLeftNum)
{
	m_nLeftNum = nLeftNum;
}
