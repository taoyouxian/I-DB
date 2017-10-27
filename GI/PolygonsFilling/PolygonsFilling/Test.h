#pragma once

#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

struct Student{
	long rid;
	char name[100];
	int age;
	int weight;
};

struct supplier{
	long S_superkey;
	char S_name[30];
	char S_address[40];
	long S_nationkey;
	char S_phone[20];
	float S_acctbal;
	char S_comment[110];
};

#endif // TEST_H_INCLUDED