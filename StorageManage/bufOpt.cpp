#include "bufOpt.h"
#include "storage.h"


BufOpt::BufOpt()
{
}


BufOpt::~BufOpt()
{
}


//��ѯһҳ�Ƿ��ڻ������У����ڣ��򷵻����ڻ������е��±�
int BufOpt::queryPage(struct DbMetaHead *head, long queryPageNo){
	for (int i = 0; i < BUFFER_Size; i++) {
		if ((head->buff).map[i].pageNo == queryPageNo) {
			return i;
		}
	}
	return BUFF_NOT_HIT;
}

//���±�ΪmapNo�Ļ������� �滻Ϊ ҳ��ΪpageNo��ҳ
int BufOpt::replacePage(struct DbMetaHead *head, int mapNo, long pageNo){
	//�û��������ѱ��༭����Ҫд�ش�����
	if ((head->buff).map[mapNo].isEdited == true) {
		rewind(head->dataPath);
		fseek(head->dataPath, head->desc.dataAddStart + (head->buff).map[mapNo].pageNo * PAGE_SIZE, SEEK_SET);
		fwrite(head->buff.data[mapNo], sizeof(char), PAGE_SIZE, head->dataPath);
	}
	rewind(head->dataPath);
	fseek(head->dataPath, head->desc.dataAddStart + pageNo * PAGE_SIZE, SEEK_SET);
	fread(head->buff.data[mapNo], sizeof(char), PAGE_SIZE, head->dataPath);
	head->buff.map[mapNo].isEdited = false;
	head->buff.map[mapNo].pageNo = pageNo;
	return 0;
}

//�����㷨���滻�����û��ʹ�õĻ������飬�����±�� (������δʹ���㷨 LRU)
int BufOpt::schedulingBuffer(struct DbMetaHead *head){
	int min = 0;
	for (int i = 0; i < BUFFER_Size; i++){
		//�û���������У���ֱ�ӷ���
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


//�����дһ��ҳ������ҳ���ڻ������У�������滻�㷨�Ѹ�ҳ�����������У����ظ�ҳ�ڻ������е��±�
int BufOpt::requestPage(struct DbMetaHead *head, long queryPageNo){
	int mapNo = queryPage(head, queryPageNo);
	//�����ڻ������ڣ�����ȣ���һ���滻
	if (mapNo == BUFF_NOT_HIT) {
		mapNo = schedulingBuffer(head);
		replacePage(head, mapNo, queryPageNo);
	}
	head->buff.curTimeStamp++;
	head->buff.map[mapNo].visitTime = head->buff.curTimeStamp;

	if ((1 << 30) == head->buff.curTimeStamp) {
		for (int i = 0; i < BUFFER_Size; i++) {
			head->buff.map[i].visitTime = 0;
			head->buff.curTimeStamp = 0;
		}
	}
	return mapNo;
}