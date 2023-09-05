#include "Manager.h"

Manager::Manager()
{
}


Manager::~Manager()
{
}


void Manager::ShowMenu()
{
	system("cls");
	std::cout << "==========================��ӭʹ��ͼ��ݺ�̨����ϵͳ��==========================\n" ;
	std::cout << "                              1.����¼��\n";
	std::cout << "                              2.ͼ���ѯ\n";
	std::cout << "                              3.ɾ��ͼ��\n";
	std::cout << "                              4.ͼ���б�\n";
	std::cout << "                              5.���ļ�¼\n";
	std::cout << "                              6.�����û�\n";
	std::cout << "                              7.�û��б�\n";
	std::cout << "                              0.�˳���¼\n";
}

bool Manager::AddBook()
{
	std::string bookName;
	std::string author;
	std::string isbn;
	std::string pub;
	int total;
	char szTime[32] = {0};
	time_t lt;
	lt = time(NULL);
	m_timeUtil.TimeToString(lt, szTime);

	std::cout << "��ѡ���������ͼ�鹦�ܣ�����������ͼ����Ϣ" << std::endl;
	std::cout << "ͼ������    ����    ISBN    ������    �ܲ���" << std::endl;
	std::cin >> bookName;
	std::cin >> author;
	std::cin >> isbn;
	std::cin >> pub;
	std::cin >> total;

	Book book;
	book.SetBookName(bookName);
	book.SetAuthor(author);
	book.SetISBN(isbn);
	book.SetPub(pub);
	book.SetTotalNum(total);
	book.SetLeftNum(0);
	book.SetInDate(szTime);

	if (!m_dbUtil.isOpen)
	{
		m_dbUtil.OpenDB();
	}
	m_dbUtil.AddBook(book);
	std::cout << "����ͼ��ɹ�" << std::endl;
	system("pause");
	return true;
}

//չʾ�����鼮
bool Manager::DisplayAllBook()
{
	std::vector<Book> books;
	if (!m_dbUtil.isOpen)
	{
		m_dbUtil.OpenDB();
	}
	
	m_dbUtil.SelectAllBook(books);
	if (!books.empty())
	{
		std::vector<Book>::iterator vecIter;
		std::cout << "ID     ����            ����           ������       �������     �ݲ�����   ����" << std::endl;
		for (vecIter = books.begin(); vecIter != books.end(); vecIter++)
		{

			std::cout << std::setiosflags(std::ios::left) << std::setw(4) <<
				vecIter->GetBookID() << "  " << std::setw(14) << vecIter->GetBookName()
				<< "  " << std::setw(10) << vecIter->GetAuthor() << "  " << std::setw(14)
				<< vecIter->GetPub() << "  " << std::setw(14) << vecIter->GetInDate()
				<< "  " << std::setw(8) << vecIter->GetTotalNum() << "  " << std::setw(3)
				<< vecIter->GetLeftNum() << std::endl;
		}
	}
	else {
		std::cout << "ϵͳ��δ��¼ͼ����Ϣ��";
	}
	std::cin.get();
	std::cin.get();
	return true;
}

//����������ѯ�鼮
void Manager::QueryBook()
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
				<< std::setw(14) << vecIter->GetInDate() << "  " << std::setw(8) << vecIter->GetTotalNum()
				<< "  " << std::setw(3) << vecIter->GetLeftNum() << std::endl;
		}
	}
	else {
		std::cout << "δ��ѯ��ͼ��\n";
	}
	std::cin.get();
	std::cin.get();
	return;
}

