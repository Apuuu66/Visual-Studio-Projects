#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 10
typedef int DataType;
typedef struct seqStack {
	DataType Data[MAXSIZE];
	int top;
}SeqStack;

SeqStack *Init_SeqStack(SeqStack *s)
{	
	s=(SeqStack*)malloc(sizeof(SeqStack));
	s->top == -1;
	return s;
}

int Empty_SeqStack(SeqStack *s)
{
	if (s->top==-1) {
		printf("Stakc is Empty!\n");
		return 1;
	}
	printf("Œ¥÷¥––"); 
	return 0;
}

void Push_SeqStack(SeqStack *s, DataType x)
{
	x = scanf("%d", &x);
	if (s->top == MAXSIZE - 1) {
		printf("The Stack is full\n");
	}
	else {
		s->top++;
		s->Data[s->top] = x;
	}
}

void Pop_SeqStack(SeqStack *s, DataType x)
{
	if (Empty_SeqStack(s))
		printf("The Stack is empty\n");
	else
	{
		x = s->Data[s->top];
		printf("%d", x);
		s->top--;
	}
}

DataType Top_SeqStack(SeqStack *s)
{
	if (Empty_SeqStack)
		return 0;
	else
	{
		printf("OK");
		return (s->Data[s->top]);
	}
}
void TestSeqStack() {
	SeqStack *s;
	int i;
	DataType x;
	do {
		printf("1:’ª-≥ı ºªØ\n");
		printf("2:’ª-≈–’ªø’\n");
		printf("3:’ª-»Î’ª\n");
		printf("4:’ª-≥ˆ’ª\n");
		printf("5:’ª-»°’ª∂•\n");
		printf("0:Quit\n");
		scanf("%d", &i);
		switch (i)
		{
		case 1:
			s=Init_SeqStack(s); break;
		case 2:
			Empty_SeqStack(s); break;
		case 3:
			Push_SeqStack(s, x); break;
		case 4:
			Pop_SeqStack(s, x); break;
		case 5:
			printf("%d\n", Top_SeqStack(s)); break;
		default:
			break;//
		}
		
	} while (i != 0);
}
