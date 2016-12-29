#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define N 256

typedef struct {
	unsigned char uch;
	int weight;
	char *code;
	int parent, lchild, rchild;
}HuffmanTree;

// ͳ���ַ�Ƶ�ȵ���ʱ���
typedef struct {
	unsigned char uch;			// ��8bitsΪ��Ԫ���޷����ַ�
	int weight;					// ÿ���ַ�����Ƶ��
} HuffNode;
int lenNode;//������
void GetNode(HuffNode *Node);//��ȡȨֵ
void CreatHuffmanTree(HuffmanTree *HT, HuffNode *Node);//����Huffman��
void GetHuffCode(HuffmanTree *HT);  //��ȡ����
void Select(HuffmanTree *HT, unsigned int n, int *s1, int *s2);//ѡ����С����Ȩֵ
void HuffmanEncode(char Code[][N], int weight[]);//����
void HuffmanDecode(HuffmanTree *HT);//����
void Menu(HuffmanTree *HT, int weight[], char Code[][N]);//�˵�
void PrintCode(HuffmanTree *HT);//���

int main()
{
	HuffNode *Node;
	HuffmanTree *HT;

	Node = (HuffNode *)malloc(N * sizeof(HuffNode));
	GetNode(Node);
	HT = (HuffmanTree *)malloc((2 * lenNode) * sizeof(HuffmanTree));
	CreatHuffmanTree(HT, Node);
	GetHuffCode(HT);
	PrintCode(HT);
	//Menu(HT, weight, Code);
	return 0;
}

//��ȡȨֵ
void GetNode(HuffNode *Node)
{
	FILE *fp;
	unsigned char ch;
	int i, j;
	lenNode = 0;
	HuffNode node_temp;
	printf("���ڶ�ȡ�ļ������Ȩ�أ����Ժ󡣡���\n\n");
	// Sleep(1000);
	fp = fopen("speech.txt", "rb");
	if (fp == NULL || fgetc(fp) == feof)
	{
		printf("�ļ������ڻ�Ϊ�գ���ȷ�Ϻ��������г���\n");
		exit(0);
	}
	rewind(fp);
	for (i = 1; i < N; ++i)
	{

		Node[i].weight = 0;
		Node[i].uch = (unsigned char)i; // �����256���±���256���ַ���Ӧ		
	}

	fread((char *)&ch, sizeof(char), 1, fp);		// ����һ���ַ�
	while (!feof(fp))
	{
		Node[ch].weight++;		// ͳ���±��Ӧ�ַ���Ȩ�أ����������������ʿ���ͳ���ַ�Ƶ��
		fread((char *)&ch, sizeof(char), 1, fp);		// ����һ���ַ�
	}
	fclose(fp);

	// ���򣬽�Ƶ��Ϊ��ķ�����޳�
	for (i = 1; i < N - 1; ++i)
		for (j = 1; j < N - i; ++j)
			if (Node[j].weight < Node[j + 1].weight)
			{
				node_temp = Node[j];
				Node[j] = Node[j + 1];
				Node[j + 1] = node_temp;
			}

	// ͳ��ʵ�ʵ��ַ����ࣨ���ִ�����Ϊ0��
	for (i = 1; i < N; ++i)
		if (Node[i].weight == 0)
			break;
	lenNode = i - 1;

	printf("\n\n��ȡȨ�سɹ���\n\n");
}
//����Huffman��
void CreatHuffmanTree(HuffmanTree *HT, HuffNode *Node)
{
	int i, t;
	int s1, s2;

	printf("���ڴ���Huffman�������Ժ󡣡���\n\n");
	// Sleep(1000);
	//��ʼ�����зǿս��
	for (i = 1; i <= lenNode; ++i)
	{
		HT[i].uch = Node[i].uch;
		HT[i].weight = Node[i].weight;
		HT[i].parent = 0;
		HT[i].lchild = HT[i].rchild = 0;
	}
	//��ʼ����lenNode-1�����
	for (i = lenNode + 1; i <= 2 * lenNode - 1; ++i)
	{
		HT[i].parent = 0;
		HT[i].lchild = HT[i].rchild = 0;
	}

	for (t = lenNode + 1; t <= 2 * lenNode - 1; ++t)
	{
		Select(HT, t - 1, &s1, &s2);		// ѡ����С���������
		HT[t].weight = HT[s1].weight + HT[s2].weight;
		HT[s1].parent = HT[s2].parent = t;
		HT[t].lchild = s1;
		HT[t].rchild = s2;
	}
	printf("\n\n�ɹ�����Huffman����\n\n");
}

//ѡ����С����Ȩֵ
void Select(HuffmanTree *HT, unsigned int n, int *s1, int *s2)
{
	// ����С
	unsigned int i;
	unsigned long min = ULONG_MAX;
	for (i = 1; i <= n; ++i)
		if (HT[i].parent == 0 && HT[i].weight < min)
		{
			min = HT[i].weight;
			*s1 = i;
		}
	HT[*s1].parent = 1;   // ��Ǵ˽���ѱ�ѡ��

						  // �Ҵ�С
	min = ULONG_MAX;
	for (i = 1; i <= n; ++i)
		if (HT[i].parent == 0 && HT[i].weight < min)
		{
			min = HT[i].weight;
			*s2 = i;
		}
}

// ���ɹ���������
void GetHuffCode(HuffmanTree *HT)
{
	int start;
	int n, c, p, i;
	char *cd;
	n = 2 * lenNode - 1;
	cd = (char *)malloc(n * sizeof(char));
	cd[n - 1] = '\0';
	for (i = 1; i <= lenNode; i++) {
		start = n - 1;
		c = i; p = HT[i].parent;
		while (p != 0) {
			--start;
			if (HT[p].lchild == c)
				cd[start] = '0';
			else
				cd[start] = '1';
			c = p;
			p = HT[p].parent;

		}
		HT[i].code = (char *)malloc((n - start) * sizeof(char));
		strcpy(HT[i].code, &cd[start]);
	}
	free(cd);
	printf("\n\n�ɹ���ȡ���룡\n\n");
}

//���
void PrintCode(HuffmanTree *HT)
{
	int i;
	fflush(stdin);
	for (i = 1; i <= lenNode; i++)
		printf("%c\t%d\t%s\n", HT[i].uch, HT[i].weight, HT[i].code);
	//printf("%d\n", HT[i].weight);
}
