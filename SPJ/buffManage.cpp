#include "dbHead.h"

//��ѯһҳ�Ƿ��ڻ������У����ڣ��򷵻����ڻ������е��±�
int queryPage(struct dbSysHead *head, long queryPageNo){
	for (int i = 0; i < SIZE_BUFF; i++) {
		if ((head->buff).map[i].pageNo == queryPageNo) {
			return i;
		}
	}
	return BUFF_NOT_HIT;
}

//���±�ΪmapNo�Ļ������� �滻Ϊ ҳ��ΪpageNo��ҳ
int replacePage(struct dbSysHead *head, int mapNo, long pageNo){
	//�û��������ѱ��༭����Ҫд�ش�����
	if ((head->buff).map[mapNo].isEdited == true) {
		rewind(head->fpdesc);
		fseek(head->fpdesc, head->desc.addr_data + (head->buff).map[mapNo].pageNo * SIZE_PER_PAGE, SEEK_SET);
		fwrite(head->buff.data[mapNo], sizeof(char), SIZE_PER_PAGE, head->fpdesc);
	}
	rewind(head->fpdesc);
	fseek(head->fpdesc, head->desc.addr_data + pageNo * SIZE_PER_PAGE, SEEK_SET);
	fread(head->buff.data[mapNo], sizeof(char), SIZE_PER_PAGE, head->fpdesc);
	head->buff.map[mapNo].isEdited = false;
	head->buff.map[mapNo].pageNo = pageNo;
	return 0;
}

//�����㷨���滻�����û��ʹ�õĻ������飬�����±��
int scheBuff(struct dbSysHead *head){
	int min = 0;
	for (int i = 0; i < SIZE_BUFF; i++){
		//�û���������У���ֱ�ӷ���
		if (head->buff.map[i].pageNo < 0) {
			return i;
		}
		else {
			//Ѱ�����δ��ʹ�ã���δ��pinס�Ŀ�
			if (head->buff.map[i].visitTime < head->buff.map[min].visitTime
				&& head->buff.map[i].isPin == false) {
				min = i;
			}
		}
	}
	return min;
}


//�����дһ��ҳ������ҳ���ڻ������У�������滻�㷨�Ѹ�ҳ�����������У����ظ�ҳ�ڻ������е��±�
int reqPage(struct dbSysHead *head, long queryPageNo){
	int mapNo = queryPage(head, queryPageNo);
	//�����ڻ������ڣ�����ȣ���һ���滻
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