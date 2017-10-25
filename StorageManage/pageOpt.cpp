#include "pageOpt.h"
#include "storage.h"


PageOpt::PageOpt()
{
}


PageOpt::~PageOpt()
{
}


/**
* @brief 读取一个unsigned long类型的某个位
*
* @param [in] num  : unsigned long *    要读取的unsigned long
* @param [in] pos : int                 要读取的unsigned long的位置，该unsigned long的最高位为第1位
* @return  int
* @retval  返回值为0或1
*
**/
int PageOpt::getBit(unsigned long num, long pos) {
	unsigned long result = 0x00000001;
	result = result << (32 - pos);
	result = result & num;
	if (result == 0) {
		return 0;
	}
	else {
		return 1;
	}
}

/**
* @brief 设定一个unsigned long类型的某个位
*
* @param [in] num  : unsigned long *    要修改的unsigned long
* @param [in] pos : int                 要修改的unsigned long的位置，该unsigned long的最高位为第1位
* @param [in] setValue : int            要设定的值，只能为0或1
* @return  int
*
**/
int PageOpt::setBit(unsigned long *num, long pos, int setValue){
	unsigned long result;

	if (setValue != 1 && setValue != 0) {
		return -1;
	}
	if (setValue == getBit(*num, pos)) {
		return 0;
	}
	result = 0x00000001;
	result = result << (32 - pos);

	if (setValue == 0) {
		result = ~result;
		*num = *num & result;
	}
	else {
		*num = *num + result;
	}
	return 0;
}

// 分配页
long PageOpt::allocatePage(struct DbMetaHead *head, long requestPageNum){
	if (head->desc.pageFree == 0){
		printf("分配页空间失败！当前该数据库暂无可用空闲页\n");
		exit(0);
	}
	long page, pos;
	int count = 0;
	long i, j;
	long alloStaPage;
	long pageSum = head->desc.pageSum;

	for (i = 0; i < pageSum; i++) {
		page = i / (8 * sizeof(long));
		pos = i - 8 * sizeof(long) * page + 1;
		count = 0;
		if (getBit(*(head->FreeSpace_bitMap + page), pos) == PAGE_FREE) {
			for (j = i; count < requestPageNum && j < pageSum; j++) {
				page = j / (8 * sizeof(long));
				pos = j - 8 * sizeof(long) * page + 1;
				if (getBit(*(head->FreeSpace_bitMap + page), pos) == PAGE_FREE)
					count++;
				else
					break;
			}
		}
		if (count == requestPageNum)
			break;
		else
			i = i + count;
	}
	if (count != requestPageNum) {
		return ALLO_FAIL;
	}
	else {
		alloStaPage = i;
		for (j = 0; j < requestPageNum; j++) {
			page = (i + j) / (8 * sizeof(long));
			pos = (i + j) - 8 * sizeof(long) * page + 1;
			setBit(head->FreeSpace_bitMap + page, pos, PAGE_UNFREE);
		}
		(head->desc).pageFree -= requestPageNum;
	}

	return alloStaPage;
	return 0;
}

//回收一页
void PageOpt::recyOnePage(struct DbMetaHead *head, long pageNo)
{
	int page, pos;

	page = pageNo / (sizeof(long) * 8);
	pos = pageNo - page * 8 * sizeof(long) + 1;
	setBit(head->FreeSpace_bitMap + page, pos, PAGE_FREE);
}

void PageOpt::recyAllPage(struct DbMetaHead *head) {
	for (long i = 0; i < head->desc.pageSum; i++) {
		recyOnePage(head, i);
	}
	head->desc.pageFree = head->desc.pageSum;
}
