#include "Student.h"
#include <limits>
#include <string>
Student::Student()
{
}


Student::~Student()
{
}

void Student::ShowMenu()
{
	system("cls");
	std::cout << "==========================��ӭʹ��ͼ��ݺ�̨����ϵͳ��==========================\n";
	std::cout << "                              1.��ѯͼ��\n";
	std::cout << "                              2.����ͼ��\n";
	std::cout << "                              3.�黹ͼ��\n";
	std::cout << "                              4.���ļ�¼\n";
	std::cout << "                              5.����ͼ��\n";
	std::cout << "                              0.�˳���¼\n";
}


//����������ѯ�鼮
void Student::QueryBook()
{
	std::string strBookName;
	std::cout << "������������";
	std::cin >> strBookName;
	std::vector<Book> books;
	if (!m_dbUtil.isOpen)
	{
		m_dbUtil.OpenDB();
	}

	m_dbUtil.SelectBookByName(strBookName, books);
	if (!books.empty()) {
		std::vector<Book>::iterator vecIter;
		std::cout << "ID     ����            ����           ������       �������     �ݲ�����   ����" << std::endl;
		for (vecIter = books.begin(); vecIter != books.end(); vecIter++)
		{
			std::cout << std::setiosflags(std::ios::left) << std::setw(4) << vecIter->GetBookID()
				<< "  " << std::setw(14) << vecIter->GetBookName() << "  " << std::setw(10)
				<< vecIter->GetAuthor() << "  " << std::setw(14) << vecIter->GetPub() << "  "
				<< std::setw(14) << vecIter->GetInDate() << "  " << std::setw(8)
				<< vecIter->GetTotalNum() << "  " << std::setw(3) << vecIter->GetLeftNum() << std::endl;
		}
	}
	else {
		std::cout << "δ��ѯ��ͼ�飡";
	}
	std::cin.get();
	std::cin.get();
	return;
}

//����ͼ��ID�����鼮
void Student::BorrowBook()
{
	int nBookId;
	std::cout << "������ͼ��ID��";
	std::cin >> nBookId;
	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		std::cout << "�Ƿ����룡";
		std::cin.get();
		return;
	}
	Book book;
	book.SetBookID(-1);
	if (!m_dbUtil.isOpen)
	{
		m_dbUtil.OpenDB();
	}
	m_dbUtil.SelectBookById(nBookId, book);
	if (book.GetBookID() != -1)
	{
		std::cout << "ID     ����          ����           ������       �������     �ݲ�����   ����" << std::endl;
		std::cout << std::setiosflags(std::ios::left) << std::setw(4) << book.GetBookID() 
		<< "  " << std::setw(14) << book.GetBookName() << "  " << std::setw(10) 
		<< book.GetAuthor() << "  " << std::setw(14) << book.GetPub() << "  " 
		<< std::setw(14) << book.GetInDate() << "  " << std::setw(8) << book.GetTotalNum() 
			<< "  " << std::setw(3) << book.GetLeftNum() << std::endl;
		if (book.GetLeftNum() == 0)
		{
			std::cout << "���鼮��ȫ���������ʱ�޷�����!" << std::endl;
			std::cin.get();
			std::cin.get();
			return;
		}
		//ѯ���Ƿ����
		char chIsBorrow = 'a';
		std::cout << "�����Ȿ����?y/n" << std::endl;
		std::cin.get();
		std::cin >> chIsBorrow;
		while (true)
		{
			if (chIsBorrow == 'y' || chIsBorrow == 'Y')
			{

				//����
				BorrowRecord borrowRecord;
				borrowRecord.m_nBookId = book.GetBookID();
				borrowRecord.m_nUserId = this->m_nID;
				time_t tTemp = time(NULL);
				char szTime[16] = {0};
				m_timeUtil.TimeToString(tTemp, szTime);
                std::string bTime=szTime;
                borrowRecord.m_tBorrowDate = bTime;
				borrowRecord.m_tShouldReturnDate = m_timeUtil.AddMonth(tTemp);

				borrowRecord.m_tReturnDate = "";
				borrowRecord.m_nContinue = 0;
				m_dbUtil.AddBorrowRecord(borrowRecord);

				break;
			}
			else if (chIsBorrow == 'n' || chIsBorrow == 'N')
			{
				std::cout << "��ȡ�����ģ�";
				break;
			}
			else
			{
				std::cout << "��������,����������:";
				std::cin.get();
				std::cin >> chIsBorrow;
			}
		}
	}
	else
	{
		std::cout << "û���ҵ�IDΪ "<<nBookId<<" ���鼮";
	}
	std::cin.get();
	std::cin.get();
	return;
}

