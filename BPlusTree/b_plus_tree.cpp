
#include"b_plus_tree.h"
#include<stdio.h>
#include<iostream>
using namespace std;


void getRoot(FILE *index, Node &node)
{
	
	int offset;
	rewind(index);//ָ���ļ��Ŀ�ͷ
	if (fread(&offset, 1, sizeof(offset), index) == 0)
	{
		cout << "offset: " << offset<<endl;
		offset = sizeof(offset);		
		fwrite(&offset, 1, sizeof(offset), index);
		fflush(index);//�����������������������д�뵽�ļ��С�
	}
	fseek(index, offset, SEEK_SET);
	
	if (fread(&node, 1, sizeof(node), index) == 0)
	{
		cout << "offset2�� " << offset<<endl;
		fseek(index, sizeof(node), SEEK_CUR);
		node.count = 0;
		node.parent = 0;
		node.type = LEAF;
	}
	cout << endl;
	
}

int search(FILE *index, int key)
{
	Node node;
	int pos;
	getRoot(index, node);
	searchNode(index, node, key);
	pos = searchRecord(node, key);
	if (pos == node.count || node.pair[pos].key != key)
		return -1;
	else
		return node.pair[pos].pos;
}

int searchRecord(Node &node, int key)
{
	//�ڱ��ڵ����ҵ���һ����key���λ��
	int i;
	for (i = 0; i < node.count && key > node.pair[i].key; i++);
	return i;
}

void searchNode(FILE *index, Node &node, int key)
{
	int pos, offset;
	if (node.type == LEAF){
		return;
	}
	else
	{
		pos = searchRecord(node, key);
		pos = pos == node.count ? pos - 1 : pos;
		offset = node.pair[pos].pos;
		fseek(index, offset, SEEK_SET);
		fread(&node, 1, sizeof(node), index);
		searchNode(index, node, key);
	}
} 

int insertRecord(FILE *index, Node &node, Record record)
{
	
	int pos, cur;
	pos = searchRecord(node, record.key);
	cout << "�����λ���ǣ� " << pos << endl;
	cout << "����ļ�ֵ�ǣ� " << record.key << endl;;
	cout << endl;
	if (pos == node.count || node.pair[pos].key != record.key)
	{
		// node.pair[pos].key != record.key˵���ýڵ��л�û�д洢�ü�¼�����Բ���
		if (node.count < MAX)
		{
			for (cur = node.count; cur > pos; cur--)
			{
				//curָ����Ǳ��ڵ��еĵ�һ����λ�ã��������Ԫ���ǲ�������Ԫ�ص��м���ô����Ҫ�Ѵ��ڴ�����Ԫ�ص�����Ԫ��ͳͳ����һ��λ�á�
				node.pair[cur] = node.pair[cur - 1];
			}
			node.pair[pos] = record;
			node.count++;
			//���˵����ڵ�ĳ�ʼλ�ã������β���ļ�ֵд���ýڵ��С�
			fseek(index, -(int)sizeof(node), SEEK_CUR);
			fwrite(&node, 1, sizeof(node), index);
			fflush(index);
			if (pos == node.count - 1 && node.parent != 0)
			{
				//�����ڸýڵ�ĺ��棬���Ҹ�Ԫ���и��ڵ�Ļ������п��ܻ���ɸ��ڵ��еķָ���ֵ��Ҫ�޸ġ�
				//�����Ҫȥ�鿴���ڵ�,�Ƿ���Ҫ��Ҫ�޸ķָ���ֵ��
				cout << "�޸ĸ��ڵ�" << endl;
				enlargeKey(index, node);
			}
		}
		else
			//�ڵ����������з���
			splitNode(index, node, record, pos);
		return 0;
	}
	else
		return -1;
}

