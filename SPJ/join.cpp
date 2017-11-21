#include "dbHead.h"

int nestedLoopJoin(struct dbSysHead *head, int employee_dictID, int department_dictID){
	Relation emp = head->data_dict[employee_dictID];
	Relation dept = head->data_dict[department_dictID];

	//找到要做连接的公共属性(的下标)
	int emp_pub_attr = 0, dept_pub_attr = 0;
	bool isFound = false;
	for (emp_pub_attr = 0; emp_pub_attr < emp.attributeNum; emp_pub_attr++) {
		for (dept_pub_attr = 0; dept_pub_attr < dept.attributeNum; dept_pub_attr++) {
			if (emp.atb[emp_pub_attr].isSameAttribute(dept.atb[dept_pub_attr])){
				isFound = true;
				break;
			}
		}
		if (isFound)
			break;
	}
	if (isFound == false) {
		printf("连接失败！表%s和表%s没有公共属性！\n", emp.relationName, dept.relationName);
		return -1;
	}
	int emp_fid = emp.fileID;
	long emp_pageNo = head->desc.fileDesc[emp_fid].fileFirstPageNo;
	long emp_pageNum = head->desc.fileDesc[emp_fid].filePageNum;

	int dept_fid = dept.fileID;
	long dept_pageNo = head->desc.fileDesc[dept_fid].fileFirstPageNo;
	long dept_pageNum = head->desc.fileDesc[dept_fid].filePageNum;


	int tmp_table_dictID = createTmpTable2(head, emp, dept, emp_pub_attr, dept_pub_attr);
	if (tmp_table_dictID < 0){
		printf("创建临时表失败！\n");
		return -1;
	}
	Relation *tmp = &(head->data_dict[tmp_table_dictID]);

	//外层循环：用小的，暂时假定department小
	int *buffID = (int*)malloc(dept_pageNum);
	int m = SIZE_BUFF - 1;
	int outer = dept_pageNum / m + 1;
	for (int x = 0; x < outer; x++){
		for (int y = 0; y < m && y < dept_pageNum; y++) {
			int mapNo = reqPage(head, dept_pageNo);
			struct pageHead ph;
			memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
			head->buff.map[mapNo].isPin = true; //把dept的这些页都pin在缓冲区中
			buffID[y] = mapNo;
			if (ph.nextPageNo < 0)
				break;
			else
				dept_pageNo = ph.nextPageNo;
		}

		for (int z = 0; z < emp_pageNum; z++) {
			int emp_mapNo = reqPage(head, emp_pageNo);
			struct pageHead emp_ph;
			memcpy(&emp_ph, head->buff.data[emp_mapNo], SIZE_PAGEHEAD);
			for (int k = 0; k < emp_ph.curRecordNum; k++) {
				char *emp_record = (char*)malloc(emp.recordLength);
				getNextRecord(head, emp_mapNo, k, emp_record);
				char *emp_value = (char*)malloc(emp.recordLength);
				int pe = getValueByAttrID(emp_record, emp_pub_attr, emp_value);//得到公共属性上的值
				if (pe < 0){
					printf("bug in join.cpp\n");
					exit(0);
				}
				for (int y = 0; y < m && y < dept_pageNum; y++){
					int dept_mapNo = buffID[y];
					struct pageHead dept_ph;
					memcpy(&dept_ph, head->buff.data[dept_mapNo], SIZE_PAGEHEAD);
					for (int i = 0; i < dept_ph.curRecordNum; i++) {
						char *dept_record = (char*)malloc(dept.recordLength);
						getNextRecord(head, dept_mapNo, i, dept_record);
						char *dept_value = (char*)malloc(dept.recordLength);
						int pd = getValueByAttrID(dept_record, dept_pub_attr, dept_value);//得到公共属性上的值
						if (pd < 0){
							printf("bug in join.cpp\n");
							exit(0);
						}

						if (strcmp(emp_value, dept_value) == 0){
							char *result = (char*)malloc(tmp->recordLength);
							memset(result, 0, tmp->recordLength);
							//连接这两个元组，放入临时表中
							strcpy(result, emp_record);
							strcat(result, "|");
							strncat(result, dept_record, pd);
							strcat(result, dept_record + pd + strlen(dept_value) + 1);
							insertOneRecord(head, tmp_table_dictID, result);
						}
					}
				}

			}
			if (emp_ph.nextPageNo < 0)
				break;
			else
				emp_pageNo = emp_ph.nextPageNo;
		}
	}

	return tmp_table_dictID;
}

