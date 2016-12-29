#pragma once
#ifndef _QIQI_H
#define _QIQI_H

#define X 2 //选手人数 
#define Y 5 //评委人数 

typedef struct play
{
	float Score[Y];
	float Ave;
	float max;
	float min;
	float sum;
}Player;

void Vote();
#endif // !_QIQI_H