void splitNode(FILE *index, Node &nodea, Record record, int pos)
{
	
	//nodea ����֮ǰ�Ľڵ㡢nodebΪ�����ڵ�
	Node nodeb, top;
	Record recorda, recordb;
	int cur;
	//��ʼ���½ڵ�
	nodeb.type = nodea.type;//���ڵ�����ͱ���һ�£�Ҫô����Ҷ�ӽڵ㣬Ҫô�����ڲ��ڵ㡣
	nodeb.parent = nodea.parent;//���ڵ�ӵ�й�ͬ�ĸ��ڵ㣬�˴˳�Ϊ�ֵܽڵ�
	nodeb.count = (MAX + 1) / 2;//���ܹ�Ҫ���ѵļ�ֵ����Ŀ��һ��������ڵ㡣
	nodea.count = nodea.count - nodeb.count + 1;//ʣ�µ���Ŀ����ԭ���Ľڵ㡣
	for (cur = MAX; cur > pos && cur >= nodea.count; cur--)
		cout << "test1" << endl;
		nodeb.pair[cur - nodea.count] = nodea.pair[cur - 1];
	if (cur < nodea.count)
	{
		cout << "test2" << endl;
		for (; cur > pos; cur--)
			nodea.pair[cur] = nodea.pair[cur - 1];
		nodea.pair[pos] = record;
	}
	else
	{
		//cout << "test3" << endl;
		nodeb.pair[pos - nodea.count] = record;
		for (cur--; cur >= nodea.count; cur--)
			//��ԭ���ڵ�a�еķֳ�ȥ��Ԫ���Ƶ��ڵ�b�С�
			nodeb.pair[cur - nodea.count] = nodea.pair[cur];
	}
	
	fseek(index, -(int)sizeof(nodea), SEEK_CUR);
	recorda.key = nodea.pair[nodea.count - 1].key;
	recorda.pos = ftell(index);
	fseek(index, 0, SEEK_END);
	recordb.key = nodeb.pair[nodeb.count - 1].key;
	recordb.pos = ftell(index);
	fwrite(&nodeb, 1, sizeof(nodeb), index);
	fflush(index);
	if (nodea.parent == 0)
	{
		/*--------------------Ϊ����ʾ��ӡ����Ϣstart----------------------*/
		cout << "���ѽڵ�  ֮  û�и��ڵ�ʱ" << endl;
		for (int temp = 0; temp < nodeb.count; temp++)
		{
			cout << nodeb.pair[temp].key << " ";
		}
		cout << endl;
		cout << "�������ڵ�" << endl;
		/*--------------------Ϊ����ʾ��ӡ����Ϣend----------------------*/
		top.count = 0;
		top.type = NODE;//����Ϊ�ڲ��ڵ�
		top.parent = 0;
		/*���������ӽڵ�͸��ڵ��������*/
		fseek(index, 0, SEEK_END);
		cur = ftell(index);
		nodea.parent = cur;
		nodeb.parent = cur;
		//��ʱ�ڵ�a�ͽڵ�b�ĸ��ڵ����Զ����˸�ֵ���������½���д�뵽�����ϡ�
		fseek(index, recorda.pos, SEEK_SET);
		fwrite(&nodea, 1, sizeof(nodea), index);
		fflush(index);
		fseek(index, recordb.pos, SEEK_SET);
		fwrite(&nodeb, 1, sizeof(nodeb), index);
		fflush(index);
		fseek(index, sizeof(top), SEEK_CUR);
		insertRecord(index, top, recorda);
		insertRecord(index, top, recordb);
		rewind(index);
		fwrite(&cur, 1, sizeof(cur), index);//����ǰ���ڵ��λ��д�������ļ�����ʵλ�ã�Ҳ�����Ǹ�offset�С�
		fflush(index);
	}
	else
	{
		cout << "���ѽڵ�   ֮  �и��ڵ�ʱ" << endl;
		fseek(index, recorda.pos, SEEK_SET);
		fwrite(&nodea, 1, sizeof(nodea), index);
		fflush(index);
		//fseek(index, recordb.pos, SEEK_SET);//��ʵ�������д��recordb�ģ���Ϊ֮ǰ�Ѿ�д���ˣ�����֮��û���޸ġ�
		//fwrite(&nodeb, 1, sizeof(nodeb), index);
		//fflush(index);
		fseek(index, nodea.parent, SEEK_SET);
		fread(&top, 1, sizeof(top), index);
		cur = searchRecord(top, recordb.key);
		if (cur == top.count)
			cur--;
		top.pair[cur] = recorda;
		insertRecord(index, top, recordb);
	}

	if (nodeb.type == NODE)
	{
		cout << "�޸��ڲ��ڵ㣬�ı亢�ӵĸ��ڵ�"<<endl;
		Node node;
		for (cur = 0; cur < nodeb.count; cur++){
			cout << "nodeb.pair[cur].key" << nodeb.pair[cur].key << endl;
			cout << "recordb   " << recordb.key << endl;
			//����node�ڵ����֮�����Щ����Ԫ�����·ֵ�������Ľڵ㵱��
			//�����⼸������Ԫ�صĸ��ڵ���Ȼ��δ����֮ǰ���Ǹ��ڵ㡣
			//������Ҫ���ڰѷ��ѳ�ȥ�ĺ��ӽڵ�ĸ��ڵ�ı���·��ѵ�����ڵ㣬Ҳ������ν��change parent
			changeParent(index, node, nodeb.pair[cur].pos, recordb.pos);
		}
	}
}

