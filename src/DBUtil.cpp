#include "DBUtil.h"


DBUtil::DBUtil()
{
	szUsername = "root";
	szPswd = "cfds";
	szHost = "localhost";
	szDatabase = "Library_wyq";
	nPort = 3306;
	isOpen = false;
}


DBUtil::~DBUtil()
{
	CloseDB();
}

//�����ݿ�
bool DBUtil::OpenDB()
{
	mysql_init(&myCont);
	if (mysql_real_connect(&myCont, szHost.c_str(), szUsername.c_str(), szPswd.c_str(), "mysql", nPort, NULL, 0))
	{
		//���Խ������ݿ⼰��
		int i = 0;
		bool flag = CreateDatabase(szDatabase);
		while (!flag && i < 3) { i++; flag = CreateDatabase(szDatabase); };
		if (flag) {
			isOpen = true;
			return true;
		}
		else {
			std::cerr << "ERROR:���ݿⴴ��ʧ�ܣ�\n";
			CloseDB(); //�ر����ݿ�
			exit(0);
		}
	}
	else {
		std::cerr << "ERROR:���ݿ�����ʧ�ܣ�\n";
		CloseDB();
		exit(0);
	}
	system("pause");
	return false;
}

//�ر����ݿ�
bool DBUtil::CloseDB()
{
	mysql_close(&myCont);//�Ͽ�����
	return true;
}

bool DBUtil::UserEmpty()
{
	std::string query = "select *from user ";
	int res = mysql_query(&myCont, query.c_str());
	if (res == 0) {
		result = mysql_store_result(&myCont);
		int numRows = mysql_num_rows(result);
		mysql_free_result(result);
		if (numRows == 0) {
			return true;	
		}
			
	}
	return false;
}

bool DBUtil::enroll(const std::string&strUserName,const std::string&strUserPWD)
{
	User user;
	user.m_strName = strUserName;
	user.m_strPassword = strUserPWD;
	user.m_nRole = 1;
	AddUser(user);
	return true;
}

bool DBUtil::CreateDatabase(const std::string& databaseName)
{
	std::string query;
	int res;

	//������ݿ�ͱ��Ƿ����
	query = "SHOW DATABASES LIKE '" + databaseName + "'";
	res = mysql_query(&myCont, query.c_str());
	if (res == 0) {
		result = mysql_store_result(&myCont);
		int numRows = mysql_num_rows(result);
		mysql_free_result(result);
		if (numRows > 0) {
			query = "USE " + databaseName;
			mysql_query(&myCont, query.c_str());
			query = "SHOW TABLES;";
			res = mysql_query(&myCont, query.c_str());
			if (res == 0) {
				result = mysql_store_result(&myCont);
				int numRow = mysql_num_rows(result);
				mysql_free_result(result);
				if (numRow == 3) {
					return true;
				}
					
			}
		}
	}

	// Create the database
	query = "CREATE DATABASE IF NOT EXISTS " + databaseName;
	res = mysql_query(&myCont, query.c_str());
	if (res != 0) {
		std::cerr << "Failed to create database." << std::endl;
		std::cerr << "Error: " << mysql_error(&myCont) << std::endl;
		return false;
	}

	// Use the InnoDB storage engine for all tables
	query = "USE " + databaseName;
	mysql_query(&myCont, query.c_str());

	// Create book table
	query = "CREATE TABLE IF NOT EXISTS `book` ("
		"`id` int(11) NOT NULL AUTO_INCREMENT,"
		"`bookname` varchar(30) DEFAULT NULL,"
		"`author` varchar(30) DEFAULT NULL,"
		"`isbn` varchar(30) DEFAULT NULL,"
		"`pub` varchar(30) DEFAULT NULL,"
		"`inDate` date DEFAULT NULL,"
		"`total` int(11) DEFAULT NULL,"
		"`left` int(11) DEFAULT NULL,"
		"PRIMARY KEY(`id`)"
		") ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;";
	res = mysql_real_query(&myCont, query.c_str(), query.size());
	if (res != 0) {
		std::cerr << "Failed to create book table." << std::endl;
		std::cerr << "Error: " << mysql_error(&myCont) << std::endl;
		return false;
	}

	// Create borrowrecord table
	query = "CREATE TABLE IF NOT EXISTS  `borrowrecord` ("
		"`id` int(11) NOT NULL AUTO_INCREMENT,"
		"`bookid` int(11) DEFAULT NULL,"
		"`userid` int(11) DEFAULT NULL,"
		"`borrowdate` date DEFAULT NULL,"
		"`shouldreturndate` date DEFAULT NULL,"
		"`returndate` date DEFAULT NULL,"
		"`continue` int(11) DEFAULT NULL,"
		"PRIMARY KEY(`id`)"
		") ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;";
	res = mysql_real_query(&myCont, query.c_str(), query.size());
	if (res != 0) {
		std::cerr << "Failed to create borrowrecord table." << std::endl;
		std::cerr << "Error: " << mysql_error(&myCont) << std::endl;
		return false;
	}

	// Create user table
	query = "CREATE TABLE IF NOT EXISTS `user` ("
		"`id` int(11) NOT NULL AUTO_INCREMENT,"
		"`name` varchar(20) DEFAULT NULL,"
		"`password` varchar(20) DEFAULT NULL,"
		"`usertype` int(11) DEFAULT NULL,"
		"PRIMARY KEY(`id`)"
		") ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;";
	res = mysql_real_query(&myCont, query.c_str(), query.size());
	if (res != 0) {
		std::cerr << "Failed to create user table." << std::endl;
		std::cerr << "Error: " << mysql_error(&myCont) << std::endl;
		return false;
	}

	return true;
}






