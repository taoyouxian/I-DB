#pragma once

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <iostream>
#include <cstdio>
#include <vector> 
#include <fstream>
using namespace std; 

// 常见配置参数
#define MAX_FILE_NUM 64

#define SIZE_BUFF 1000
#define SIZE_PER_PAGE (4*1024)
#define SIZE_DATA_SPACE (196*1024*1024)
#define SIZE_BIT_MAP  (SIZE_DATA_SPACE/(SIZE_PER_PAGE*8))

#define ADDR_BITMAP 1024
#define ADDR_DATA (ADDR_BITMAP+SIZE_BIT_MAP)

#define MAP_FILE         0
#define NORMAL_FILE      1
#define INDEX_FILE_HASH  2
#define INDEX_FILE_BTREE 3
#define INDEX_FILE_SEQ   4

#define ALLO_FAIL  -1
#define PAGE_AVAI   1
#define PAGE_UNAVAI 0

#define BUFF_NOT_HIT -1

class common
{
public:
	common();
	~common();
};


#endif // COMMON_H_INCLUDED