int insert(FILE *index, Record record)
{
	Node node;	
	getRoot(index, node);
	cout << "node.type " << node.type << endl;
	cout << "node.parent " << node.parent << endl;
	cout << "node.count " << node.count << endl;
	cout << "node.pair[0].key " << node.pair[0].key<<endl; 
	cout << "node.pair[1].key " << node.pair[1].key << endl;
	cout << "node.pair[2].key " << node.pair[2].key << endl;
	cout << "node.pair[3].key " << node.pair[3].key << endl;
	cout << "node.pair[4].key " << node.pair[4].key << endl;
	searchNode(index, node, record.key);
	return insertRecord(index, node, record);
}

void enlargeKey(FILE *index, Node &node)
{
	int key;
	key = node.pair[node.count - 1].key;
	fseek(index, node.parent, SEEK_SET);
	fread(&node, 1, sizeof(node), index);
/*	int pos = searchRecord(node, key)-1;
	if (node.pair[pos].key < key)
	{
		node.pair[pos].key = key;
		fseek(index, -(int)sizeof(node), SEEK_CUR);
		fwrite(&node, 1, sizeof(node), index);
		fflush(index);
		if (node.parent != 0)
		{
			enlargeKey(index, node);
		}
	}
	*/
	if (node.pair[node.count - 1].key < key)
	{
		cout << "�� " << node.pair[node.count - 1].key << " �޸ĳ� " << key<<endl;
		node.pair[node.count - 1].key = key;
		fseek(index, -(int)sizeof(node), SEEK_CUR);
		fwrite(&node, 1, sizeof(node), index);
		fflush(index);
		if (node.parent != 0)
		{
			enlargeKey(index, node);
		}
	}
}

int del(FILE *index, int key)
{
	cout << "��ʼɾ��" << key << endl;
	Node node;
	getRoot(index, node);
	cout << "�ҵ�root�ڵ�" << endl;
	cout << "root.count:" << node.count << endl;
	cout << "root��¼"<<endl;
	for (int i = 0; i < node.count; i++)
	{
		cout <<  node.pair[i].key << "   ";
	}
	cout <<  endl;
	
	searchNode(index, node, key);
	cout << "�ҵ�Ҫɾ����¼��Ҷ�ӽڵ�" << endl;
	cout << "node��¼" << endl;
	for (int i = 0; i < node.count; i++)
	{
		cout << node.pair[i].key << "   ";
	}
	cout << endl;
	return delRecord(index, node, key);
}

