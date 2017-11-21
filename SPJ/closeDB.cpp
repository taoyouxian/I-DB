#include "dbHead.h"

//���£����ڴ��е���������д�뵽������
int sysUpdate(struct dbSysHead *head){
	//��ģʽ��Ϣ��д�ش��̣�ԭ���ݶ����ǣ�
	int fid = head->desc.fid_DataDictionary;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	struct pageHead ph;
	fseek(head->fpdesc, head->desc.addr_data + SIZE_PER_PAGE * pageNo, SEEK_SET);
	fread(&ph, SIZE_PAGEHEAD, 1, head->fpdesc);
	ph.curRecordNum = 0;
	ph.freeSpace = SIZE_PER_PAGE - SIZE_PAGEHEAD;

	for (int i = 0; i < MAX_FILE_NUM; i++) {
		int fid_ = head->data_dict[i].fileID;
		if (fid_ < 0 || head->desc.fileDesc[fid_].fileType < 0){
			continue;
		}
		//ɾ����ʱ���ļ�����д��
		if (head->desc.fileDesc[fid_].fileType == TMP_TABLE){
			deleteFile(head, fid_);
			continue;
		}
		if (ph.freeSpace < sizeof(Relation)){
			if (ph.nextPageNo < 0)
				ph = extendFile(head, head->desc.fid_DataDictionary, &ph);
			else {
				pageNo = ph.nextPageNo;
				fseek(head->fpdesc, head->desc.addr_data + SIZE_PER_PAGE * pageNo, SEEK_SET);
				fread(&ph, SIZE_PAGEHEAD, 1, head->fpdesc);
			}
		}
		//�����¼�¼����ģʽ��
		fseek(head->fpdesc, head->desc.addr_data + SIZE_PER_PAGE * ph.pageNo + SIZE_PAGEHEAD + sizeof(Relation) * ph.curRecordNum, SEEK_SET);
		fwrite(&head->data_dict[i], sizeof(Relation), 1, head->fpdesc);

		ph.curRecordNum++;
		ph.freeSpace -= sizeof(Relation);
	}
	//����ҳͷ
	fseek(head->fpdesc, head->desc.addr_data + SIZE_PER_PAGE * ph.pageNo, SEEK_SET);
	fwrite(&ph, SIZE_PAGEHEAD, 1, head->fpdesc);


	//�ײ���Ϣ��д��
	rewind(head->fpdesc);
	fwrite(&(head->desc), sizeof(struct SysDesc), 1, head->fpdesc);

	//���пռ�λʾͼ��д��
	rewind(head->fpdesc);
	fseek(head->fpdesc, head->desc.addr_FreeSpace_BitMap, SEEK_SET);
	fwrite(head->FreeSpace_bitMap, sizeof(char), head->desc.size_FreeSpace_BitMap, head->fpdesc);

	//��������ɾ����־λΪ1���߼���
	recyAllFreeLogicID(head);

	//�߼���λʾͼ��д��
	fseek(head->fpdesc, head->desc.addr_FreeLogicID_BitMap, SEEK_SET);
	fwrite(head->FreeLogicID_bitMap, sizeof(char), head->desc.size_FreeLogicID_BitMap, head->fpdesc);

	return 0;
}
void deleteFile(struct dbSysHead *head, int fid){
	if (fid >= 0) {
		long pageNum = head->desc.fileDesc[fid].filePageNum;
		long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
		long pageAddr = head->desc.addr_data + pageNo * SIZE_PER_PAGE;
		long nextPage = -1;

		for (long i = 0; i < pageNum; i++) {
			struct pageHead ph;
			rewind(head->fpdesc);
			fseek(head->fpdesc, pageAddr, SEEK_SET);
			fread(&ph, SIZE_PAGEHEAD, 1, head->fpdesc);

			nextPage = ph.nextPageNo;
			recyOnePage(head, ph.pageNo);

			if (nextPage < 0)
				break;
			else
				pageAddr = head->desc.addr_data + nextPage * SIZE_PER_PAGE;
		}

		head->desc.AvaiPage += pageNum;
		head->desc.curFileNum--;
		head->desc.fileDesc[fid].fileType = -1;
		//        head->desc.fileDesc[fx].fileID = -1;
		head->desc.fileDesc[fid].fileFirstPageNo = -1;
		head->desc.fileDesc[fid].filePageNum = -1;
	}
	else{
		printf("���ݿ��в������ļ���Ϊ%d���ļ���\n", fid);
	}
}
void deleteDataDict(struct dbSysHead *head, int fid) {
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID == fid){
			memset(&head->data_dict[i], 0, sizeof(Relation));
			head->data_dict[i].fileID = -1;
		}
	}
}
void close_database(struct dbSysHead* head){
	//���Ȱѻ����������������д�ش���
	for (int i = 0; i < SIZE_BUFF; i++) {
		if (head->buff.map[i].isEdited == true) {
			rewind(head->fpdesc);
			fseek(head->fpdesc, head->desc.addr_data + SIZE_PER_PAGE * head->buff.map[i].pageNo, SEEK_SET);
			fwrite(head->buff.data[i], sizeof(char), SIZE_PER_PAGE, head->fpdesc);
			head->buff.map[i].isEdited = false;
		}
	}

	show_SysDesc(head);
	sysUpdate(head);
	show_SysDesc(head);
	fclose(head->fpdesc);
	free(head->FreeSpace_bitMap);
	free(head->FreeLogicID_bitMap);
	exitdb();
}