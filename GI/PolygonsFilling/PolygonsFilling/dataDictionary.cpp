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
		printf("数据库中不存在文件号为%d的文件！\n", fid);
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
		printf("该表中的属性个数已经达到最大值！无法再增加新的属性\n");
		return -1;
	}
	//是属性表中的第一个属性，偏移为0
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
	printf("在关系%s中，属性%s不存在！\n", this->relationName, name);
	exit(0);    //按理说不应该直接退出，而是有返回值
}

int Relation::getAttributeIndexByName(char *name){
	for (int i = 0; i < this->attributeNum; i++){
		if (strcmp(this->atb[i].getName(), name) == 0){
			return i;
		}
	}
	return -1;
}