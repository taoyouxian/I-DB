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
long PageOpt::allocatePage(struct dbSysHead *head, long reqPageNum){
	if (head->desc.pageAvai == 0){
		printf("分配页空间失败！当前该数据库暂无可用空闲页\n");
		exit(0);
	}
	long page, pos;
	int count = 0;
	long i, j;
	long alloStaPage;
	long totalPage = head->desc.totalPage;

	for (i = 0; i < totalPage; i++) {
		page = i / (8 * sizeof(long));
		pos = i - 8 * sizeof(long) * page + 1;
		count = 0;
		if (getBit(*(head->FreeSpace_bitMap + page), pos) == PAGE_AVAI) {
			for (j = i; count < reqPageNum && j < totalPage; j++) {
				page = j / (8 * sizeof(long));
				pos = j - 8 * sizeof(long) * page + 1;
				if (getBit(*(head->FreeSpace_bitMap + page), pos) == PAGE_AVAI)
					count++;
				else
					break;
			}
		}
		if (count == reqPageNum)
			break;
		else
			i = i + count;
	}
	if (count != reqPageNum) {
		return ALLO_FAIL;
	}
	else {
		alloStaPage = i;
		for (j = 0; j < reqPageNum; j++) {
			page = (i + j) / (8 * sizeof(long));
			pos = (i + j) - 8 * sizeof(long) * page + 1;
			setBit(head->FreeSpace_bitMap + page, pos, PAGE_UNAVAI);
		}
		(head->desc).pageAvai -= reqPageNum;
	}

	return alloStaPage;
	return 0;
}

//回收一页
void PageOpt::recyOnePage(struct dbSysHead *head, long pageNo)
{
	int page, pos;

	page = pageNo / (sizeof(long) * 8);
	pos = pageNo - page * 8 * sizeof(long) + 1;
	setBit(head->FreeSpace_bitMap + page, pos, PAGE_AVAI);
}

void PageOpt::recyAllPage(struct dbSysHead *head) {
	for (long i = 0; i < head->desc.totalPage; i++) {
		recyOnePage(head, i);
	}
	head->desc.pageAvai = head->desc.totalPage;
}