//���ݹ黹�鼮
bool Student::ReturnBook()
{
	Book book;
	User user;
	book.SetBookID(-1);
	std::vector<BorrowRecord> borrowRecords;
	if (!m_dbUtil.isOpen)
	{
		m_dbUtil.OpenDB();
	}
	//չʾ��δ�黹��ͼ���¼
	m_dbUtil.SelectBorrowRecordByUserId(borrowRecords, m_nID, 1);
	std::cout << "������û�й黹��ͼ��" << std::endl;
	std::vector<BorrowRecord>::iterator vecIter;
	std::cout << "ID       ����        ������    ��������       Ӧ������     ��������   �Ƿ�����" << std::endl;
	for (vecIter = borrowRecords.begin(); vecIter != borrowRecords.end(); vecIter++)
	{
		user = m_dbUtil.SelectUserById(vecIter->m_nUserId);
		m_dbUtil.SelectBookById(vecIter->m_nBookId, book);
		std::cout << std::setiosflags(std::ios::left) << std::setw(4) << vecIter->m_nBorrowId 
		<< "  " << std::setw(14) << book.GetBookName() << "  " << std::setw(6) << user.m_strName 
		<< "  " << std::setw(13) << vecIter->m_tBorrowDate << "  " << std::setw(13)
		<< vecIter->m_tShouldReturnDate << "  " << std::setw(13) << vecIter->m_tReturnDate
			<< "  " << std::setw(3) << vecIter->m_nContinue << std::endl;
	}
	std::cout << "��ѡ��Ҫ�黹ͼ��ļ�¼ID��";

	//ѡ��Ҫ���ı���
	int nRecordId;
	std::cin >> nRecordId;
	bool bIsFind = true;
	while (bIsFind){
		if (std::cin.fail()) {
			std::cin.clear(); // �������״̬��־
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // ������뻺����
			std::cout << "�������ID��Ч������������(����-1ȡ������): ";
			std::cin >> nRecordId;
			if (nRecordId == -1) {
				std::cout << "��ȡ�����飡\n";
				std::cin.get();
				std::cin.get();
				return false;
			}
		}
		else {
			for (vecIter = borrowRecords.begin(); vecIter != borrowRecords.end(); vecIter++)
			{
				if (nRecordId == vecIter->m_nBorrowId)
				{
					bIsFind = false;
					break;
				}
			}
			if (vecIter == borrowRecords.end())
			{
				std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
				std::cout << "���������ID���ڼ�¼�У�����������(����-1ȡ������): ";
				std::cin >> nRecordId;
				if (nRecordId == -1) {
					std::cout << "��ȡ�����飡\n";
					std::cin.get();
					std::cin.get();
					return false;
				}
			}
		}
	}

	char flag;
	std::cout << "ȷ�Ϲ黹�Ȿ��?y/n:";
	std::cin >> flag;
	if (flag == 'y' || flag == 'Y') {
		m_dbUtil.FinishBorrowRecord(vecIter->m_nBorrowId, vecIter->m_nBookId);
		std::cout << "����ɹ�!" << std::endl;
	}
	else if (flag == 'n' || flag == 'N') {
		std::cout << "��ȡ�����飡\n";
	}
	else {
		std::cout << "��������\n";
	}
	std::cin.get();
	std::cin.get();
	return true;
}

