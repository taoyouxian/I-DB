#pragma once

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <iostream>
#include <cstdio>
#include <vector> 
#include <fstream>
#include <time.h>
#include <stdlib.h>
using namespace std; 

// 常见配置参数
#define MAX_FILE_NUM 1 // 64

#define SEG_NUM 4

#define BUFFER_SIZE 1000
#define PAGE_SIZE (4 * 1024)
#define DATA_SPACE_SIZE (196*1024*1024)
#define BIT_MAP_SIZE  (DATA_SPACE_SIZE / (PAGE_SIZE * 8))

#define ADDR_BITMAP 1024
#define ADDR_DATA (ADDR_BITMAP + BIT_MAP_SIZE)

#define MAP_FILE         0
#define NORMAL_FILE      1
#define INDEX_FILE_HASH  2
#define INDEX_FILE_BTREE 3
#define INDEX_FILE_SEQ   4

#define ALLO_FAIL  -1
#define PAGE_FREE   1
#define PAGE_UNFREE 0

#define BUFF_NOT_HIT -1
 
char *GetDate();

class common
{
public:
	common();
	~common();
};


#endif // COMMON_H_INCLUDED