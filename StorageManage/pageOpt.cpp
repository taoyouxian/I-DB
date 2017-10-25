#include "pageOpt.h"
#include "storage.h"


PageOpt::PageOpt()
{
}


PageOpt::~PageOpt()
{
}


/**
* @brief ��ȡһ��unsigned long���͵�ĳ��λ
*
* @param [in] num  : unsigned long *    Ҫ��ȡ��unsigned long
* @param [in] pos : int                 Ҫ��ȡ��unsigned long��λ�ã���unsigned long�����λΪ��1λ
* @return  int
* @retval  ����ֵΪ0��1
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
* @brief �趨һ��unsigned long���͵�ĳ��λ
*
* @param [in] num  : unsigned long *    Ҫ�޸ĵ�unsigned long
* @param [in] pos : int                 Ҫ�޸ĵ�unsigned long��λ�ã���unsigned long�����λΪ��1λ
* @param [in] setValue : int            Ҫ�趨��ֵ��ֻ��Ϊ0��1
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

// ����ҳ
long PageOpt::allocatePage(struct DbMetaHead *head, long requestPageNum){
	if (head->desc.pageFree == 0){
		printf("����ҳ�ռ�ʧ�ܣ���ǰ�����ݿ����޿��ÿ���ҳ\n");
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

//����һҳ
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