//����ͼ��IDɾ��ͼ��
bool Manager::DeleteBook()
{
	int nBookId;
	std::cout << "������Ҫɾ��ͼ���ID" << std::endl;
	std::cin >> nBookId;
	Book book;
	book.SetBookID(-1);
	if (!m_dbUtil.isOpen)
	{
		m_dbUtil.OpenDB();
	}
	m_dbUtil.SelectBookById(nBookId, book);
	if (book.GetBookID() != -1)
	{
		std::cout << "ID     ����            ����           ������       �������     �ݲ�����   ����" << std::endl;
		std::cout << std::setiosflags(std::ios::left) << std::setw(4) << book.GetBookID() 
			<< "  " << std::setw(14) << book.GetBookName() << "  " << std::setw(10)
			<< book.GetAuthor() << "  " << std::setw(14) << book.GetPub() << "  "
			<< std::setw(14) << book.GetInDate() << "  " << std::setw(8) << book.GetTotalNum()
			<< "  " << std::setw(3) << book.GetLeftNum() << std::endl;

		//ѯ���Ƿ�ɾ��
		char chIsDelete = 'a';
		std::cout << "ɾ���Ȿ����?n/y" << std::endl;
		std::cin.get();
		std::cin >> chIsDelete;
		while (true)
		{
			if (chIsDelete == 'y' || chIsDelete == 'Y')
			{
				m_dbUtil.DeleteBookById(nBookId);
				break;
			}
			else if (chIsDelete == 'n' || chIsDelete == 'N')
			{
				std::cout << "��ȡ��ɾ����";
				break;
			}
			else
			{
				std::cout << "��������,����������:";
				std::cin.get();
				std::cin >> chIsDelete;
			}
		}
	}
	else
	{
		std::cout << "û���ҵ�IDΪ" << nBookId << "���鼮" << std::endl;
	}
	std::cin.get();
	std::cin.get();
	return true;
}

//չʾ���н��ļ�¼
bool Manager::DiaplayAllBorrowRecord()
{
	std::vector<BorrowRecord> borrowRecords;
	if (!m_dbUtil.isOpen)
	{ 
		m_dbUtil.OpenDB();
	}

	m_dbUtil.SelectAllBorrowRecord(borrowRecords);
	if (!borrowRecords.empty()) {
		User user;
		Book book;
		std::vector<BorrowRecord>::iterator vecIter;
		std::cout << "ID    ����        ������      ��������       Ӧ������     ��������   �������" << std::endl;
		for (vecIter = borrowRecords.begin(); vecIter != borrowRecords.end(); vecIter++)
		{
			user = m_dbUtil.SelectUserById(vecIter->m_nUserId);
			m_dbUtil.SelectBookById(vecIter->m_nBookId, book);
			std::cout << std::setiosflags(std::ios::left) << std::setw(4) << vecIter->m_nBorrowId <<
				"  " << std::setw(10) << book.GetBookName() << "  " << std::setw(8) <<
				user.m_strName << "  " << std::setw(13) << vecIter->m_tBorrowDate
				<< "  " << std::setw(13) << vecIter->m_tShouldReturnDate << "  "
				<< std::setw(13) << vecIter->m_tReturnDate << "  " << std::setw(3)
				<< vecIter->m_nContinue << std::endl;
		}
	}
	else
		std::cout << "ͼ����ļ�¼δ�գ�";
	std::cin.get();
	std::cin.get();
	return true;
}

//�����û�
bool Manager::AddUser()
{
	std::cout << "������һ���û���Ϣ:" << std::endl;
	std::cout << "�˻���   ����   �û�����" << std::endl;
	User user;

	std::cin >> user.m_strName;
	std::cin >> user.m_strPassword;
	std::cin >> user.m_nRole;

	if (!m_dbUtil.isOpen)
	{
		m_dbUtil.OpenDB();
	}

	m_dbUtil.AddUser(user);
	std::cout << "�����û��ɹ�" << std::endl;
	std::cin.get();
	std::cin.get();
	return true;
}

//չʾ�����û�
bool Manager::DisplayAllUser()
{
	std::vector<User> users;
	if (!m_dbUtil.isOpen)
	{
		m_dbUtil.OpenDB();
	}

	m_dbUtil.SelectAllUser(users);
	User user;
	Book book;
	std::vector<User>::iterator vecIter;
	std::cout << "ID     �û���      ���" << std::endl;
	for (vecIter = users.begin(); vecIter != users.end(); vecIter++)
	{
		std::cout << std::setiosflags(std::ios::left) << std::setw(6) << vecIter->m_nID 
			<< "  " << std::setw(8) << vecIter->m_strName << "  " 
			<< (vecIter->m_nRole == 1 ? "����Ա" : "ѧ��") << std::endl;
	}
	std::cin.get();
	std::cin.get();
	return true;
}

