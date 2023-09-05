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
	std::cout << "==========================欢迎使用图书馆后台管理系统：==========================\n";
	std::cout << "                              1.查询图书\n";
	std::cout << "                              2.借阅图书\n";
	std::cout << "                              3.归还图书\n";
	std::cout << "                              4.借阅记录\n";
	std::cout << "                              5.续借图书\n";
	std::cout << "                              0.退出登录\n";
}


//根据书名查询书籍
void Student::QueryBook()
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
				<< std::setw(14) << vecIter->GetInDate() << "  " << std::setw(8)
				<< vecIter->GetTotalNum() << "  " << std::setw(3) << vecIter->GetLeftNum() << std::endl;
		}
	}
	else {
		std::cout << "未查询到图书！";
	}
	std::cin.get();
	std::cin.get();
	return;
}

//根据图书ID借阅书籍
void Student::BorrowBook()
{
	int nBookId;
	std::cout << "请输入图书ID：";
	std::cin >> nBookId;
	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		std::cout << "非法输入！";
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
		std::cout << "ID     书名          作者           出版社       入库日期     馆藏数量   余量" << std::endl;
		std::cout << std::setiosflags(std::ios::left) << std::setw(4) << book.GetBookID() 
		<< "  " << std::setw(14) << book.GetBookName() << "  " << std::setw(10) 
		<< book.GetAuthor() << "  " << std::setw(14) << book.GetPub() << "  " 
		<< std::setw(14) << book.GetInDate() << "  " << std::setw(8) << book.GetTotalNum() 
			<< "  " << std::setw(3) << book.GetLeftNum() << std::endl;
		if (book.GetLeftNum() == 0)
		{
			std::cout << "该书籍已全部借出，暂时无法借阅!" << std::endl;
			std::cin.get();
			std::cin.get();
			return;
		}
		//询问是否借阅
		char chIsBorrow = 'a';
		std::cout << "借阅这本书吗?y/n" << std::endl;
		std::cin.get();
		std::cin >> chIsBorrow;
		while (true)
		{
			if (chIsBorrow == 'y' || chIsBorrow == 'Y')
			{

				//借阅
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
				std::cout << "已取消借阅！";
				break;
			}
			else
			{
				std::cout << "输入有误,请重新输入:";
				std::cin.get();
				std::cin >> chIsBorrow;
			}
		}
	}
	else
	{
		std::cout << "没有找到ID为 "<<nBookId<<" 的书籍";
	}
	std::cin.get();
	std::cin.get();
	return;
}

//根据归还书籍
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
	//展示还未归还的图书记录
	m_dbUtil.SelectBorrowRecordByUserId(borrowRecords, m_nID, 1);
	std::cout << "已下是没有归还的图书" << std::endl;
	std::vector<BorrowRecord>::iterator vecIter;
	std::cout << "ID       书名        借阅人    借阅日期       应还日期     还书日期   是否续借" << std::endl;
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
	std::cout << "请选择要归还图书的记录ID：";

	//选择要还哪本书
	int nRecordId;
	std::cin >> nRecordId;
	bool bIsFind = true;
	while (bIsFind){
		if (std::cin.fail()) {
			std::cin.clear(); // 清除错误状态标志
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 清除输入缓冲区
			std::cout << "您输入的ID无效，请重新输入(输入-1取消还书): ";
			std::cin >> nRecordId;
			if (nRecordId == -1) {
				std::cout << "已取消还书！\n";
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
				std::cout << "您所输入的ID不在记录中，请重新输入(输入-1取消还书): ";
				std::cin >> nRecordId;
				if (nRecordId == -1) {
					std::cout << "已取消还书！\n";
					std::cin.get();
					std::cin.get();
					return false;
				}
			}
		}
	}

	char flag;
	std::cout << "确认归还这本书?y/n:";
	std::cin >> flag;
	if (flag == 'y' || flag == 'Y') {
		m_dbUtil.FinishBorrowRecord(vecIter->m_nBorrowId, vecIter->m_nBookId);
		std::cout << "还书成功!" << std::endl;
	}
	else if (flag == 'n' || flag == 'N') {
		std::cout << "已取消还书！\n";
	}
	else {
		std::cout << "输入有误\n";
	}
	std::cin.get();
	std::cin.get();
	return true;
}

//展示个人借阅记录
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
	std::cout << "以下是您的所有借阅记录" << std::endl;
	std::vector<BorrowRecord>::iterator vecIter;
	std::cout << "ID       书名        借阅人    借阅日期       应还日期     还书日期   续借次数" << std::endl;
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

//续借图书
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
	//展示还未归还的图书记录
	m_dbUtil.SelectBorrowRecordByUserId(borrowRecords, m_nID, 1);
	std::cout << "以下是没有归还的图书" << std::endl;
	std::vector<BorrowRecord>::iterator vecIter;
	std::cout << "ID       书名        借阅人    借阅日期       应还日期     还书日期   续借次数" << std::endl;
	for (vecIter = borrowRecords.begin(); vecIter != borrowRecords.end(); vecIter++)
	{
		user = m_dbUtil.SelectUserById(vecIter->m_nUserId);
		m_dbUtil.SelectBookById(vecIter->m_nBookId, book);
		std::cout << std::setiosflags(std::ios::left) << std::setw(4) << vecIter->m_nBorrowId 
		<< "  " << std::setw(14) << book.GetBookName() << "  " << std::setw(6) << user.m_strName 
		<< "  " << std::setw(13) << vecIter->m_tBorrowDate << "  " << std::setw(13) << vecIter->m_tShouldReturnDate
		<< "  " << std::setw(13) << vecIter->m_tReturnDate << "  " << std::setw(3) << vecIter->m_nContinue << std::endl;
	}
	std::cout << "请选择要续借图书的记录ID：";
	//选择要续借哪本书
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
			std::cout << "您输入的ID无效，请重新输入(输入-1取消续借): ";
			std::cin >> nRecordId;
			if (nRecordId == -1) {
				std::cout << "已经取消续借！\n";
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
				std::cout << "您所输入的ID不在记录中，请重新输入(输入-1取消续借):";
				std::cin >> nRecordId;
				if (nRecordId == -1) {
					std::cout << "已经取消续借！\n";
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
		std::cout << "确定续借?y/n:";
		std::cin >> flag;
		if (flag == 'y' || flag == 'Y') {
			m_dbUtil.ExtendBorrowRecord(nRecordId);
			std::cout << "续借成功!" << std::endl;
		}
		else if(flag == 'n' || flag == 'N'){
			std::cout << "已经取消续借！\n";
		}	
		else {
			std::cout << "输入有误！\n";
		}
	}
	else
	{
		std::cout << "此书籍已经续借过一次，无法再次续借" << std::endl;
	}
	std::cin.get();
	std::cin.get();
	return true;

}