int nestedLoopJoinByConds(struct dbSysHead *head, int birthday_dictID, int department_dictID, char* attribute_name, char* value, int like){
	Relation birth = head->data_dict[birthday_dictID];
	Relation dept = head->data_dict[department_dictID];

	//找到要做连接的公共属性(的下标)
	int birth_pub_attr = 0, dept_pub_attr = 0;
	bool isFound = false;
	for (birth_pub_attr = 0; birth_pub_attr < birth.attributeNum; birth_pub_attr++) {
		for (dept_pub_attr = 0; dept_pub_attr < dept.attributeNum; dept_pub_attr++) {
			if (birth.atb[birth_pub_attr].isSameAttribute(dept.atb[dept_pub_attr])){
				isFound = true;
				break;
			}
		}
		if (isFound)
			break;
	}
	if (isFound == false) {
		printf("连接失败！表%s和表%s没有公共属性！\n", birth.relationName, dept.relationName);
		return -1;
	}
	int birth_fid = birth.fileID;
	long birth_pageNo = head->desc.fileDesc[birth_fid].fileFirstPageNo;
	long birth_pageNum = head->desc.fileDesc[birth_fid].filePageNum;

	int dept_fid = dept.fileID;
	long dept_pageNo = head->desc.fileDesc[dept_fid].fileFirstPageNo;
	long dept_pageNum = head->desc.fileDesc[dept_fid].filePageNum;


	int tmp_table_dictID = createTmpTable2(head, birth, dept, birth_pub_attr, dept_pub_attr);
	if (tmp_table_dictID < 0){
		printf("创建临时表失败！\n");
		return -1;
	}
	Relation *tmp = &(head->data_dict[tmp_table_dictID]);

	//外层循环：用小的，暂时假定department小
	int *buffID = (int*)malloc(dept_pageNum);
	int m = SIZE_BUFF - 1;
	int outer = dept_pageNum / m + 1;
	for (int x = 0; x < outer; x++){
		for (int y = 0; y < m && y < dept_pageNum; y++) {
			int mapNo = reqPage(head, dept_pageNo);
			struct pageHead ph;
			memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
			head->buff.map[mapNo].isPin = true; //把dept的这些页都pin在缓冲区中
			buffID[y] = mapNo;
			if (ph.nextPageNo < 0)
				break;
			else
				dept_pageNo = ph.nextPageNo;
		}
		// 查找属性
		int attr_index = birth.getAttributeIndexByName(attribute_name);

		for (int z = 0; z < birth_pageNum; z++) {
			int birth_mapNo = reqPage(head, birth_pageNo);
			struct pageHead birth_ph;
			memcpy(&birth_ph, head->buff.data[birth_mapNo], SIZE_PAGEHEAD);
			for (int k = 0; k < birth_ph.curRecordNum; k++) {
				char *birth_record = (char*)malloc(birth.recordLength);
				getNextRecord(head, birth_mapNo, k, birth_record);
				char *birth_value = (char*)malloc(birth.recordLength);
				int pe = getValueByAttrID(birth_record, birth_pub_attr, birth_value);//得到公共属性上的值
				if (pe < 0){
					printf("bug in join.cpp\n");
					exit(0);
				}

				if (attr_index < 0){
					printf("关系表%s中没有名为%s的属性！\n", birth.relationName, attribute_name);
					return -1;
				}
				else{
					char *val = (char*)malloc(strlen(birth_record));
					memset(val, 0, strlen(birth_record));
					int s = getValueByAttrID(birth_record, attr_index, val);
					// [列名判断](https://zhidao.baidu.com/question/438950092.html)
					bool flag = false;
					if (like == 0){

					}
					else if (like == 1){
						if (strstr(val, value))
							flag = true;
					}
					else if (like == 2){

					}
					else if (like == 3){
						string s = val;
						string::size_type idx = s.find(value);
						if (idx == string::npos)
							flag = true;
					}
					if (flag){
						for (int y = 0; y < m && y < dept_pageNum; y++){
							int dept_mapNo = buffID[y];
							struct pageHead dept_ph;
							memcpy(&dept_ph, head->buff.data[dept_mapNo], SIZE_PAGEHEAD);
							for (int i = 0; i < dept_ph.curRecordNum; i++) {
								char *dept_record = (char*)malloc(dept.recordLength);
								getNextRecord(head, dept_mapNo, i, dept_record);
								char *dept_value = (char*)malloc(dept.recordLength);
								int pd = getValueByAttrID(dept_record, dept_pub_attr, dept_value);//得到公共属性上的值
								if (pd < 0){
									printf("bug in join.cpp\n");
									exit(0);
								}

								if (strcmp(birth_value, dept_value) == 0){
									char *result = (char*)malloc(tmp->recordLength);
									memset(result, 0, tmp->recordLength);
									//连接这两个元组，放入临时表中
									strcpy(result, birth_record);
									strcat(result, "|");
									// bug, 长度-1
									strncat(result, dept_record, pd-1);
									insertOneRecord(head, tmp_table_dictID, result);
								}
							}
						}
					}
				}

			}
			if (birth_ph.nextPageNo < 0)
				break;
			else
				birth_pageNo = birth_ph.nextPageNo;
		}
	}

	return tmp_table_dictID;
}

