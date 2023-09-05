#include "TimeUtil.h"
#include<iostream>

TimeUtil::TimeUtil()
{
}


TimeUtil::~TimeUtil()
{
}

void TimeUtil::TimeToString(time_t time1, char *szTime)
{
	//struct tm *tm1;
	//tm1 = localtime_s(&time1);
	struct tm ptrStructTime;
	localtime_s(&ptrStructTime, &time1);


	struct tm*tm1 = &ptrStructTime;
	sprintf_s(szTime, 50, "%4.4d-%2.2d-%2.2d",
		tm1->tm_year + 1900, tm1->tm_mon + 1, tm1->tm_mday);

}


//ָ��YYYYMMDDHH24MISS�͵�ʱ�䣬��ʽ��Ϊtime_t�͵�ʱ��  
time_t TimeUtil::StringToTime(char * szTime)
{
 	struct tm tm1;
 	time_t time1 = NULL;

	sscanf_s(szTime, "%4d%2d%2d%2d%2d%2d",
		&tm1.tm_year,
		&tm1.tm_mon,
		&tm1.tm_mday,
		&tm1.tm_hour,
		&tm1.tm_min,
		&tm1.tm_sec);
	tm1.tm_year -= 1900;
	tm1.tm_mon--;
	tm1.tm_isdst = -1;
	time1 = mktime(&tm1);
	return time1;
}

std::string TimeUtil::AddMonth(time_t &tTime)
{

	struct tm ptrStructTime ;

	localtime_s(&ptrStructTime, &tTime);

	
	struct tm *structTime=&ptrStructTime;
	//struct tm* ptrStructTime = &structTime;
	//structTime = localtime_s(&tTime);
	switch (structTime->tm_mon + 1)
	{
	case 1:
		if (structTime->tm_mday == 29)
		{
			//�ж��Ƿ�������
			if ((structTime->tm_year + 1900) / 4 == 0 && (structTime->tm_year + 1900) / 100 != 0 || (structTime->tm_year + 1900) / 400 == 0)
			{
				structTime->tm_mon ++;
				break;
			}
			else
			{
				structTime->tm_mon ++;
				structTime->tm_mday = 28;
			}
		}
		else if (structTime->tm_mday > 29)
		{
			structTime->tm_mon++;
			structTime->tm_mday = 28;
		}
		else
		{
			structTime->tm_mon++;
		}
		break;
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
		if (structTime->tm_mday == 31)
		{
			structTime->tm_mon ++;
			structTime->tm_mday = 30;
		}
		else
		{
			structTime->tm_mon ++;
		}
		break;
	case 2:
	case 4:
	case 6:
	case 9:
	case 11:
		structTime->tm_mon ++;
		break;
	case 12:
		structTime->tm_year ++;
		structTime->tm_mon = 1;
		break;
	default:
		break;
	}
	char szTime[16] = {0};
	sprintf_s(szTime, "%4.4d-%2.2d-%2.2d",
		structTime->tm_year + 1900, structTime->tm_mon + 1, structTime->tm_mday);
	return szTime;
}