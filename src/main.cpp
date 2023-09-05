#include <conio.h>
#include <limits>
#include "Manager.h"
#include "DBUtil.h"
#include "Student.h"
#include "Book.h"

User login();

int main()
{
	while (1) {
		User user = login();
		bool bIsLogin = true;
		if (user.m_nRole == 1)
		{
			Manager manager;
			manager.m_nID = user.m_nRole;
			manager.m_strName = user.m_strName;
			manager.m_nRole = user.m_nRole;
			int command = -1;		//选择命令
			bIsLogin = true;
			std::string strBookName = "";
			int nBookId = 0;
			while (bIsLogin)
			{
				system("cls");
				manager.ShowMenu();
				std::cout << "请选择命令：";
				std::string _command;
				std::cin >> _command;
				if (_command.size() == 1 && _command[0] < '8' && _command[0] >= '0') {
					command = _command[0] - '0';
					switch (command)
					{
					case 1:
						//选择新增图书
						manager.AddBook();
						break;
					case 2:
						manager.QueryBook();
						break;
					case 3:
						manager.DeleteBook();
						break;
					case 4:
						manager.DisplayAllBook();
						break;
					case 5:
						manager.DiaplayAllBorrowRecord();
						break;
					case 6:
						manager.AddUser();
						break;
					case 7:
						manager.DisplayAllUser();
						break;
					case 0:
						//选择退出登录
						bIsLogin = false;
						break;
					}
				}
				else {
					std::cout << "输入有误,请重新输入";
					std::cin.get();
					std::cin.get();
				}		
			}
			std::cout << "您已退出登录，按任意键退出...";
			std::cin.get();
			std::cin.get();
			system("cls");
		}
		else
		{
			Student student;
			student.m_nID = user.m_nID;
			student.m_strName = user.m_strName;
			student.m_nRole = user.m_nRole;
			int command = -1;		//选择命令
			bIsLogin = true;
			std::string strBookName = "";
			int nBookId;
			while (bIsLogin)
			{
				system("cls");
				student.ShowMenu();
				std::cout << "请选择命令：";
				std::string _command;
				std::cin >> _command;
				if (_command.size() == 1 && _command[0] < '6' && _command[0] >= '0') {
					command = _command[0] - '0';
					switch (command)
					{

					case 1:
						//选择查询图书					
						student.QueryBook();
						break;
					case 2:
						student.BorrowBook();
						break;
					case 3:
						//归还图书
						student.ReturnBook();
						break;
					case 4:
						student.ShowMyBorrowRecord();
						break;
					case 5:
						student.RenewBook();
						break;
					case 0:
						//选择退出登录
						bIsLogin = false;
						break;
					}
				}
				else {
					std::cout << "输入有误,请重新输入";
					std::cin.get();
					std::cin.get();
				}
				
			}
			std::cout << "您已退出登录，按任意键退出...";
			std::cin.get();
			std::cin.get();
			system("cls");
		}
	}
}

User login() {
	std::string strUserName = "";
	std::string strUserPWD = "";
	User user;
	DBUtil dbUtil;
	dbUtil.OpenDB();

	auto getPassword = []() {
		std::string password;
		char input_char;
		while ((input_char = _getch()) != '\r') {
			if (input_char == '\b') {
				if (!password.empty()) {
					putchar('\b');
					putchar(' ');
					putchar('\b');
					password.pop_back();
				}
			}
			else {
				std::cout << '*';
				password += input_char;
			}
		}
		std::cout << std::endl;
		return password;
	};

	auto getUserCredentials = [&strUserName, &strUserPWD, &getPassword]() {
		std::cout << "请输入用户名：";
		getline(std::cin, strUserName);
		std::cout << "请输入密码：";
		strUserPWD = getPassword();
	};

	auto signinCredentials = [&strUserName, &strUserPWD, &dbUtil, &getPassword]() {
		system("cls");
		std::cout << "首次启动系统，请注册！\n";
		std::string checkPWD = "";
		std::cout << "请输入用户名：";
		getline(std::cin, strUserName);
		std::cout << "请输入密码：";
		strUserPWD = getPassword();
		std::cout << "请确认密码：";
		checkPWD = getPassword();
		if (strUserPWD==checkPWD) {
			if (dbUtil.enroll(strUserName, strUserPWD)) {
				std::cout << "注册成功\n";
				std::cin.get();
				return true;
			}
		}
		else {
			std::cout << "注册失败，两次密码不一致！\n";
			std::cin.get();
			return false;
		}
	};

	//检查数据库中是否注册用户
	if (dbUtil.UserEmpty()) {
		while (!signinCredentials());
	}
	system("cls");
	getUserCredentials();
	user = dbUtil.ReadUser(strUserName, strUserPWD);
	while (user.m_nID == -1)
	{
		std::cout << "用户名或者密码错误，请重新输入,按任意键继续..." << std::endl;
		strUserName.clear();
		strUserPWD.clear();
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 清除输入缓冲区
		getUserCredentials();
		user = dbUtil.ReadUser(strUserName, strUserPWD);
	}
	return user;
}