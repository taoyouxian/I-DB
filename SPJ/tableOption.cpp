#include "dbHead.h"

//�������������������ֵ��е��±�
int createTable_employee(struct dbSysHead *head) {
	int fid = createFile(head, USER_FILE, 1);
	if (fid < 0){
		printf("�����û��ļ�ʧ�ܣ�");
		return  -1;
	}
	else
		printf("�����û��ļ�%d�ɹ���\n\n", fid);

	if (fid < 0) {
		printf("���ݿ��в������ļ���Ϊ%d���ļ���\n", fid);
		return -1;
	}
	//�������ֵ�����һ����λ
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("��ǰ���ݿ����Ѵ��ڹ���Ĺ�ϵ���޷������¹�ϵ��\n");
		return  -1;
	}
	/* todo:

	char* table_pattern;
	printf("��ϵͳ�Ļ����﷨�ɲο�SQL��\n");
	printf("�Դ���������Ϊ����\n");
	printf("create table employee(rid INT, name VARCHAR(20), age INT, PRIMARY KEY(rid));\n");
	printf("������ʾ������һ����Ϊemployee�ı�\n");
	printf("�����������ֶΣ��ֱ���int���͵�rid��age���Լ�varchar���͵�name\n");
	printf("����ridΪ�ñ������\n");
	printf("���棬�봴�����ı�");
	scanf("%s", table_pattern);

	* дһ��parser����table_pattern��create��䣩
	* �õ���relationName,attributeName��attributeType
	* ����attributeName��attributeType�������Attribute�����������������Ա�
	* �ٹ���Relation����
	* Ŀǰ���ֶ�����relation
	*/
	head->data_dict[dictID].initRelation(head, fid, "employee");
	head->data_dict[dictID].insertAttribute("rid", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("did", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("age", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("rname", CHAR_TYPE, 20);

	return dictID;
}

//�������������������ֵ��е��±�
int createTable_department(struct dbSysHead *head) {
	int fid = createFile(head, USER_FILE, 1);
	if (fid < 0){
		printf("�����û��ļ�ʧ�ܣ�");
		return  -1;
	}
	else
		printf("�����û��ļ�%d�ɹ���\n\n", fid);

	if (fid < 0) {
		printf("���ݿ��в������ļ���Ϊ%d���ļ���\n", fid);
		return -1;
	}
	//�������ֵ�����һ����λ
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("��ǰ���ݿ����Ѵ��ڹ���Ĺ�ϵ���޷������¹�ϵ��\n");
		return  -1;
	}

	head->data_dict[dictID].initRelation(head, fid, "department");
	head->data_dict[dictID].insertAttribute("did", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("manager_id", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("num", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("dname", CHAR_TYPE, 20);

	return dictID;
}