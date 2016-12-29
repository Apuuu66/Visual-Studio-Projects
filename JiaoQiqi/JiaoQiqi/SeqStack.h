#pragma once
#ifndef _SEQSTACK_H
#define _SEQSTACK_H

#define MAXSIZE 10
typedef int DataType;
typedef struct seqStack {
	DataType Data[MAXSIZE];
	int top;
}SeqStack;

SeqStack *Init_SeqStack();

int Empty_SeqStack(SeqStack *s);

int Push_SeqStack(SeqStack *s, DataType x);

int Pop_SeqStack(SeqStack *s, DataType *x);

DataType Top_SeqStack(SeqStack *s);

void TestSeqStack();
#endif // !_SEQSTACK_H
