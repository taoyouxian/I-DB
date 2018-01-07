#pragma once

#ifndef SEGMENT_H_INCLUDED
#define SEGMENT_H_INCLUDED

#include "common.h"

/* (�α���Ϣ)
*/
struct Segment{
	int segState;					// ��״̬ 

	int segType;
	int segID;                    // �κ�
	long segFirstPageNo;		  // �öκ� ��ʼҳ
	long segPageNum;              // �α�ռ���˶���ҳ
}; 
 

class SegmentOpt
{
public:
	SegmentOpt();
	~SegmentOpt();

	int getBit(unsigned long num, long pos);
	int setBit(unsigned long *num, long pos, int setValue);
	long allocateSegment(struct dbSysHead *head, long reqSegmentNum);
	void recyOneSegment(struct dbSysHead *head, long SegmentNo);
	void recyAllSegment(struct dbSysHead *head);
};

#endif // SEGMENT_H_INCLUDED