int nestedLoopJoinByThree(struct dbSysHead *head, int temp_dictID, int department_dictID, int birthday_dictID){
	Relation temp = head->data_dict[temp_dictID]; // 5
	Relation birth = head->data_dict[birthday_dictID];// 10

	//找到要做连接的公共属性(的下标)
	int temp_pub_attr = 0, birth_pub_attr = 0;
	bool isFound = false;
	for (temp_pub_attr = 0; temp_pub_attr < temp.attributeNum; temp_pub_attr++) {
		for (birth_pub_attr = 0; birth_pub_attr < birth.attributeNum; birth_pub_attr++) {
			if (temp.atb[temp_pub_attr].isSameAttribute(birth.atb[birth_pub_attr])){
				isFound = true;
				break;
			}
		}
		if (isFound)
			break;
	}
	if (isFound == false) {
		printf("连接失败！表%s和表%s没有公共属性！\n", temp.relationName, birth.relationName);
		return -1;
	}
	int temp_fid = temp.fileID;
	long temp_pageNo = head->desc.fileDesc[temp_fid].fileFirstPageNo;
	long temp_pageNum = head->desc.fileDesc[temp_fid].filePageNum;

	int birth_fid = birth.fileID;
	long birth_pageNo = head->desc.fileDesc[birth_fid].fileFirstPageNo;
	long birth_pageNum = head->desc.fileDesc[birth_fid].filePageNum;


	int tmp_table_dictID = createTmpTable2(head, temp, birth, temp_pub_attr, birth_pub_attr);
	if (tmp_table_dictID < 0){
		printf("创建临时表失败！\n");
		return -1;
	}
	Relation *tmp = &(head->data_dict[tmp_table_dictID]);

	//外层循环：用小的，暂时假定department小
	int *buffID = (int*)malloc(birth_pageNum);
	int m = SIZE_BUFF - 1;
	int outer = birth_pageNum / m + 1;
	for (int x = 0; x < outer; x++){
		for (int y = 0; y < m && y < birth_pageNum; y++) {
			int mapNo = reqPage(head, birth_pageNo);
			struct pageHead ph;
			memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
			head->buff.map[mapNo].isPin = true; //把birth的这些页都pin在缓冲区中
			buffID[y] = mapNo;
			if (ph.nextPageNo < 0)
				break;
			else
				birth_pageNo = ph.nextPageNo;
		}

		for (int z = 0; z < temp_pageNum; z++) {
			int temp_mapNo = reqPage(head, temp_pageNo);
			struct pageHead temp_ph;
			memcpy(&temp_ph, head->buff.data[temp_mapNo], SIZE_PAGEHEAD);
			for (int k = 0; k < temp_ph.curRecordNum; k++) {
				char *temp_record = (char*)malloc(temp.recordLength);
				getNextRecord(head, temp_mapNo, k, temp_record);
				char *temp_value = (char*)malloc(temp.recordLength);
				int pe = getValueByAttrID(temp_record, temp_pub_attr, temp_value);//得到公共属性上的值
				if (pe < 0){
					printf("bug in join.cpp\n");
					exit(0);
				}
				for (int y = 0; y < m && y < birth_pageNum; y++){
					int birth_mapNo = buffID[y];
					struct pageHead birth_ph;
					memcpy(&birth_ph, head->buff.data[birth_mapNo], SIZE_PAGEHEAD);
					for (int i = 0; i < birth_ph.curRecordNum; i++) {
						char *birth_record = (char*)malloc(birth.recordLength);
						getNextRecord(head, birth_mapNo, i, birth_record);
						char *birth_value = (char*)malloc(birth.recordLength);
						int pd = getValueByAttrID(birth_record, birth_pub_attr, birth_value);//得到公共属性上的值
						if (pd < 0){
							printf("bug in join.cpp\n");
							exit(0);
						}

						if (strcmp(temp_value, birth_value) == 0){
							char *result = (char*)malloc(tmp->recordLength);
							memset(result, 0, tmp->recordLength);
							//连接这两个元组，放入临时表中
							strcpy(result, temp_record);
							strcat(result, "|");
							strncat(result, birth_record, pd);
							strcat(result, birth_record + pd + strlen(birth_value) + 1);
							insertOneRecord(head, tmp_table_dictID, result);
						}
					}
				}

			}
			if (temp_ph.nextPageNo < 0)
				break;
			else
				temp_pageNo = temp_ph.nextPageNo;
		}
	}

	return tmp_table_dictID;
}

