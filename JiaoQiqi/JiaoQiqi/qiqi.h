#pragma once
#ifndef _QIQI_H
#define _QIQI_H

#define X 2 //ѡ������ 
#define Y 5 //��ί���� 

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