//չʾ���˽��ļ�¼
bool Student::ShowMyBorrowRecord()
{
	std::vector<BorrowRecord> borrowRecords;
	if (!m_dbUtil.isOpen)
	{
		m_dbUtil.OpenDB();
	}

	m_dbUtil.SelectBorrowRecordByUserId(borrowRecords, m_nID, 2);
	User user;
	Book book;
	std::cout << "�������������н��ļ�¼" << std::endl;
	std::vector<BorrowRecord>::iterator vecIter;
	std::cout << "ID       ����        ������    ��������       Ӧ������     ��������   �������" << std::endl;
	for (vecIter = borrowRecords.begin(); vecIter != borrowRecords.end(); vecIter++)
	{
		user = m_dbUtil.SelectUserById(vecIter->m_nUserId);
		m_dbUtil.SelectBookById(vecIter->m_nBookId, book);
		std::cout << std::setiosflags(std::ios::left) << std::setw(4) << vecIter->m_nBorrowId 
		<< "  " << std::setw(14) << book.GetBookName() << "  " << std::setw(6)
			<< user.m_strName << "  " << std::setw(13) << vecIter->m_tBorrowDate
			<< "  " << std::setw(13) << vecIter->m_tShouldReturnDate << "  " << std::setw(13)
			<< vecIter->m_tReturnDate << "  " << std::setw(3) << vecIter->m_nContinue << std::endl;
	}
	std::cin.get();
	std::cin.get();
	return true;
}

//����ͼ��
bool Student::RenewBook()
{
	Book book;
	User user;
	book.SetBookID(-1);
	std::vector<BorrowRecord> borrowRecords;
	if (!m_dbUtil.isOpen)
	{
		m_dbUtil.OpenDB();
	}
	//չʾ��δ�黹��ͼ���¼
	m_dbUtil.SelectBorrowRecordByUserId(borrowRecords, m_nID, 1);
	std::cout << "������û�й黹��ͼ��" << std::endl;
	std::vector<BorrowRecord>::iterator vecIter;
	std::cout << "ID       ����        ������    ��������       Ӧ������     ��������   �������" << std::endl;
	for (vecIter = borrowRecords.begin(); vecIter != borrowRecords.end(); vecIter++)
	{
		user = m_dbUtil.SelectUserById(vecIter->m_nUserId);
		m_dbUtil.SelectBookById(vecIter->m_nBookId, book);
		std::cout << std::setiosflags(std::ios::left) << std::setw(4) << vecIter->m_nBorrowId 
		<< "  " << std::setw(14) << book.GetBookName() << "  " << std::setw(6) << user.m_strName 
		<< "  " << std::setw(13) << vecIter->m_tBorrowDate << "  " << std::setw(13) << vecIter->m_tShouldReturnDate
		<< "  " << std::setw(13) << vecIter->m_tReturnDate << "  " << std::setw(3) << vecIter->m_nContinue << std::endl;
	}
	std::cout << "��ѡ��Ҫ����ͼ��ļ�¼ID��";
	//ѡ��Ҫ�����ı���
	int nRecordId;
	std::cin >> nRecordId;
	bool bIsNotFind = true;
	bool bCanContinue = false;
	int giveup = 1;
	while (bIsNotFind)
	{
		if (std::cin.fail()){
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
			std::cout << "�������ID��Ч������������(����-1ȡ������): ";
			std::cin >> nRecordId;
			if (nRecordId == -1) {
				std::cout << "�Ѿ�ȡ�����裡\n";
				std::cin.get();
				std::cin.get();
				return false;
			}
		}
		else {
			for (vecIter = borrowRecords.begin(); vecIter != borrowRecords.end(); vecIter++){
				if (nRecordId == vecIter->m_nBorrowId){
					bIsNotFind = false;
					if (vecIter->m_nContinue > 0){
						bCanContinue = false;
					}
					else{
						bCanContinue = true;
					}

					break;
				}
			}
			if (vecIter == borrowRecords.end()) {
				std::cout << "���������ID���ڼ�¼�У�����������(����-1ȡ������):";
				std::cin >> nRecordId;
				if (nRecordId == -1) {
					std::cout << "�Ѿ�ȡ�����裡\n";
					std::cin.get();
					std::cin.get();
					return false;
				}
			}
		}	
	}

	if (true == bCanContinue)
	{
		char flag;
		std::cout << "ȷ������?y/n:";
		std::cin >> flag;
		if (flag == 'y' || flag == 'Y') {
			m_dbUtil.ExtendBorrowRecord(nRecordId);
			std::cout << "����ɹ�!" << std::endl;
		}
		else if(flag == 'n' || flag == 'N'){
			std::cout << "�Ѿ�ȡ�����裡\n";
		}	
		else {
			std::cout << "��������\n";
		}
	}
	else
	{
		std::cout << "���鼮�Ѿ������һ�Σ��޷��ٴ�����" << std::endl;
	}
	std::cin.get();
	std::cin.get();
	return true;

}