//�����û������������û���Ϣ
User DBUtil::ReadUser(std::string strUserName, std::string strUserPWD)
{
	User user;
	char column[32][32];
	int res;
	std::string sql;
	if (isOpen)
	{
		mysql_query(&myCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
		sql += "select * from user where name='" + strUserName + "' and password='" + strUserPWD + "'";

		res = mysql_query(&myCont, sql.c_str());//��ѯ
		if (!res)
		{
			result = mysql_store_result(&myCont);//�����ѯ�������ݵ�result
			if (result->row_count > 0)
			{
				int i, j;
				//cout << "number of result: " << (unsigned long)mysql_num_rows(result) << endl;
				for (i = 0; fd = mysql_fetch_field(result); i++)//��ȡ����
				{
					strcpy_s(column[i], fd->name);
				}
				j = mysql_num_fields(result);

				if (j > 0)
				{
					sql_row = mysql_fetch_row(result);
					user.m_nID = atoi(sql_row[0]);
					user.m_strName = sql_row[1];
					user.m_nRole = atoi(sql_row[3]);
				}
			}
			else
			{
				user.m_nID = -1;
				return user;
			}
		}
		else
		{
			std::cout << "query sql failed!" << std::endl;
		}
	}
	else
	{
		std::cout << "connect failed!" << std::endl;
	}
	if (result != NULL)
	{
		mysql_free_result(result);//�ͷŽ����Դ
	}
	return user;
}

//����ͼ��
bool DBUtil::AddBook(Book book)
{
	std::string sql = "";
	char szTotal[16];
	char szLeftNum[16];
	sprintf_s(szTotal, "%d", book.GetTotalNum());
	sprintf_s(szLeftNum, "%d", book.GetTotalNum());

	if (isOpen)
	{
		//���ʱ��
		mysql_query(&myCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
		sql += "insert into book values(null,'" + book.GetBookName() + "','" + book.GetAuthor() + "','" + book.GetISBN() + "','" + book.GetPub() + "','" + book.GetInDate() + "', " + szTotal + ", " + szLeftNum + ")";
		mysql_query(&myCont, sql.c_str());
	}
	else
	{
		std::cout << "���ݿ�����ʧ��!" << std::endl;
	}
	std::cin.get();
	return false;

}

//��������ͼ��
bool DBUtil::SelectAllBook(std::vector<Book> &books)
{
	int res;
	std::string sql;
	if (isOpen)
	{
		mysql_query(&myCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
		sql += "select * from book";
		res = mysql_query(&myCont, sql.c_str());//��ѯ
		if (!res)
		{
			result = mysql_store_result(&myCont);//�����ѯ�������ݵ�result
			if (result)
			{
				while (sql_row = mysql_fetch_row(result))//��ȡ���������
				{
					Book book;
					book.SetBookID(atoi(sql_row[0]));
					book.SetBookName(sql_row[1]);
					book.SetAuthor(sql_row[2]);
					book.SetISBN(sql_row[3]);
					book.SetPub(sql_row[4]);
					book.SetInDate(sql_row[5]);
					book.SetTotalNum(atoi(sql_row[6]));
					book.SetLeftNum(atoi(sql_row[7]));
					books.push_back(book);
				}
			}
		}
		else
		{
			std::cout << "��ѯʧ��!" << std::endl;
		}
	}
	else
	{
		std::cout << "���ݿ�����ʧ��!" << std::endl;
	}
	if (result != NULL)
	{
		mysql_free_result(result);//�ͷŽ����Դ
	}

	return true;
}

//����������ѯ�鼮
bool DBUtil::SelectBookByName(std::string strBookName, std::vector<Book> &books)
{
	int res;
	std::string sql;
	if (isOpen)
	{
		mysql_query(&myCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
		sql += "select * from book where bookname like '%" + strBookName + "%'";
		res = mysql_query(&myCont, sql.c_str());//��ѯ
		if (!res)
		{
			result = mysql_store_result(&myCont);//�����ѯ�������ݵ�result
			if (result)
			{
				while (sql_row = mysql_fetch_row(result))//��ȡ���������
				{
					Book book;
					book.SetBookID(atoi(sql_row[0]));
					book.SetBookName(sql_row[1]);
					book.SetAuthor(sql_row[2]);
					book.SetISBN(sql_row[3]);
					book.SetPub(sql_row[4]);
					book.SetInDate(sql_row[5]);
					book.SetTotalNum(atoi(sql_row[6]));
					book.SetLeftNum(atoi(sql_row[7]));
					books.push_back(book);
				}
			}
		}
		else
		{
			std::cout << "��ѯʧ��!" << std::endl;
		}
	}
	else
	{
		std::cout << "���ݿ�����ʧ��!" << std::endl;
	}
	if (result != NULL)
	{
		mysql_free_result(result);//�ͷŽ����Դ
	}
	return true;
}

//����ͼ��ID��ѯ�鼮
bool DBUtil::SelectBookById(int nBookId, Book &book)
{
	int res;
	std::string sql;
	char szBookId[10] = {0};
	sprintf_s(szBookId,"%d", nBookId);
	if (isOpen)
	{
		mysql_query(&myCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
		sql += "select * from book where id=";
		sql += szBookId;
		res = mysql_query(&myCont, sql.c_str());//��ѯ
		if (!res)
		{
			result = mysql_store_result(&myCont);//�����ѯ�������ݵ�result
			if (result)
			{
				while (sql_row = mysql_fetch_row(result))//��ȡ���������
				{
					book.SetBookID(atoi(sql_row[0]));
					book.SetBookName(sql_row[1]);
					book.SetAuthor(sql_row[2]);
					book.SetISBN(sql_row[3]);
					book.SetPub(sql_row[4]);
					book.SetInDate(sql_row[5]);
					book.SetTotalNum(atoi(sql_row[6]));
					book.SetLeftNum(atoi(sql_row[7]));
				}
			}
		}
		else
		{
			std::cout << "query sql failed!" << std::endl;
		}
	}
	else
	{
		std::cout << "connect failed!" << std::endl;
	}
	if (result != NULL)
	{
		mysql_free_result(result);//�ͷŽ����Դ
	}
	return true;
}

//����IDɾ��ͼ��
bool DBUtil::DeleteBookById(int nBookId)
{
	int res;
	std::string sql;
	char szBookId[5] = {0};
	sprintf_s(szBookId, "%d", nBookId);
	if (isOpen)
	{
		mysql_query(&myCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
		sql += "delete from book where id=" ;
		sql += szBookId;
		res = mysql_query(&myCont, sql.c_str());//��ѯ
		if (!res)
		{
			std::cout << "ɾ���ɹ���" << std::endl;
		}
		else
		{
			std::cout << "query sql failed!" << std::endl;
		}
	}
	else
	{
		std::cout << "connect failed!" << std::endl;
	}

	return true;
}

//�������ļ�¼
bool DBUtil::AddBorrowRecord(BorrowRecord borrowRecord)
{
	std::string sql = "";
	char szBookId[16];
	char szUserId[16];
	sprintf_s(szBookId,10 ,"%d", borrowRecord.m_nBookId);
	sprintf_s(szUserId,10,"%d", borrowRecord.m_nUserId);
	if (isOpen)
	{
		sql = sql + "insert into borrowrecord values(null," + szBookId + "," + szUserId + ",'" + borrowRecord.m_tBorrowDate + "','" + borrowRecord.m_tShouldReturnDate + "',NULL,0)";
		mysql_query(&myCont, sql.c_str());

		sql = "";

		sql = sql + "update book set book.left=book.left-1 where id = " + szBookId;
		mysql_query(&myCont, sql.c_str());
		std::cout << "���ĳɹ�!" << std::endl;
	}
	else
	{
		std::cout << "connect failed!" << std::endl;
	}
	return false;

}


//�����û�ID����û���Ϣ
User DBUtil::SelectUserById(int nUserId)
{
	int res;
	std::string sql;
	User user;
	char szUserId[10] = { 0 };
	sprintf_s(szUserId,10 ,"%d", nUserId);
	//cout << "��ѯ��IDΪ��" << szUserId << endl;
	//system("pause");
	if (isOpen)
	{
		mysql_query(&myCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
		sql += "select * from user where id=";
		sql += szUserId;
		res = mysql_query(&myCont, sql.c_str());//��ѯ
		if (!res)
		{
			result = mysql_store_result(&myCont);//�����ѯ�������ݵ�result
			if (result)
			{
				while (sql_row = mysql_fetch_row(result))//��ȡ���������
				{
					user.m_nID = nUserId;
					user.m_strName = sql_row[1];
				}
			}
		}
		else
		{
			std::cout << "query sql failed!" << std::endl;
		}
	}
	else
	{
		std::cout << "connect failed!" << std::endl;
	}
	if (result != NULL)
	{
		mysql_free_result(result);//�ͷŽ����Դ
	}
	return user;
}

//��ѯ���н��ļ�¼
bool DBUtil::SelectAllBorrowRecord(std::vector<BorrowRecord> &borrowRecords)
{
	int res;
	std::string sql;
	if (isOpen)
	{
		mysql_query(&myCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
		sql += "select * from borrowrecord";
		res = mysql_query(&myCont, sql.c_str());//��ѯ
		if (!res)
		{
			result = mysql_store_result(&myCont);//�����ѯ�������ݵ�result
			if (result)
			{
				while (sql_row = mysql_fetch_row(result))//��ȡ���������
				{
					BorrowRecord borrowRecord;
					borrowRecord.m_nBorrowId = atoi(sql_row[0]);
					borrowRecord.m_nBookId = atoi(sql_row[1]);
					borrowRecord.m_nUserId = atoi(sql_row[2]);
					borrowRecord.m_tBorrowDate = sql_row[3];
					borrowRecord.m_tShouldReturnDate = sql_row[4];
					borrowRecord.m_tReturnDate = (sql_row[5] == NULL ? "" : sql_row[5]);
					borrowRecord.m_nContinue = atoi(sql_row[6]);
					borrowRecords.push_back(borrowRecord);
				}
			}
		}
		else
		{
			std::cout << "query sql failed!" << std::endl;
		}
	}
	else
	{
		std::cout << "connect failed!" << std::endl;
	}
	if (result != NULL)
	{
		mysql_free_result(result);//�ͷŽ����Դ
	}

	return true;
}

//��ѯĳλ�û��Ľ��ļ�¼ 
bool DBUtil::SelectBorrowRecordByUserId(std::vector<BorrowRecord> &borrowRecords, int nUserId, int nType)
{
	//nType=1��ʾѡ��δ�黹 nType=2��ʾѡ��ȫ��
	int res;
	std::string sql;
	char szUserId[8] = {0};
	sprintf_s(szUserId, "%d", nUserId);
	if (isOpen)
	{
		mysql_query(&myCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
		sql = sql + "select * from borrowrecord where userid=" + szUserId;
		res = mysql_query(&myCont, sql.c_str());//��ѯ
		if (!res)
		{
			result = mysql_store_result(&myCont);//�����ѯ�������ݵ�result
			if (result)
			{
				while (sql_row = mysql_fetch_row(result))//��ȡ���������
				{
					//�����ڿձ�ʾ�Ѿ��黹
					if (nType == 1)
					{
						if (sql_row[5] != NULL)
						{
							continue;
						}
					}
					BorrowRecord borrowRecord;
					borrowRecord.m_nBorrowId = atoi(sql_row[0]);
					borrowRecord.m_nBookId = atoi(sql_row[1]);
					borrowRecord.m_nUserId = atoi(sql_row[2]);
					borrowRecord.m_tBorrowDate = sql_row[3];
					borrowRecord.m_tShouldReturnDate = sql_row[4];
					borrowRecord.m_tReturnDate = (sql_row[5] == NULL ? "" : sql_row[5]);
					borrowRecord.m_nContinue = atoi(sql_row[6]);
					borrowRecords.push_back(borrowRecord);
				}
			}
		}
		else
		{
			std::cout << "��ѯʧ��!" << std::endl;
		}
	}
	else
	{
		std::cout << "���ݿ�����ʧ��!" << std::endl;
	}
	if (result != NULL)
	{
		mysql_free_result(result);//�ͷŽ����Դ
	}

	return true;
}

//�黹�鼮
bool DBUtil::FinishBorrowRecord(int nRecordId,int nBookId)
{
	std::string sql = "";
	char szRecordId[16];
	char szBookId[16];
	sprintf_s(szRecordId, 16,"%d", nRecordId);
	sprintf_s(szBookId, 16,"%d", nBookId);
	time_t tTemp = time(NULL);
	char szTime[16] = { 0 };
	timeUtil.TimeToString(tTemp, szTime);
	if (isOpen)
	{
		sql = sql + "update borrowrecord set returndate='" + szTime + "' where id=" + szRecordId;
		mysql_query(&myCont, sql.c_str());

		sql = "";

		sql = sql + "update book set book.left=book.left+1 where id = " + szBookId;
		mysql_query(&myCont, sql.c_str());
	}
	else
	{
		std::cout << "connect failed!" << std::endl;
	}
	return true;

}

//�����û�
bool DBUtil::AddUser(User user)
{
	std::string sql = "";
	char szRoleType[16];
	sprintf_s(szRoleType, "%d", user.m_nRole);

	if (isOpen)
	{
		sql += "insert into user values(null,'" + user.m_strName + "','" + user.m_strPassword + "'," + szRoleType + ")";
		mysql_query(&myCont, sql.c_str());
	}
	else
	{
		std::cout << "connect failed!" << std::endl;
	}
	return true;
}

//��ѯ�����û�
bool DBUtil::SelectAllUser(std::vector<User> &users)
{
	int res;
	std::string sql;
	if (isOpen)
	{
		mysql_query(&myCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
		sql += "select * from user";
		res = mysql_query(&myCont, sql.c_str());//��ѯ
		if (!res)
		{
			result = mysql_store_result(&myCont);//�����ѯ�������ݵ�result
			if (result)
			{
				while (sql_row = mysql_fetch_row(result))//��ȡ���������
				{
					User user;
					user.m_nID = atoi(sql_row[0]);
					user.m_strName = sql_row[1];
					user.m_nRole = atoi(sql_row[3]); 
					users.push_back(user);
				}
			}
		}
		else
		{
			std::cout << "query sql failed!" << std::endl;
		}
	}
	else
	{
		std::cout << "connect failed!" << std::endl;
	}
	if (result != NULL)
	{
		mysql_free_result(result);//�ͷŽ����Դ
	}
	return true;
}

//����ͼ��
bool DBUtil::ExtendBorrowRecord(int nRecordId)
{
	std::string sql = "";
	char szRecordId[16];
	char szBookId[16];
	sprintf_s(szRecordId, "%d", nRecordId);
	time_t tTemp = time(NULL);
	if (isOpen)
	{
		sql = sql + "update borrowrecord set shouldreturndate='" + timeUtil.AddMonth(tTemp) + "',borrowrecord.continue=borrowrecord.continue+1 where id=" + szRecordId;
		mysql_query(&myCont, sql.c_str());
	}
	else
	{
		std::cout << "connect failed!" << std::endl;
	}
	return true;
}

//����ID��ѯ���ļ�¼
bool DBUtil::SelectBorrowRecordByRecordId(BorrowRecord &borrowRecord, int nRecordId)
{
	int res;
	std::string sql;
	char szRecordId[8] = { 0 };
	sprintf_s(szRecordId, "%d", nRecordId);
	if (isOpen)
	{
		sql = sql + "select * from borrowrecord where id=" + szRecordId;
		res = mysql_query(&myCont, sql.c_str());//��ѯ
		if (!res)
		{
			result = mysql_store_result(&myCont);//�����ѯ�������ݵ�result
			if (result)
			{
				sql_row = mysql_fetch_row(result);
				borrowRecord.m_nBorrowId = atoi(sql_row[0]);
				borrowRecord.m_nBookId = atoi(sql_row[1]);
				borrowRecord.m_nUserId = atoi(sql_row[2]);
				borrowRecord.m_tBorrowDate = sql_row[3];
				borrowRecord.m_tShouldReturnDate = sql_row[4];
				borrowRecord.m_tReturnDate = (sql_row[5] == NULL ? "" : sql_row[5]);
				borrowRecord.m_nContinue = atoi(sql_row[6]);
				
			}
		}
		else
		{
			std::cout << "query sql failed!" << std::endl;
		}
	}
	else
	{
		std::cout << "connect failed!" << std::endl;
	}
	if (result != NULL)
	{
		mysql_free_result(result);//�ͷŽ����Դ
	}

	return true;
}