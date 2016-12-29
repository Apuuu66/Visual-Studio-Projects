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

// 统计字符频度的临时结点
typedef struct {
	unsigned char uch;			// 以8bits为单元的无符号字符
	int weight;					// 每类字符出现频度
} HuffNode;
int lenNode;//结点个数
void GetNode(HuffNode *Node);//获取权值
void CreatHuffmanTree(HuffmanTree *HT, HuffNode *Node);//创建Huffman树
void GetHuffCode(HuffmanTree *HT);  //获取编码
void Select(HuffmanTree *HT, unsigned int n, int *s1, int *s2);//选择最小两个权值
void HuffmanEncode(char Code[][N], int weight[]);//编码
void HuffmanDecode(HuffmanTree *HT);//译码
void Menu(HuffmanTree *HT, int weight[], char Code[][N]);//菜单
void PrintCode(HuffmanTree *HT);//输出

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

//获取权值
void GetNode(HuffNode *Node)
{
	FILE *fp;
	unsigned char ch;
	int i, j;
	lenNode = 0;
	HuffNode node_temp;
	printf("正在读取文件，获得权重，请稍后。。。\n\n");
	// Sleep(1000);
	fp = fopen("speech.txt", "rb");
	if (fp == NULL || fgetc(fp) == feof)
	{
		printf("文件不存在或为空，请确认后重新运行程序！\n");
		exit(0);
	}
	rewind(fp);
	for (i = 1; i < N; ++i)
	{

		Node[i].weight = 0;
		Node[i].uch = (unsigned char)i; // 数组的256个下标与256种字符对应		
	}

	fread((char *)&ch, sizeof(char), 1, fp);		// 读入一个字符
	while (!feof(fp))
	{
		Node[ch].weight++;		// 统计下标对应字符的权重，利用数组的随机访问快速统计字符频度
		fread((char *)&ch, sizeof(char), 1, fp);		// 读入一个字符
	}
	fclose(fp);

	// 排序，将频度为零的放最后，剔除
	for (i = 1; i < N - 1; ++i)
		for (j = 1; j < N - i; ++j)
			if (Node[j].weight < Node[j + 1].weight)
			{
				node_temp = Node[j];
				Node[j] = Node[j + 1];
				Node[j + 1] = node_temp;
			}

	// 统计实际的字符种类（出现次数不为0）
	for (i = 1; i < N; ++i)
		if (Node[i].weight == 0)
			break;
	lenNode = i - 1;

	printf("\n\n获取权重成功！\n\n");
}
//创建Huffman树
void CreatHuffmanTree(HuffmanTree *HT, HuffNode *Node)
{
	int i, t;
	int s1, s2;

	printf("正在创建Huffman树，请稍后。。。\n\n");
	// Sleep(1000);
	//初始化所有非空结点
	for (i = 1; i <= lenNode; ++i)
	{
		HT[i].uch = Node[i].uch;
		HT[i].weight = Node[i].weight;
		HT[i].parent = 0;
		HT[i].lchild = HT[i].rchild = 0;
	}
	//初始化后lenNode-1个结点
	for (i = lenNode + 1; i <= 2 * lenNode - 1; ++i)
	{
		HT[i].parent = 0;
		HT[i].lchild = HT[i].rchild = 0;
	}

	for (t = lenNode + 1; t <= 2 * lenNode - 1; ++t)
	{
		Select(HT, t - 1, &s1, &s2);		// 选择最小的两个结点
		HT[t].weight = HT[s1].weight + HT[s2].weight;
		HT[s1].parent = HT[s2].parent = t;
		HT[t].lchild = s1;
		HT[t].rchild = s2;
	}
	printf("\n\n成功创建Huffman树！\n\n");
}

//选择最小两个权值
void Select(HuffmanTree *HT, unsigned int n, int *s1, int *s2)
{
	// 找最小
	unsigned int i;
	unsigned long min = ULONG_MAX;
	for (i = 1; i <= n; ++i)
		if (HT[i].parent == 0 && HT[i].weight < min)
		{
			min = HT[i].weight;
			*s1 = i;
		}
	HT[*s1].parent = 1;   // 标记此结点已被选中

						  // 找次小
	min = ULONG_MAX;
	for (i = 1; i <= n; ++i)
		if (HT[i].parent == 0 && HT[i].weight < min)
		{
			min = HT[i].weight;
			*s2 = i;
		}
}

// 生成哈夫曼编码
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
	printf("\n\n成功获取编码！\n\n");
}

//输出
void PrintCode(HuffmanTree *HT)
{
	int i;
	fflush(stdin);
	for (i = 1; i <= lenNode; i++)
		printf("%c\t%d\t%s\n", HT[i].uch, HT[i].weight, HT[i].code);
	//printf("%d\n", HT[i].weight);
}
