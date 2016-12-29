#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <windows.h>

#define N 30
#define M 2*N-1

typedef struct {
	float weight;
	int parent, lchild, rchild;
}HuffmanTree;

typedef struct
{
	char ch;
	char rear;
	int count;
}leafWeight;
int GetWeight(leafWeight Weight[], float weight[]);//��ȡȨֵ
void CreatHuffmanTree(HuffmanTree HT[], float weight[]);//����Huffman��
void GetCode(HuffmanTree *HT, char Code[][N]);  //��ȡ����
void Select(HuffmanTree *HT, int n, int *min1, int *min2);//ѡ����С����Ȩֵ
void HuffmanEncode(char Code[][N], float weight[]);//����
void HuffmanDecode(HuffmanTree *HT);//����
void Menu(HuffmanTree *HT, float weight[], char Code[][N]);//�˵�
void PrintCode(float weight[], char Code[][N]);//���

int main()
{
	float weight[N];
	HuffmanTree HT[M];
	char Code[N][N];

	GetWeight(weight);
	CreatHuffmanTree(HT, weight);
	GetCode(HT, Code);

	Menu(HT, weight, Code);
	return 0;
}
//��ȡȨֵ
int GetWeight(leafWeight Weight[],float weight[])
{
	FILE *fp;
	char ch;
	int i,j, length;
	char code[N];
	length = 1;
	printf("���ڶ�ȡ�ļ������Ȩ�أ����Ժ󡣡���\n\n");
	Sleep(1000);
	fp = fopen("speech.txt", "rt");
	if (fp == NULL || fgetc(fp) == EOF)
	{
		printf("�ļ������ڻ�Ϊ�գ���ȷ�Ϻ��������г���\n");
		exit(0);
	}
	ch = fgetc(fp);
	Weight[length].ch = ch;
	Weight[length].count = 0;
	rewind(fp);
	while ((ch = fgetc(fp)) != EOF)
	{
		for (j = 1; j <= length; j++) {
			if (ch == Weight[j].ch) {
				Weight[j].count++;
				break;
			}
		}
		if (j - 1 == length) {
			length++;
			Weight[length].ch = ch;
			Weight[length].count = 1;
		}
	}

	fclose(fp);
	for (i = 1; i <= length; i++) {
		weight[i] = Weight[i].count;
	}
	printf("\n\n��ȡȨ�سɹ���\n\n");
	return length;
}
//����Huffman��
void CreatHuffmanTree(HuffmanTree *HT, float weight[])
{
	int i;
	int min1, min2;

	printf("���ڴ���Huffman�������Ժ󡣡���\n\n");
	Sleep(1000);
	for (i = 1; i < N; i++)//��ʼ��ǰn�����Ϊ���ڵ�
	{
		HT[i].weight = weight[i];

		HT[i].parent = 0;
		HT[i].lchild = HT[i].rchild = 0;
	}

	for (i = N; i < M; i++)//��ʼ����n+1���ս��
	{
		HT[i].weight = 0;
		HT[i].parent = 0;
		HT[i].lchild = HT[i].rchild = 0;
	}

	for (i = N; i < M; i++)//��N+1����㿪ʼ�����½ڵ�
	{
		Select(HT, i - 1, &min1, &min2);
		HT[min1].parent = HT[min2].parent = i;
		HT[i].lchild = min1;		HT[i].rchild = min2;
		HT[i].weight = HT[min1].weight + HT[min2].weight;
	}
	printf("\n\n�ɹ�����Huffman����\n\n");
}

//ѡ����С����Ȩֵ
void Select(HuffmanTree *HT, int n, int *min1, int *min2)
{
	int i, j = 1;
	int mark[M + 1];
	for (i = 1; i <= n; i++)
		if (HT[i].parent == 0)mark[j++] = i;
	*min1 = HT[mark[1]].weight < HT[mark[2]].weight ? mark[1] : mark[2];
	*min2 = HT[mark[1]].weight >= HT[mark[2]].weight ? mark[1] : mark[2];
	for (i = 3; i < j; i++)
	{
		if (HT[mark[i]].weight < HT[*min2].weight)
		{
			if (HT[mark[i]].weight <= HT[*min1].weight)
			{
				*min2 = *min1;
				*min1 = mark[i];
			}
			else
			{
				*min2 = mark[i];
			}
		}
	}
}

//��ȡ����
void GetCode(HuffmanTree *HT, char Code[][N])
{
	int i, child, parent, start;
	char code[N] = { "\0" };

	printf("���ڼ����ַ����룬���Ժ󡣡���\n\n");
	// Sleep(1000);
	for (i = 1; i < N; i++)
	{
		child = i;
		start = N - 1;
		while (HT[child].parent != 0)
		{
			parent = HT[child].parent;
			start--;
			if (HT[parent].lchild == child)
				code[start] = '0';
			else
				code[start] = '1';
			child = parent;
		}
		strcpy(Code[i], &code[start]);
	}
	printf("\n\n�ɹ���ȡ���룡\n\n");
}

//����
void HuffmanEncode(char Code[][N], float weight[])
{
	char ch;

	fflush(stdin);
	printf("��������Ҫ������ַ���:\n");
	while ((ch = getchar()) != '\n')
	{
		if (weight[ch - 'a' + 1] == 0)continue;
		if (ch >= 'a'&&ch <= 'z')
			printf("%s", Code[ch - 'a' + 1]);
		else if (ch >= 'A'&&ch <= 'Z')
			printf("%s", Code[ch - 'A' + 1]);
	}
	putchar('\n');
	system("pause");
}

//����
void HuffmanDecode(HuffmanTree *HT)
{
	char ch;
	int temp = M - 1;

	fflush(stdin);
	printf("��������Ҫ����Ķ����ƴ���:\n");
	while ((ch = getchar()) != '\n')
	{
		if (ch == '0')temp = HT[temp].lchild;
		else if (ch == '1')temp = HT[temp].rchild;
		if (HT[temp].lchild == 0)
		{
			printf("%c", temp + 'a' - 1);
			temp = M - 1;
		}
	}
	putchar('\n');
	system("pause");
}

//�˵�
void Menu(HuffmanTree *HT, float weight[], char Code[][N])
{
	char choose;


	while (1)
	{
		printf("\n\n\n");
		printf("\t\t\t* * * * * * * * * * * * * * * * *\n");
		printf("\t\t\t*      1�������ַ����б���      *\n");
		printf("\t\t\t*      2�����������н���      *\n");
		printf("\t\t\t*      3����ӡ�����ַ�����      *\n");
		printf("\t\t\t*      4���˳�                  *\n");
		printf("\t\t\t* * * * * * * * * * * * * * * * *\n");
		printf("\t\t\t��ѡ��  ");
		scanf("%c", &choose);
		fflush(stdin);
		printf("\n\n\n");
		switch (choose)
		{
		case '1':
			HuffmanEncode(Code, weight);
			break;
		case '2':
			HuffmanDecode(HT);
			break;
		case '3':
			PrintCode(weight, Code);
			break;
		case '4':
			exit(0);
		default:
			printf("������������������\n");
			continue;
		}
	}
}

//���
void PrintCode(float weight[], char Code[][N])
{
	int i;

	printf("\t\t\t�ַ�    Ȩ��    ����\n");
	for (i = 1; i < N; i++)
	{
		if (weight[i] == 0)continue;
		printf("\t\t\t%c:     %5.2f    %s\n", i + 64, weight[i], Code[i]);
	}
	printf("\n\n");
}