int delRecord(FILE *index, Node &node, int key)
{
	Node left, right, top,root;//��ʾ��ɾ��Ԫ�����ڽڵ�����ڽڵ㣬���ڽڵ㣬�Լ����ڵ�
	Record recon ,recol, recor, recot;//��ʾ���ڵ��д�ɾ���ڵ㣬���ڽڵ㣬���ڽڵ�ķָ�Ԫ�أ��Լ����ڵ�ĸ��ڵ㡣
	int pos, tpos;
	pos = searchRecord(node, key);
	cout << "Ҫɾ����¼��λ��pos:" << pos << endl;
	if (pos == node.count)
		return -1;//һֱ�ҵ����һ���ڵ����һ��λ�ã�����û���ҵ�
	else if (node.parent == 0 || node.count > MIN)
	{//��û�и��ڵ㣬���Ҵ��ڰ�������
		int posTem = pos;
		cout << "ɾ����λ���Ǹ��ڵ�" << endl;
		cout << "ɾ����¼��λ���ǣ�"<<pos << endl;
		for (node.count--; posTem < node.count; posTem++)
			node.pair[posTem] = node.pair[posTem + 1];//�������Ԫ��ͳͳ��ǰ�ƶ�
		cout << "��ɾ��λ�ú�ļ�¼ǰ��" << endl;
		cout << "��ʱ��node�ڵ㣺" << endl;
		cout << "node.count:" << node.count << endl;
		if (node.parent == 0 && node.count == 1 && node.type == NODE)
		{
			cout << "ɾ��Ԫ��֮�󣬸��ڵ���ֻ��һ��Ԫ�أ���ô������ڵ��Ҫ��ȡ�������к��ӵĸ�����Ϊ0" << endl;
			node.count = 0;
			fseek(index, -(int)sizeof(node), SEEK_CUR);
			fwrite(&node, 1, sizeof(node), index);
			fflush(index);
			rewind(index);
			cout << "����root�ڵ��λ�ã���root�ڵ����Ϊnode�ĺ��ӽڵ�" << endl;
			fwrite(&node.pair[0].pos, 1, sizeof(node.pair[0].pos), index);
			fflush(index);
			changeParent(index, node, node.pair[0].pos, 0);
			cout << "��ʱ��root�ڵ�Ϊ��";
			getRoot(index, root);
			for (int i = 0; i < root.count; i++)
			{
				cout << root.pair[i].key << "  ";
			}
			cout << endl;

			

		}
		else
		{
			//���ڵ���Ԫ�ظ�������3��ʱ��ɾ����ֱ��ˢ�ش���
			fseek(index, -(int)sizeof(node), SEEK_CUR);
			fwrite(&node, 1, sizeof(node), index);
			fflush(index);
			if (node.parent != 0 && pos == node.count)
				//��ɾ��Ԫ�����ڵĽڵ��и��ڵ㣬��ɾ����Ԫ���ǽڵ������һ��Ԫ�أ���ô��Ҫ�ı丸�ڵ��зָ�Ԫ�ص�key��
				ensmallKey(index, node);
		}
		return 0;
	}
	else
	{
		//cout << "ɾ������" << endl;
		//recon����ڵ�������Ԫ��ɾ��֮�󣬸��¸��ڵ��зָ�Ԫ�ء�
		//��ɾ��Ԫ��֮�����ڽڵ�����һ��λ�ý��ᱻ����Ϊ���ڽڵ��еĵ�һ��Ԫ�أ�
		//��˸��ڵ��е������ҵ��ýڵ���Ǹ��ָ�Ԫ��Ҳ��Ҫ���¡�
		//recon��key���Ǵ�ɾ��Ԫ�����ڽڵ�����һ��Ԫ��
		//recon��pos���Ǵ�ɾ��Ԫ�����ڽڵ����ʼ��ַ
		cout << "ɾ����¼�󣬼�¼��<Min,���������Һ��ӽ���ߺϲ�" << endl;
		recon.key = node.pair[node.count - 1].key;//��Ԫ�����ڽڵ�����һ��key
		recon.pos = ftell(index) - sizeof(node);
		//recot�ڵ�ָ���Ԫ�صĸ��ڵ�
		recot.pos = node.parent;
		fseek(index, recot.pos, SEEK_SET);
		fread(&top, 1, sizeof(top), index);
		cout << "�ҵ�node�ĸ��׽ڵ�" << endl;
		cout << "top��¼" << endl;
		for (int i = 0; i < top.count; i++)
		{
			cout << top.pair[i].key << "   ";
		}
		cout << endl;
		recot.key = top.pair[top.count - 1].key;//recot�ڵ��keyֵ�Ǹ�Ԫ�صĸ��ڵ�����һ��key
	//	cout << "recot.key" << recot.key << endl;
		tpos = searchRecord(top, key);//�ڴ�ɾ��Ԫ�صĸ��ڵ����ҵ���һ���������λ��
		cout <<"tpos:"<< tpos << endl;
		if (tpos == top.count - 1)
		{//��ɾ����λ���ǽڵ��е����һ��Ԫ��
			cout << "���Һ���" << endl;
			recol = top.pair[tpos - 1];
			fseek(index, recol.pos, SEEK_SET);
			fread(&left, 1, sizeof(left), index);
			cout << "�ҵ����ӵ�Ҷ�ӽڵ�" << endl;
			cout << "left.count:" << left.count << endl;
			for (int i = 0; i < left.count; i++)
			{
				cout << left.pair[i].key << "   ";
			}
			if (left.count > MIN)
			{
				transRecord(index, left, node, RIGHT, pos);
				if (node.type == NODE)
					changeParent(index, right, node.pair[0].pos, recon.pos);
				recon.key = node.pair[node.count - 1].key;
				recol.key = left.pair[left.count - 1].key;
				top.pair[tpos - 1] = recol;
				top.pair[tpos] = recon;
				fseek(index, recol.pos, SEEK_SET);
				fwrite(&left, 1, sizeof(left), index);
				fflush(index);
				fseek(index, recon.pos, SEEK_SET);
				fwrite(&node, 1, sizeof(node), index);
				fflush(index);
				fseek(index, recot.pos, SEEK_SET);
				fwrite(&top, 1, sizeof(top), index);
				fflush(index);
			}
			else
			{
				cout << "���ӵļ�¼��ĿС��Min" << endl;
				for (node.count--; pos < node.count; pos++)
					node.pair[pos] = node.pair[pos + 1];

				cout << "��node��ɾ����¼����ļ�¼ǰ��" << endl;
				cout << "node.count:" << node.count << endl;
				for (int i = 0; i < node.count; i++)
				{
					cout << node.pair[i].key << "   ";
				}
				if (node.type == NODE)
				{
					for (pos = 0; pos < node.count; pos++)
						changeParent(index, right, node.pair[pos].pos, recol.pos);
				}
				mergeNode(index, left, node);
				cout << "�����Ӻ�node�ϲ�" << endl;
				fseek(index, recol.pos, SEEK_SET);
				fwrite(&left, 1, sizeof(left), index);
				fflush(index);
				cout << "left:" << endl;
				for (int i = 0; i < left.count; i++)
				{
					cout << left.pair[i].key << "   ";
				}
				fseek(index, recon.pos, SEEK_SET);
				fwrite(&node, 1, sizeof(node), index);
				fflush(index);
				cout << "node:" << endl;
				for (int i = 0; i < node.count; i++)
				{
					cout << node.pair[i].key << "   ";
				}
				cout << endl;
				cout << "�����ڵ�tpos�ұ�һ��Ԫ�ص�ָ��ϲ���Ľڵ㣬��node�ڵ��λ��" << endl;
				recon.key = left.pair[left.count - 1].key;
				recon.pos = recol.pos;
				top.pair[tpos] = recon;
				fseek(index, recot.pos, SEEK_SET);
				fwrite(&top, 1, sizeof(top), index);
				fflush(index);
				cout << "�����ڵ��еľɵķָ�Ԫ��:" << recol.key << "ɾ��" << endl;
				cout << endl;
				delRecord(index, top, recol.key);
				//ע������ɾ���ķָ�Ԫ�������ڽڵ�ķָ�Ԫ�ض����Ǳ��ڵ�ķָ�Ԫ�ء���Ϊ���ڵ�ķָ�Ԫ����
			}
			if (top.parent != 0 && top.count != 0)
				ensmallKey(index, top);
		}
		else
		{
			//ɾ����Ԫ���ǽڵ��е��м�Ԫ��
			cout << "tpos!=top.count,���Һ���"<< endl;
		//	cout << "top.pair[tpos]" << top.pair[tpos].key << endl;
			recor = top.pair[tpos + 1];
			//recor.pos�����ɾ��Ԫ�ص����ڽڵ�����ڽڵ�ĵ�ַ��
			fseek(index, recor.pos, SEEK_SET);
		//	cout << "recor" << recor.key << endl;
			fread(&right, 1, sizeof(right), index);
			cout << "�Һ��ӣ�" << endl;
			cout << "right.count:" << right.count << endl;
			for (int i = 0; i < right.count; i++)
			{
				cout << right.pair[i].key << "   ";
			}
			cout << endl;
			if (right.count > MIN)
			{
				//���ڽڵ����3�������
				transRecord(index, node, right, LEFT, pos);//����ɾ��Ԫ��ɾ��
				if (node.type == NODE)
					changeParent(index, left, node.pair[node.count - 1].pos, recon.pos);
				//��transRecord���֮��
				//��Ԫ�����ڽڵ�����һ��Ԫ�ر�����Ϊ���ڽڵ��еĵ�һ��Ԫ��
				//���Ҹ��¸��ڵ��еķָ�Ԫ��
				recon.key = node.pair[node.count - 1].key;
				top.pair[tpos] = recon;
				//�����º���ҽڵ�д�ش��̣���һ��Ԫ�ر��Ƶ�ǰһ���ڵ��У�
				fseek(index, recor.pos, SEEK_SET);
				fwrite(&right, 1, sizeof(right), index);
				fflush(index);
				//�����º�ĵ�ǰ�ڵ�д�ش��̣�Ŀ��ɾ��Ԫ�ر�ɾ���������������ڽڵ�ĵ�һ��Ԫ�أ�
				fseek(index, recon.pos, SEEK_SET);
				fwrite(&node, 1, sizeof(node), index);
				fflush(index);
				//�����º�ĸ��ڵ�д�ش��̣����ڵ��еķָ�Ԫ�ظ����ˣ�
				fseek(index, recot.pos, SEEK_SET);
				fwrite(&top, 1, sizeof(top), index);
				fflush(index);
			}
			else if (tpos != 0)
			{
				cout << "�Һ��ӹؼ��ֵ���Ŀ<Min,��������"<< endl;
				
				//���統ǰ�ڵ㲻�ǵ�һ���ڵ�Ļ����������ڽڵ���Ѿ�С�ڰ���״̬������
				recol = top.pair[tpos - 1];
				fseek(index, recol.pos, SEEK_SET);
				fread(&left, 1, sizeof(left), index);
				cout << "���ӣ�" << endl;
				cout << "left.count:" << left.count << endl;
				for (int i = 0; i <left.count; i++)
				{
					cout << left.pair[i].key << "   ";
				}
				cout << endl;
				if (left.count > MIN)
				{
					//���ڽڵ����3��������
					transRecord(index, left, node, RIGHT, pos);
					if (node.type == NODE)
						changeParent(index, right, node.pair[0].pos, recon.pos);
					recon.key = node.pair[node.count - 1].key;
					recol.key = left.pair[left.count - 1].key;
					top.pair[tpos - 1] = recol;//���¸��¸��ڵ��е���ָ�Ԫ��
					top.pair[tpos] = recon;//���¸��¸��ڵ��еķָ�Ԫ��
					fseek(index, recol.pos, SEEK_SET);
					fwrite(&left, 1, sizeof(left), index);
					fflush(index);
					fseek(index, recon.pos, SEEK_SET);
					fwrite(&node, 1, sizeof(node), index);
					fflush(index);
					fseek(index, recot.pos, SEEK_SET);
					fwrite(&top, 1, sizeof(top), index);
					fflush(index);
				}
				else
				{
					//���ҽڵ���ȫ����С�ڻ����3���ڵ�
					cout << "������Ŀ<Min" << endl;
					for (node.count--; pos < node.count; pos++)
						node.pair[pos] = node.pair[pos + 1];
					if (right.type == NODE)
					{
						for (pos = 0; pos < right.count; pos++)
							changeParent(index, left, right.pair[pos].pos, recon.pos);
					}
					mergeNode(index, node, right);//���ҽڵ���кϲ�
					cout << "node���Һ��ӽ��кϲ�" << endl;
					fseek(index, recor.pos, SEEK_SET);
					fwrite(&right, 1, sizeof(right), index);
					cout << "right��" << endl;
					cout << "right.count:" << right.count << endl;
					for (int i = 0; i < right.count; i++)
					{
						cout << right.pair[i].key << "   ";
					}
					cout << endl;
					fflush(index);
					fseek(index, recon.pos, SEEK_SET);
					fwrite(&node, 1, sizeof(node), index);
					cout << "node��" << endl;
					cout << "node.count:" << node.count << endl;
					cout << "node��¼" << endl;
					for (int i = 0; i < node.count; i++)
					{
						cout << node.pair[i].key << "   ";
					}
					cout << endl;
					fflush(index);
					//��Ϊ���ڸýڵ�����һ��ֵ�Ѿ����ԭ�������ڽڵ��е����һ����
					//���Ե�ǰ�ڵ�ķָ�Ԫ�ر�����ڽڵ�ķָ�Ԫ��
					//֮��ɾ����ԭ�����Ǹ��ָ�Ԫ�ؾͿ���
					cout << "top:" << endl;
					for (int i = 0; i < top.count; i++)
					{
						cout << top.pair[i].key << "   ";
					}
					cout <<"�����ڵ�tpos�ұ�һ��Ԫ�ص�ָ��ϲ���Ľڵ㣬��node�ڵ��λ��" << endl;
					recor.pos = recon.pos;//�ѵ�ǰ�ڵ��ڸ��ڵ��зָ�Ԫ�ص�pos����ֵ�����ڵ��ڸ��ڵ��зָ�Ԫ�ص�pos
					top.pair[tpos + 1] = recor;//���¸��ڵ��е����ڷָ�Ԫ��
					fseek(index, recot.pos, SEEK_SET);
					fwrite(&top, 1, sizeof(top), index);
					fflush(index);
					//ɾ�����ڵ��еĵ�ǰԪ�صľɷָ�Ԫ�أ�
					
					cout <<"�����ڵ��еľɵķָ�Ԫ��:"<<recon.key<<"ɾ��" << endl;
					cout << endl;
					delRecord(index, top, recon.key);
					
				}
			}
			else
			{
				//��ɾ��Ԫ�����ڵĽڵ����丸�ڵ��еĵ�һ���ڵ㣬�����ڽڵ�û����ڵ�
				for (node.count--; pos < node.count; pos++)
					node.pair[pos] = node.pair[pos + 1];
				if (right.type == NODE)
				{
					for (pos = 0; pos < right.count; pos++)
						changeParent(index, left, right.pair[pos].pos, recon.pos);
				}
				mergeNode(index, node, right);
				fseek(index, recor.pos, SEEK_SET);
				fwrite(&right, 1, sizeof(right), index);
				fflush(index);
				fseek(index, recon.pos, SEEK_SET);
				fwrite(&node, 1, sizeof(node), index);
				fflush(index);
				recor.pos = recon.pos;
				top.pair[tpos + 1] = recor;
				fseek(index, recot.pos, SEEK_SET);
				fwrite(&top, 1, sizeof(top), index);
				fflush(index);
				delRecord(index, top, recon.key);
			}
		}
		return 0;
	}
}

