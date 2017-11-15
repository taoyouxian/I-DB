
#include"b_plus_tree.h"
#include<stdio.h>
#include<iostream>
using namespace std;


void getRoot(FILE *index, Node &node)
{
	
	int offset;
	rewind(index);//指向文件的开头
	if (fread(&offset, 1, sizeof(offset), index) == 0)
	{
		cout << "offset: " << offset<<endl;
		offset = sizeof(offset);		
		fwrite(&offset, 1, sizeof(offset), index);
		fflush(index);//清除缓冲区，将缓冲区内容写入到文件中。
	}
	fseek(index, offset, SEEK_SET);
	
	if (fread(&node, 1, sizeof(node), index) == 0)
	{
		cout << "offset2： " << offset<<endl;
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
	//在本节点中找到第一个比key大的位置
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
	cout << "插入的位置是： " << pos << endl;
	cout << "插入的键值是： " << record.key << endl;;
	cout << endl;
	if (pos == node.count || node.pair[pos].key != record.key)
	{
		// node.pair[pos].key != record.key说明该节点中还没有存储该记录，可以插入
		if (node.count < MAX)
		{
			for (cur = node.count; cur > pos; cur--)
			{
				//cur指向的是本节点中的第一个空位置，若插入的元素是插在已有元素的中间那么就需要把大于待插入元素的所有元素统统后移一个位置。
				node.pair[cur] = node.pair[cur - 1];
			}
			node.pair[pos] = record;
			node.count++;
			//回退到本节点的初始位置，将本次插入的键值写进该节点中。
			fseek(index, -(int)sizeof(node), SEEK_CUR);
			fwrite(&node, 1, sizeof(node), index);
			fflush(index);
			if (pos == node.count - 1 && node.parent != 0)
			{
				//若是在该节点的后面，并且该元素有父节点的话，就有可能会造成父节点中的分隔键值需要修改。
				//因此需要去查看父节点,是否需要需要修改分隔键值。
				cout << "修改父节点" << endl;
				enlargeKey(index, node);
			}
		}
		else
			//节点已满，进行分裂
			splitNode(index, node, record, pos);
		return 0;
	}
	else
		return -1;
}

void splitNode(FILE *index, Node &nodea, Record record, int pos)
{
	
	//nodea 分裂之前的节点、nodeb为新增节点
	Node nodeb, top;
	Record recorda, recordb;
	int cur;
	//初始化新节点
	nodeb.type = nodea.type;//两节点的类型保持一致，要么都是叶子节点，要么都是内部节点。
	nodeb.parent = nodea.parent;//两节点拥有共同的父节点，彼此成为兄弟节点
	nodeb.count = (MAX + 1) / 2;//把总共要分裂的键值的数目分一半给新增节点。
	nodea.count = nodea.count - nodeb.count + 1;//剩下的数目留给原来的节点。
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
			//把原来节点a中的分出去的元素移到节点b中。
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
		/*--------------------为了显示打印的信息start----------------------*/
		cout << "分裂节点  之  没有父节点时" << endl;
		for (int temp = 0; temp < nodeb.count; temp++)
		{
			cout << nodeb.pair[temp].key << " ";
		}
		cout << endl;
		cout << "建立父节点" << endl;
		/*--------------------为了显示打印的信息end----------------------*/
		top.count = 0;
		top.type = NODE;//类型为内部节点
		top.parent = 0;
		/*将两个孩子节点和父节点关联起来*/
		fseek(index, 0, SEEK_END);
		cur = ftell(index);
		nodea.parent = cur;
		nodeb.parent = cur;
		//此时节点a和节点b的父节点属性都有了赋值，所以重新将其写入到磁盘上。
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
		fwrite(&cur, 1, sizeof(cur), index);//将当前根节点的位置写到整个文件的其实位置，也就是那个offset中。
		fflush(index);
	}
	else
	{
		cout << "分裂节点   之  有父节点时" << endl;
		fseek(index, recorda.pos, SEEK_SET);
		fwrite(&nodea, 1, sizeof(nodea), index);
		fflush(index);
		//fseek(index, recordb.pos, SEEK_SET);//其实这里可以写入recordb的，因为之前已经写过了，并且之后没有修改。
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
		cout << "修改内部节点，改变孩子的父节点"<<endl;
		Node node;
		for (cur = 0; cur < nodeb.count; cur++){
			cout << "nodeb.pair[cur].key" << nodeb.pair[cur].key << endl;
			cout << "recordb   " << recordb.key << endl;
			//由于node节点分裂之后的那些孩子元素重新分到了右面的节点当中
			//但是这几个孩子元素的父节点仍然是未分裂之前的那个节点。
			//所以需要现在把分裂出去的孩子节点的父节点改变成新分裂的这个节点，也就是所谓的change parent
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
		cout << "将 " << node.pair[node.count - 1].key << " 修改成 " << key<<endl;
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
	cout << "开始删除" << key << endl;
	Node node;
	getRoot(index, node);
	cout << "找到root节点" << endl;
	cout << "root.count:" << node.count << endl;
	cout << "root记录"<<endl;
	for (int i = 0; i < node.count; i++)
	{
		cout <<  node.pair[i].key << "   ";
	}
	cout <<  endl;
	
	searchNode(index, node, key);
	cout << "找到要删除记录的叶子节点" << endl;
	cout << "node记录" << endl;
	for (int i = 0; i < node.count; i++)
	{
		cout << node.pair[i].key << "   ";
	}
	cout << endl;
	return delRecord(index, node, key);
}

int delRecord(FILE *index, Node &node, int key)
{
	Node left, right, top,root;//表示待删除元素所在节点的左邻节点，右邻节点，以及父节点
	Record recon ,recol, recor, recot;//表示父节点中待删除节点，左邻节点，右邻节点的分隔元素，以及父节点的父节点。
	int pos, tpos;
	pos = searchRecord(node, key);
	cout << "要删除记录的位置pos:" << pos << endl;
	if (pos == node.count)
		return -1;//一直找到最后一个节点的下一个位置，代表没有找到
	else if (node.parent == 0 || node.count > MIN)
	{//若没有父节点，并且大于半满个数
		int posTem = pos;
		cout << "删除的位置是根节点" << endl;
		cout << "删除记录的位置是："<<pos << endl;
		for (node.count--; posTem < node.count; posTem++)
			node.pair[posTem] = node.pair[posTem + 1];//将后面的元素统统向前移动
		cout << "将删除位置后的记录前移" << endl;
		cout << "此时的node节点：" << endl;
		cout << "node.count:" << node.count << endl;
		if (node.parent == 0 && node.count == 1 && node.type == NODE)
		{
			cout << "删除元素之后，根节点中只有一个元素，那么这个根节点就要被取消，所有孩子的父节设为0" << endl;
			node.count = 0;
			fseek(index, -(int)sizeof(node), SEEK_CUR);
			fwrite(&node, 1, sizeof(node), index);
			fflush(index);
			rewind(index);
			cout << "更新root节点的位置，将root节点更改为node的孩子节点" << endl;
			fwrite(&node.pair[0].pos, 1, sizeof(node.pair[0].pos), index);
			fflush(index);
			changeParent(index, node, node.pair[0].pos, 0);
			cout << "此时的root节点为：";
			getRoot(index, root);
			for (int i = 0; i < root.count; i++)
			{
				cout << root.pair[i].key << "  ";
			}
			cout << endl;

			

		}
		else
		{
			//当节点中元素个数大于3的时候，删除完直接刷回磁盘
			fseek(index, -(int)sizeof(node), SEEK_CUR);
			fwrite(&node, 1, sizeof(node), index);
			fflush(index);
			if (node.parent != 0 && pos == node.count)
				//若删除元素所在的节点有父节点，且删除的元素是节点中最后一个元素，那么就要改变父节点中分隔元素的key。
				ensmallKey(index, node);
		}
		return 0;
	}
	else
	{
		//cout << "删除函数" << endl;
		//recon这个节点用来当元素删除之后，更新父节点中分隔元素。
		//因删除元素之后，所在节点的最后一个位置将会被更新为右邻节点中的第一个元素，
		//因此父节点中的用于找到该节点的那个分隔元素也需要更新。
		//recon的key就是待删除元素所在节点的最后一个元素
		//recon的pos就是待删除元素所在节点的起始地址
		cout << "删除记录后，记录数<Min,考虑向左右孩子借或者合并" << endl;
		recon.key = node.pair[node.count - 1].key;//本元素所在节点的最后一个key
		recon.pos = ftell(index) - sizeof(node);
		//recot节点指向该元素的父节点
		recot.pos = node.parent;
		fseek(index, recot.pos, SEEK_SET);
		fread(&top, 1, sizeof(top), index);
		cout << "找到node的父亲节点" << endl;
		cout << "top记录" << endl;
		for (int i = 0; i < top.count; i++)
		{
			cout << top.pair[i].key << "   ";
		}
		cout << endl;
		recot.key = top.pair[top.count - 1].key;//recot节点的key值是该元素的父节点的最后一个key
	//	cout << "recot.key" << recot.key << endl;
		tpos = searchRecord(top, key);//在待删除元素的父节点中找到第一个比它大的位置
		cout <<"tpos:"<< tpos << endl;
		if (tpos == top.count - 1)
		{//若删除的位置是节点中的最后一个元素
			cout << "无右孩子" << endl;
			recol = top.pair[tpos - 1];
			fseek(index, recol.pos, SEEK_SET);
			fread(&left, 1, sizeof(left), index);
			cout << "找到左孩子的叶子节点" << endl;
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
				cout << "左孩子的记录数目小于Min" << endl;
				for (node.count--; pos < node.count; pos++)
					node.pair[pos] = node.pair[pos + 1];

				cout << "将node中删除记录后面的记录前移" << endl;
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
				cout << "将左孩子和node合并" << endl;
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
				cout << "将父节点tpos右边一个元素的指向合并后的节点，即node节点的位置" << endl;
				recon.key = left.pair[left.count - 1].key;
				recon.pos = recol.pos;
				top.pair[tpos] = recon;
				fseek(index, recot.pos, SEEK_SET);
				fwrite(&top, 1, sizeof(top), index);
				fflush(index);
				cout << "将父节点中的旧的分隔元素:" << recol.key << "删除" << endl;
				cout << endl;
				delRecord(index, top, recol.key);
				//注意这里删除的分隔元素是左邻节点的分隔元素而不是本节点的分隔元素。因为本节点的分隔元素是
			}
			if (top.parent != 0 && top.count != 0)
				ensmallKey(index, top);
		}
		else
		{
			//删除的元素是节点中的中间元素
			cout << "tpos!=top.count,有右孩子"<< endl;
		//	cout << "top.pair[tpos]" << top.pair[tpos].key << endl;
			recor = top.pair[tpos + 1];
			//recor.pos代表待删除元素的所在节点的右邻节点的地址。
			fseek(index, recor.pos, SEEK_SET);
		//	cout << "recor" << recor.key << endl;
			fread(&right, 1, sizeof(right), index);
			cout << "右孩子：" << endl;
			cout << "right.count:" << right.count << endl;
			for (int i = 0; i < right.count; i++)
			{
				cout << right.pair[i].key << "   ";
			}
			cout << endl;
			if (right.count > MIN)
			{
				//右邻节点大于3的情况下
				transRecord(index, node, right, LEFT, pos);//将待删除元素删掉
				if (node.type == NODE)
					changeParent(index, left, node.pair[node.count - 1].pos, recon.pos);
				//在transRecord完成之后：
				//该元素所在节点的最后一个元素被更新为右邻节点中的第一个元素
				//并且更新父节点中的分隔元素
				recon.key = node.pair[node.count - 1].key;
				top.pair[tpos] = recon;
				//将更新后的右节点写回磁盘（第一个元素被移到前一个节点中）
				fseek(index, recor.pos, SEEK_SET);
				fwrite(&right, 1, sizeof(right), index);
				fflush(index);
				//将更新后的当前节点写回磁盘（目标删除元素被删掉，且增加了右邻节点的第一个元素）
				fseek(index, recon.pos, SEEK_SET);
				fwrite(&node, 1, sizeof(node), index);
				fflush(index);
				//将更新后的父节点写回磁盘（父节点中的分隔元素更新了）
				fseek(index, recot.pos, SEEK_SET);
				fwrite(&top, 1, sizeof(top), index);
				fflush(index);
			}
			else if (tpos != 0)
			{
				cout << "右孩子关键字的数目<Min,看其左孩子"<< endl;
				
				//假如当前节点不是第一个节点的话，由于右邻节点的已经小于半满状态，所有
				recol = top.pair[tpos - 1];
				fseek(index, recol.pos, SEEK_SET);
				fread(&left, 1, sizeof(left), index);
				cout << "左孩子：" << endl;
				cout << "left.count:" << left.count << endl;
				for (int i = 0; i <left.count; i++)
				{
					cout << left.pair[i].key << "   ";
				}
				cout << endl;
				if (left.count > MIN)
				{
					//左邻节点大于3的情形下
					transRecord(index, left, node, RIGHT, pos);
					if (node.type == NODE)
						changeParent(index, right, node.pair[0].pos, recon.pos);
					recon.key = node.pair[node.count - 1].key;
					recol.key = left.pair[left.count - 1].key;
					top.pair[tpos - 1] = recol;//重新更新父节点中的左分隔元素
					top.pair[tpos] = recon;//重新更新父节点中的分隔元素
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
					//左右节点中全部是小于或等于3个节点
					cout << "左孩子数目<Min" << endl;
					for (node.count--; pos < node.count; pos++)
						node.pair[pos] = node.pair[pos + 1];
					if (right.type == NODE)
					{
						for (pos = 0; pos < right.count; pos++)
							changeParent(index, left, right.pair[pos].pos, recon.pos);
					}
					mergeNode(index, node, right);//和右节点进行合并
					cout << "node和右孩子进行合并" << endl;
					fseek(index, recor.pos, SEEK_SET);
					fwrite(&right, 1, sizeof(right), index);
					cout << "right：" << endl;
					cout << "right.count:" << right.count << endl;
					for (int i = 0; i < right.count; i++)
					{
						cout << right.pair[i].key << "   ";
					}
					cout << endl;
					fflush(index);
					fseek(index, recon.pos, SEEK_SET);
					fwrite(&node, 1, sizeof(node), index);
					cout << "node：" << endl;
					cout << "node.count:" << node.count << endl;
					cout << "node记录" << endl;
					for (int i = 0; i < node.count; i++)
					{
						cout << node.pair[i].key << "   ";
					}
					cout << endl;
					fflush(index);
					//因为现在该节点的最后一个值已经变成原来的右邻节点中的最后一个。
					//所以当前节点的分隔元素变成右邻节点的分隔元素
					//之后删除掉原来的那个分隔元素就可以
					cout << "top:" << endl;
					for (int i = 0; i < top.count; i++)
					{
						cout << top.pair[i].key << "   ";
					}
					cout <<"将父节点tpos右边一个元素的指向合并后的节点，即node节点的位置" << endl;
					recor.pos = recon.pos;//把当前节点在父节点中分隔元素的pos，赋值给右邻点在父节点中分隔元素的pos
					top.pair[tpos + 1] = recor;//更新父节点中的右邻分隔元素
					fseek(index, recot.pos, SEEK_SET);
					fwrite(&top, 1, sizeof(top), index);
					fflush(index);
					//删除父节点中的当前元素的旧分隔元素，
					
					cout <<"将父节点中的旧的分隔元素:"<<recon.key<<"删除" << endl;
					cout << endl;
					delRecord(index, top, recon.key);
					
				}
			}
			else
			{
				//待删除元素所在的节点是其父节点中的第一个节点，即所在节点没有左节点
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
			right.pair[pos] = right.pair[pos - 1];//将待删除元素的前面的元素统统后移
		right.pair[0] = left.pair[left.count-1];//将左邻节点中的最后一个元素移动到当前节点中的第一个元素的位置。
		left.count--;
	}
	else
	{
		for (; pos < left.count - 1; pos++)
			left.pair[pos] = left.pair[pos + 1];//将待删除元素的后面的元素统统前移
		left.pair[pos] = right.pair[0];
		//因为该节点既然有右邻节点，说明当前节点至少处于半满状态。
		//又由于右邻节点中的个数大于半满个数，移动一个key之后，节点仍然会处于半满状态
		//将右邻节点中的第一个key移动到当前节点中，使得目前节点仍然是处于半满状态
		right.count--;
		//将待删除元素的后面的元素统统前移一个
		for (pos = 0; pos < right.count; pos++)
			right.pair[pos] = right.pair[pos + 1];
	}
}

void mergeNode(FILE *index, Node &left, Node &right)
{
	//既然要合并，再次情形下的隐含条件是：当前节点删除元素之后个数<=2,右邻节点的个数<=3,
	//所以两者之和的个数<=5,因此一定是合并之后，原来右邻节点中的元素全部移动到当前节点中。
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
	//当删除的元素是位于节点中的最后一个时，父节点中的分隔元素可能会发生变化，
	//假如发生变化时，那么父节点在其父节点中的分隔元素也是需要更新的。
	//这个过程是递归的
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
		cout << "叶子节点:  ";
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