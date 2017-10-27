#include "common.h"


common::common()
{
}


common::~common()
{
}


char *GetDate()
{
	time_t tt = time(NULL);
	tm* t = localtime(&tt);
	char *time = new char[50];
	sprintf(time, "%d", t->tm_year + 1900);
	char tmp[10];
	sprintf(tmp, "/%d", t->tm_mon + 1);
	strcat(time, tmp);
	sprintf(tmp, "/%d", t->tm_mday);
	strcat(time, tmp);
	sprintf(tmp, " %d", t->tm_min);
	strcat(time, tmp);
	sprintf(tmp, ":%d", t->tm_sec);
	strcat(time, tmp);
	sprintf(tmp, ":%d", t->tm_sec);
	strcat(time, tmp);
	return time;
}