void transRecord(FILE *index, Node &left, Node &right, int dir, int pos)
{
	if (dir == RIGHT)
	{
		for (; pos > 0; pos--)
			right.pair[pos] = right.pair[pos - 1];//����ɾ��Ԫ�ص�ǰ���Ԫ��ͳͳ����
		right.pair[0] = left.pair[left.count-1];//�����ڽڵ��е����һ��Ԫ���ƶ�����ǰ�ڵ��еĵ�һ��Ԫ�ص�λ�á�
		left.count--;
	}
	else
	{
		for (; pos < left.count - 1; pos++)
			left.pair[pos] = left.pair[pos + 1];//����ɾ��Ԫ�صĺ����Ԫ��ͳͳǰ��
		left.pair[pos] = right.pair[0];
		//��Ϊ�ýڵ��Ȼ�����ڽڵ㣬˵����ǰ�ڵ����ٴ��ڰ���״̬��
		//���������ڽڵ��еĸ������ڰ����������ƶ�һ��key֮�󣬽ڵ���Ȼ�ᴦ�ڰ���״̬
		//�����ڽڵ��еĵ�һ��key�ƶ�����ǰ�ڵ��У�ʹ��Ŀǰ�ڵ���Ȼ�Ǵ��ڰ���״̬
		right.count--;
		//����ɾ��Ԫ�صĺ����Ԫ��ͳͳǰ��һ��
		for (pos = 0; pos < right.count; pos++)
			right.pair[pos] = right.pair[pos + 1];
	}
}

