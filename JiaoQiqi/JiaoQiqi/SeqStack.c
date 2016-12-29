#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 10
typedef int DataType;
typedef struct seqStack {
	DataType Data[MAXSIZE];
	int top;
}SeqStack;

SeqStack *Init_SeqStack()
{
	SeqStack *s;
	s = (SeqStack *)malloc(sizeof(SeqStack));
	s->top = -1;
	return s;
}

int Empty_SeqStack(SeqStack *s)
{
	if (s->top == -1) {
		return 1;
	}
	return 0;
}

int Push_SeqStack(SeqStack *s, DataType x)
{
	if (s->top == MAXSIZE - 1) {
		return 0;
	}
	else {
		s->top++;
		s->Data[s->top] = x;
	}
	return 1;
}

int Pop_SeqStack(SeqStack *s, DataType *x)
{
	if (Empty_SeqStack(s))
		return 0;
	else
	{
		*x = s->Data[s->top];
		s->top--;
	}
	return 1;
}

DataType Top_SeqStack(SeqStack *s)
{
	if (Empty_SeqStack)
		return 0;
	else
	{
		return (s->Data[s->top]);
	}
}
void TestSeqStack() {
	int i;
	SeqStack *s;
	DataType x;
	do {
		printf("1:’ª-≥ı ºªØ\n");
		printf("2:’ª-≈–’ªø’\n");
		printf("3:’ª-»Î’ª\n");
		printf("4:’ª-≥ˆ’ª\n");
		printf("5:’ª-»°’ª∂•\n");
		printf("0:Quit\n");
		scanf("%d", &i);
		fflush(stdin);
		switch (i)
		{
		case 1:
			s = Init_SeqStack(); break;
		case 2:
			Empty_SeqStack(s); break;
		case 3:
			scanf("%d", &x);
			Push_SeqStack(s, x); break;
		case 4:
			Pop_SeqStack(s, &x); break;
		case 5:
			printf("%d\n", Top_SeqStack(s)); break;
		default:
			break;//
		}
		//getch();
		//system("cls");
	} while (i != 0);
}