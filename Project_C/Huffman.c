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
int GetWeight(leafWeight Weight[], float weight[]);//获取权值
void CreatHuffmanTree(HuffmanTree HT[], float weight[]);//创建Huffman树
void GetCode(HuffmanTree *HT, char Code[][N]);  //获取编码
void Select(HuffmanTree *HT, int n, int *min1, int *min2);//选择最小两个权值
void HuffmanEncode(char Code[][N], float weight[]);//编码
void HuffmanDecode(HuffmanTree *HT);//译码
void Menu(HuffmanTree *HT, float weight[], char Code[][N]);//菜单
void PrintCode(float weight[], char Code[][N]);//输出

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
//获取权值
int GetWeight(leafWeight Weight[],float weight[])
{
	FILE *fp;
	char ch;
	int i,j, length;
	char code[N];
	length = 1;
	printf("正在读取文件，获得权重，请稍后。。。\n\n");
	Sleep(1000);
	fp = fopen("speech.txt", "rt");
	if (fp == NULL || fgetc(fp) == EOF)
	{
		printf("文件不存在或为空，请确认后重新运行程序！\n");
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
	printf("\n\n获取权重成功！\n\n");
	return length;
}
//创建Huffman树
void CreatHuffmanTree(HuffmanTree *HT, float weight[])
{
	int i;
	int min1, min2;

	printf("正在创建Huffman树，请稍后。。。\n\n");
	Sleep(1000);
	for (i = 1; i < N; i++)//初始化前n个结点为根节点
	{
		HT[i].weight = weight[i];

		HT[i].parent = 0;
		HT[i].lchild = HT[i].rchild = 0;
	}

	for (i = N; i < M; i++)//初始化后n+1个空结点
	{
		HT[i].weight = 0;
		HT[i].parent = 0;
		HT[i].lchild = HT[i].rchild = 0;
	}

	for (i = N; i < M; i++)//从N+1个结点开始构造新节点
	{
		Select(HT, i - 1, &min1, &min2);
		HT[min1].parent = HT[min2].parent = i;
		HT[i].lchild = min1;		HT[i].rchild = min2;
		HT[i].weight = HT[min1].weight + HT[min2].weight;
	}
	printf("\n\n成功创建Huffman树！\n\n");
}

//选择最小两个权值
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

//获取编码
void GetCode(HuffmanTree *HT, char Code[][N])
{
	int i, child, parent, start;
	char code[N] = { "\0" };

	printf("正在计算字符编码，请稍后。。。\n\n");
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
	printf("\n\n成功获取编码！\n\n");
}

//编码
void HuffmanEncode(char Code[][N], float weight[])
{
	char ch;

	fflush(stdin);
	printf("请输入需要编码的字符串:\n");
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

//译码
void HuffmanDecode(HuffmanTree *HT)
{
	char ch;
	int temp = M - 1;

	fflush(stdin);
	printf("请输入需要解码的二进制代码:\n");
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

//菜单
void Menu(HuffmanTree *HT, float weight[], char Code[][N])
{
	char choose;


	while (1)
	{
		printf("\n\n\n");
		printf("\t\t\t* * * * * * * * * * * * * * * * *\n");
		printf("\t\t\t*      1：输入字符进行编码      *\n");
		printf("\t\t\t*      2：输入编码进行解码      *\n");
		printf("\t\t\t*      3：打印所有字符编码      *\n");
		printf("\t\t\t*      4：退出                  *\n");
		printf("\t\t\t* * * * * * * * * * * * * * * * *\n");
		printf("\t\t\t请选择：  ");
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
			printf("输入有误，请重新输入\n");
			continue;
		}
	}
}

//输出
void PrintCode(float weight[], char Code[][N])
{
	int i;

	printf("\t\t\t字符    权重    编码\n");
	for (i = 1; i < N; i++)
	{
		if (weight[i] == 0)continue;
		printf("\t\t\t%c:     %5.2f    %s\n", i + 64, weight[i], Code[i]);
	}
	printf("\n\n");
}