int SortJoin(struct dbSysHead *head, int employee_dictID, int department_dictID) {
	Relation emp = head->data_dict[employee_dictID];
	Relation dept = head->data_dict[department_dictID];

	//找到要做连接的公共属性(的下标)
	int emp_pub_attr = 0, dept_pub_attr = 0;
	bool isFound = false;
	for (emp_pub_attr = 0; emp_pub_attr < emp.attributeNum; emp_pub_attr++) {
		for (dept_pub_attr = 0; dept_pub_attr < dept.attributeNum; dept_pub_attr++) {
			if (emp.atb[emp_pub_attr].isSameAttribute(dept.atb[dept_pub_attr])){
				isFound = true;
				break;
			}
		}
		if (isFound)
			break;
	}
	if (isFound == false) {
		printf("连接失败！表%s和表%s没有公共属性！\n", emp.relationName, dept.relationName);
		return -1;
	}
	int tmp_table_dictID = createTmpTable2(head, emp, dept, emp_pub_attr, dept_pub_attr);
	if (tmp_table_dictID < 0){
		printf("创建临时表失败！\n");
		return -1;
	}

	//对两个表进行排序，暂存到临时文件中
	int tmp_table_emp = createTmpTableAfterSort(head, emp, emp_pub_attr);
	int tmp_table_dept = createTmpTableAfterSort(head, dept, dept_pub_attr);

	//just for test!
	//readFile(head, tmp_table_emp);
	//readFile(head, tmp_table_dept);

	Relation *tmp = &(head->data_dict[tmp_table_dictID]);
	Relation *tmp_emp = &(head->data_dict[tmp_table_emp]);
	Relation *tmp_dept = &(head->data_dict[tmp_table_dept]);

	long pageNo_emp = head->desc.fileDesc[tmp_emp->fileID].fileFirstPageNo;
	long pageNum_emp = head->desc.fileDesc[tmp_emp->fileID].filePageNum;

	long pageNo_dept = head->desc.fileDesc[tmp_dept->fileID].fileFirstPageNo;
	long pageNum_dept = head->desc.fileDesc[tmp_dept->fileID].filePageNum;

	for (int j = 0; j < pageNum_dept; j++) {
		int mapNo_dept = reqPage(head, pageNo_dept);
		struct pageHead ph_dept;
		memcpy(&ph_dept, head->buff.data[mapNo_dept], SIZE_PAGEHEAD);
		for (int i = 0; i < ph_dept.curRecordNum; i++) {
			char *record_dept = (char*)malloc(tmp_dept->recordLength);
			getNextRecord(head, mapNo_dept, i, record_dept);
			char *val_dept = (char*)malloc(strlen(record_dept));
			int pd = getValueByAttrID(record_dept, dept_pub_attr, val_dept);

			for (int x = 0; x < pageNum_emp; x++) {
				int mapNo_emp = reqPage(head, pageNo_emp);
				struct pageHead ph_emp;
				memcpy(&ph_emp, head->buff.data[mapNo_emp], SIZE_PAGEHEAD);
				bool flag = false;
				for (int y = 0; y < ph_emp.curRecordNum; y++){
					char *record_emp = (char*)malloc(tmp_emp->recordLength);
					getNextRecord(head, mapNo_emp, y, record_emp);
					char* val_emp = (char*)malloc(strlen(record_emp));
					getValueByAttrID(record_emp, emp_pub_attr, val_emp);
					if (strcmp(val_emp, val_dept) == 0){
						char *result = (char*)malloc(tmp->recordLength);
						memset(result, 0, tmp->recordLength);
						//连接这两个元组，放入临时表中
						strcpy(result, record_emp);
						strcat(result, "|");
						strncat(result, record_dept, pd);
						strcat(result, record_dept + pd + strlen(val_dept) + 1);
						insertOneRecord(head, tmp_table_dictID, result);
					}
					else if (strcmp(val_emp, val_dept) > 0){
						flag = true;
						break;
					}
					else {
						continue;
					}
				}
				if (flag)
					break;
				if (ph_emp.nextPageNo < 0)
					break;
				else
					pageNo_emp = ph_emp.nextPageNo;
			}

		}
		if (ph_dept.nextPageNo < 0)
			break;
		else
			pageNo_dept = ph_dept.nextPageNo;

	}
	return tmp_table_dictID;
}

