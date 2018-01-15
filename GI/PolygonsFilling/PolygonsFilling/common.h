#pragma once

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <iostream>
#include <cstdio>
#include <time.h>
#include <vector> 
#include <fstream>
using namespace std; 

// 常见配置参数
#define MAX_FILE_NUM 1 // 64

#define SEG_NUM 4

#define BUFFER_Size 1000
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


class common
{
public:
	common();
	~common();
};

/**
* 用于数据库地址表示
*/
typedef long long Addr;  // printf lld
/**
* 用于页内偏移地址表示
*/
typedef short PageAddr;  // printf d
typedef unsigned long long uint64; // %llu
typedef unsigned int uint; // %u
typedef unsigned short ushort; // %u
/**
* 用于构成数据字节
*/
typedef unsigned char Byte; // %u
/**
* 枚举类，表示BufferManager的缓冲策略。
*/
enum StrategyInBuffer
{
	NONE, FIFO, ClOCK, LRU, LFU
};

#endif // COMMON_H_INCLUDED