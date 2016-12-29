#pragma once
#ifndef _SEQSTACK_H
#define _SEQSTACK_H

#define MAXSIZE 20
typedef int DataType;
typedef struct seqStack {
	DataType Data[MAXSIZE];
	int top;
}SeqStack;

SeqStack *Init_SeqStack(SeqStack *s);

void Empty_SeqStack(SeqStack *s);

void Push_SeqStack(SeqStack *s, DataType x);

int Pop_SeqStack(SeqStack *s, DataType *x);

DataType Top_SeqStack(SeqStack *s);

void TestSeqStack();
#endif // !_SEQSTACK_H
