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
	std::cout << "==========================欢迎使用图书馆后台管理系统：==========================\n" ;
	std::cout << "                              1.新书录入\n";
	std::cout << "                              2.图书查询\n";
	std::cout << "                              3.删除图书\n";
	std::cout << "                              4.图书列表\n";
	std::cout << "                              5.借阅记录\n";
	std::cout << "                              6.新增用户\n";
	std::cout << "                              7.用户列表\n";
	std::cout << "                              0.退出登录\n";
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

	std::cout << "你选择的是新增图书功能，请依次输入图书信息" << std::endl;
	std::cout << "图书名称    作者    ISBN    出版社    总册数" << std::endl;
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
	std::cout << "新增图书成功" << std::endl;
	system("pause");
	return true;
}

//展示所有书籍
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
		std::cout << "ID     书名            作者           出版社       入库日期     馆藏数量   余量" << std::endl;
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
		std::cout << "系统中未记录图书信息！";
	}
	std::cin.get();
	std::cin.get();
	return true;
}

//根据书名查询书籍
void Manager::QueryBook()
{
	std::string strBookName;
	std::cout << "请输入书名：";
	std::cin >> strBookName;
	std::vector<Book> books;
	if (!m_dbUtil.isOpen)
	{
		m_dbUtil.OpenDB();
	}

	m_dbUtil.SelectBookByName(strBookName, books);
	if (!books.empty()) {
		std::vector<Book>::iterator vecIter;
		std::cout << "ID     书名            作者           出版社       入库日期     馆藏数量   余量" << std::endl;
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
		std::cout << "未查询到图书\n";
	}
	std::cin.get();
	std::cin.get();
	return;
}

//根据图书ID删除图书
bool Manager::DeleteBook()
{
	int nBookId;
	std::cout << "请输入要删除图书的ID" << std::endl;
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
		std::cout << "ID     书名            作者           出版社       入库日期     馆藏数量   余量" << std::endl;
		std::cout << std::setiosflags(std::ios::left) << std::setw(4) << book.GetBookID() 
			<< "  " << std::setw(14) << book.GetBookName() << "  " << std::setw(10)
			<< book.GetAuthor() << "  " << std::setw(14) << book.GetPub() << "  "
			<< std::setw(14) << book.GetInDate() << "  " << std::setw(8) << book.GetTotalNum()
			<< "  " << std::setw(3) << book.GetLeftNum() << std::endl;

		//询问是否删除
		char chIsDelete = 'a';
		std::cout << "删除这本书吗?n/y" << std::endl;
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
				std::cout << "已取消删除！";
				break;
			}
			else
			{
				std::cout << "输入有误,请重新输入:";
				std::cin.get();
				std::cin >> chIsDelete;
			}
		}
	}
	else
	{
		std::cout << "没有找到ID为" << nBookId << "的书籍" << std::endl;
	}
	std::cin.get();
	std::cin.get();
	return true;
}

//展示所有借阅记录
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
		std::cout << "ID    书名        借阅人      借阅日期       应还日期     还书日期   续借次数" << std::endl;
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
		std::cout << "图书借阅记录未空！";
	std::cin.get();
	std::cin.get();
	return true;
}

//增加用户
bool Manager::AddUser()
{
	std::cout << "请输入一下用户信息:" << std::endl;
	std::cout << "账户名   密码   用户类型" << std::endl;
	User user;

	std::cin >> user.m_strName;
	std::cin >> user.m_strPassword;
	std::cin >> user.m_nRole;

	if (!m_dbUtil.isOpen)
	{
		m_dbUtil.OpenDB();
	}

	m_dbUtil.AddUser(user);
	std::cout << "新增用户成功" << std::endl;
	std::cin.get();
	std::cin.get();
	return true;
}

//展示所有用户
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
	std::cout << "ID     用户名      身份" << std::endl;
	for (vecIter = users.begin(); vecIter != users.end(); vecIter++)
	{
		std::cout << std::setiosflags(std::ios::left) << std::setw(6) << vecIter->m_nID 
			<< "  " << std::setw(8) << vecIter->m_strName << "  " 
			<< (vecIter->m_nRole == 1 ? "管理员" : "学生") << std::endl;
	}
	std::cin.get();
	std::cin.get();
	return true;
}

