#include "bufOpt.h"
#include "storage.h"


BufOpt::BufOpt()
{
}


BufOpt::~BufOpt()
{
}


//查询一页是否在缓冲区中，若在，则返回它在缓冲区中的下标
int BufOpt::queryPage(struct dbSysHead *head, long queryPageNo){
	for (int i = 0; i < SIZE_BUFF; i++) {
		if ((head->buff).map[i].pageNo == queryPageNo) {
			return i;
		}
	}
	return BUFF_NOT_HIT;
}

//将下标为mapNo的缓冲区块 替换为 页号为pageNo的页
int BufOpt::replacePage(struct dbSysHead *head, int mapNo, long pageNo){
	//该缓冲区块已被编辑，需要写回磁盘中
	if ((head->buff).map[mapNo].isEdited == true) {
		rewind(head->fpdesc);
		fseek(head->fpdesc, head->desc.dataAddr + (head->buff).map[mapNo].pageNo * SIZE_PER_PAGE, SEEK_SET);
		fwrite(head->buff.data[mapNo], sizeof(char), SIZE_PER_PAGE, head->fpdesc);
	}
	rewind(head->fpdesc);
	fseek(head->fpdesc, head->desc.dataAddr + pageNo * SIZE_PER_PAGE, SEEK_SET);
	fread(head->buff.data[mapNo], sizeof(char), SIZE_PER_PAGE, head->fpdesc);
	head->buff.map[mapNo].isEdited = false;
	head->buff.map[mapNo].pageNo = pageNo;
	return 0;
}

//调度算法，替换出最久没有使用的缓冲区块，返回下标号
int BufOpt::scheBuff(struct dbSysHead *head){
	int min = 0;
	for (int i = 0; i < SIZE_BUFF; i++){
		//该缓冲区块空闲，可直接分配
		if (head->buff.map[i].pageNo < 0) {
			return i;
		}
		else {
			if (head->buff.map[i].visitTime < head->buff.map[min].visitTime) {
				min = i;
			}
		}
	}
	return min;
}


//请求读写一个页，若该页不在缓冲区中，则调用替换算法把该页调到缓冲区中，返回该页在缓冲区中的下标
int BufOpt::reqPage(struct dbSysHead *head, long queryPageNo){
	int mapNo = queryPage(head, queryPageNo);
	//若不在缓冲区内，则调度，找一块替换
	if (mapNo == BUFF_NOT_HIT) {
		mapNo = scheBuff(head);
		replacePage(head, mapNo, queryPageNo);
	}
	head->buff.curTimeStamp++;
	head->buff.map[mapNo].visitTime = head->buff.curTimeStamp;

	//???
	if ((1 << 30) == head->buff.curTimeStamp) {
		for (int i = 0; i < SIZE_BUFF; i++) {
			head->buff.map[i].visitTime = 0;
			head->buff.curTimeStamp = 0;
		}
	}
	return mapNo;
}