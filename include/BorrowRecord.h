#include <iostream>
#include <string>
#pragma once
class BorrowRecord
{
public:
	BorrowRecord();
	~BorrowRecord();
public:
	int m_nBorrowId;					//����ID
	int m_nBookId;					//ͼ��ID
	int m_nUserId;					//�û�ID
	std::string m_tBorrowDate;				//����ʱ��
	std::string m_tShouldReturnDate;		//Ӧ��ʱ��
	std::string m_tReturnDate;				//ʵ�ʹ黹ʱ��
	int m_nContinue;					//�������
};

