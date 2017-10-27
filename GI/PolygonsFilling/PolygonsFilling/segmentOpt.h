#pragma once

#ifndef SEGMENT_H_INCLUDED
#define SEGMENT_H_INCLUDED

#include "common.h"

/* (段表信息)
*/
struct Segment{
	int segState;					// 段状态 

	int segType;
	int segID;                    // 段号
	long segFirstPageNo;		  // 该段号 起始页
	long segPageNum;              // 段表占用了多少页
}; 
 

class SegmentOpt
{
public:
	SegmentOpt();
	~SegmentOpt();

	int getBit(unsigned long num, long pos);
	int setBit(unsigned long *num, long pos, int setValue);
	long allocateSegment(struct DbMetaHead *head, long reqSegmentNum);
	void recyOneSegment(struct DbMetaHead *head, long SegmentNo);
	void recyAllSegment(struct DbMetaHead *head);
};

#endif // SEGMENT_H_INCLUDED