int HashJoin(struct dbSysHead *head, int employee_dictID, int department_dictID){
	Relation emp = head->data_dict[employee_dictID];
	Relation dept = head->data_dict[department_dictID];

	//找到要做连接的公共属性(的下标)
	int emp_pub_attr = 0, dept_pub_attr = 0;
	bool isFound = false;
	for (emp_pub_attr = 0; emp_pub_attr < emp.attributeNum; emp_pub_attr++) {
		for (dept_pub_attr = 0; dept_pub_attr < dept.attributeNum; dept_pub_attr++) {
			if (emp.atb[emp_pub_attr].isSameAttribute(dept.atb[dept_pub_attr])){
				isFound = true;
				break;
			}
		}
		if (isFound)
			break;
	}
	if (isFound == false) {
		printf("连接失败！表%s和表%s没有公共属性！\n", emp.relationName, dept.relationName);
		return -1;
	}
	int tmp_table_dictID = createTmpTable2(head, emp, dept, emp_pub_attr, dept_pub_attr);
	if (tmp_table_dictID < 0){
		printf("创建临时表失败！\n");
		return -1;
	}
	Relation *tmp = &(head->data_dict[tmp_table_dictID]);
	multimap<int, long> m_emp[BUCKET_NUM];
	multimap<int, long> m_dept[BUCKET_NUM];
	//    int tmp_table_emp = createTmpTableAfterHash(head, emp, emp_pub_attr,m_emp);
	//    int tmp_table_dept = createTmpTableAfterHash(head, dept, dept_pub_attr, m_dept);
	HashRelation(head, emp, emp_pub_attr, m_emp);
	HashRelation(head, dept, dept_pub_attr, m_dept);

	for (int i = 0; i < BUCKET_NUM; i++) {
		map<int, long>::iterator it_emp, it_dept;
		for (it_dept = m_dept[i].begin(); it_dept != m_dept[i].end(); it_dept++) {
			for (it_emp = m_emp[i].begin(); it_emp != m_emp[i].end(); it_emp++){
				if (it_emp->first == it_dept->first){
					char* record_emp = (char*)malloc(emp.recordLength);
					char* record_dept = (char*)malloc(dept.recordLength);
					queryRecordByLogicID(head, it_emp->second, record_emp);
					queryRecordByLogicID(head, it_dept->second, record_dept);

					char* val_dept = new char[strlen(record_dept)];
					int pd = getValueByAttrID(record_dept, dept_pub_attr, val_dept);

					char *result = (char*)malloc(tmp->recordLength);
					memset(result, 0, tmp->recordLength);
					//连接这两个元组，放入临时表中
					strcpy(result, record_emp);
					strcat(result, "|");
					strncat(result, record_dept, pd);
					strcat(result, record_dept + pd + strlen(val_dept) + 1);
					insertOneRecord(head, tmp_table_dictID, result);
				}
				else if (it_emp->first > it_dept->first) {
					break;
				}
				else {
					continue;
				}
			}
		}
	}


	return tmp_table_dictID;
}