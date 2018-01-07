#include "storage.h"

int Attribute::initAttribute(char *name, int type, int length, int deviation) {
	strcpy(this->attributeName, name);
	this->type = type;
	this->length = length;
	this->recordDeviation = deviation;
	return 0;
}

char * Attribute::getName() {
	return this->attributeName;
}

int Attribute::getLength() {
	return this->length;
}

int Attribute::getType() {
	return this->type;
}

int Attribute::getRecordDeviation() {
	return this->recordDeviation;
}

bool Attribute::isSameAttribute(Attribute a){
	if (strcmp(this->getName(), a.getName()) == 0 && this->getType() == a.getType() && this->getLength() == a.getLength())
		return true;
	else
		return false;
}
/*************************************/

int Relation::initRelation(struct dbSysHead *head, int fid, const char *relationName){
	if (fid < 0) {
		printf("���ݿ��в������ļ���Ϊ%d���ļ���\n", fid);
		return -1;
	}
	this->fileID = fid;
	strcpy(this->relationName, relationName);

	this->attributeNum = 0;
	this->recordNum = 0;
	return 0;
}

int Relation::insertAttribute(char *name, int type, int length){
	if (this->attributeNum >= ATTRIBUTE_NUM) {
		printf("�ñ��е����Ը����Ѿ��ﵽ���ֵ���޷��������µ�����\n");
		return -1;
	}
	//�����Ա��еĵ�һ�����ԣ�ƫ��Ϊ0
	if (this->attributeNum == 0){
		this->atb[this->attributeNum].initAttribute(name, type, length, 0);
	}
	else {
		int deviation = this->atb[this->attributeNum - 1].getLength() + this->atb[this->attributeNum - 1].getRecordDeviation();
		this->atb[this->attributeNum].initAttribute(name, type, length, deviation);
	}
	this->isIndexed[this->attributeNum] = false;
	this->isOrdered[this->attributeNum] = false;
	this->attributeNum++;
	this->recordLength += length;
	return 0;
}

Attribute Relation::getAttributeByName(char *name){
	for (int i = 0; i < this->attributeNum; i++){
		if (strcmp(this->atb[i].getName(), name) == 0){
			return this->atb[i];
		}
	}
	printf("�ڹ�ϵ%s�У�����%s�����ڣ�\n", this->relationName, name);
	exit(0);    //����˵��Ӧ��ֱ���˳��������з���ֵ
}

int Relation::getAttributeIndexByName(char *name){
	for (int i = 0; i < this->attributeNum; i++){
		if (strcmp(this->atb[i].getName(), name) == 0){
			return i;
		}
	}
	return -1;
}