void mergeNode(FILE *index, Node &left, Node &right)
{
	//��ȻҪ�ϲ����ٴ������µ����������ǣ���ǰ�ڵ�ɾ��Ԫ��֮�����<=2,���ڽڵ�ĸ���<=3,
	//��������֮�͵ĸ���<=5,���һ���Ǻϲ�֮��ԭ�����ڽڵ��е�Ԫ��ȫ���ƶ�����ǰ�ڵ��С�
	int pos;
	for (pos = 0; pos < right.count; pos++)
		left.pair[pos + left.count] = right.pair[pos];
	left.count += right.count;
	right.count = 0;
}
                                
void changeParent(FILE *index, Node &node, int child, int parent)
{
	//cout << "child: " << child << endl;
	//cout << "parent: " << parent << endl;
	fseek(index, child, SEEK_SET);
	fread(&node, 1, sizeof(node), index);
	node.parent = parent;
	fseek(index, -(int)sizeof(node), SEEK_CUR);
	fwrite(&node, 1, sizeof(node), index);
	fflush(index);
}

void ensmallKey(FILE *index, Node &node)
{
	//��ɾ����Ԫ����λ�ڽڵ��е����һ��ʱ�����ڵ��еķָ�Ԫ�ؿ��ܻᷢ���仯��
	//���緢���仯ʱ����ô���ڵ����丸�ڵ��еķָ�Ԫ��Ҳ����Ҫ���µġ�
	//��������ǵݹ��
	int key, pos;
	key = node.pair[node.count - 1].key;
	fseek(index, node.parent, SEEK_SET);
	fread(&node, 1, sizeof(node), index);
	pos = searchRecord(node, key);
	if (node.pair[pos].key != key)
	{
		node.pair[pos].key = key;
		fseek(index, -(int)sizeof(node), SEEK_CUR);
		fwrite(&node, 1, sizeof(node), index);
		fflush(index);
		if (pos == node.count - 1 && node.parent != 0)
			ensmallKey(index, node);
	}
}
void showTree(FILE *index, Node &node){
	int num=1;
	Node childNode;
	if (node.type==LEAF){
		cout << "Ҷ�ӽڵ�:  ";
		for (int i4 = 0; i4< node.count; i4++)
		{
			cout << node.pair[i4].key << "   ";
		}
		return;
	}
	else
	{
		printf("the %dth:\n ",num);
		for (int j = 0; j < node.count; j++)
		{
			cout << node.pair[j].key << "  ";			
		}
		printf("\n ");
	
		for (int i3 = 0; i3< node.count; i3++)
		{
					
			rewind(index);
			fseek(index, node.pair[i3].pos, SEEK_SET);
			fread(&childNode, 1, sizeof(childNode), index);	
			
			showTree(index ,childNode);	
			printf("\n ");
		}
		num++;		
	}	
}