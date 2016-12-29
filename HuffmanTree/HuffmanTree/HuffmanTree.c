#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define N 256

typedef struct {
	unsigned char uch;
	long int weight;
	char *code;
	int parent, lchild, rchild;
}HuffmanTree;

// 统计字符频度的临时结点
typedef struct {
	unsigned char uch;			// 以8bits为单元的无符号字符
	long int weight;					// 每类字符出现频度
} HuffNode;
int lenNode;//结点个数
unsigned long fileLen;//文件长度
void GetNode(char *fileName, HuffNode *Node);//获取权值
void CreatHuffmanTree(HuffmanTree *HT, HuffNode *Node);//创建Huffman树
void GetHuffCode(HuffmanTree *HT);  //获取编码
void Select(HuffmanTree *HT, unsigned int n, int *s1, int *s2);//选择最小两个权值
void HuffmanEncode();//编码
void HuffmanDecode(HuffmanTree *HT);//译码
int CompressHuffCode(char *open_file, char *zip_Name, HuffmanTree *HT);//压缩Huffman编码
int extractHuffCode(char *zip_Name, char *extract_file);//解压Huffman编码
void Menu(HuffmanTree *HT, int weight[], char Code[][N]);//菜单
void PrintCode(HuffmanTree *HT);//输出

int main()
{
	HuffNode *Node;
	HuffmanTree *HT;
	char open_file[30];
	char zip_Name[30];
	char fileName[30];
	char extract_file[30];
	printf("请输入文件名(*.souce)：");
	fflush(stdin);
	gets(fileName);
	Node = (HuffNode *)malloc(N * sizeof(HuffNode));
	GetNode(fileName, Node);
	HT = (HuffmanTree *)malloc((2 * lenNode) * sizeof(HuffmanTree));
	CreatHuffmanTree(HT, Node);
	GetHuffCode(HT);
	PrintCode(HT);
	gets(zip_Name);
	CompressHuffCode(fileName, zip_Name, HT);
	//gets(extract_file);
	//extractHuffCode(zip_Name, extract_file);

	//Menu(HT, weight, Code);
	return 0;
}

//获取权值
void GetNode(char *fileName, HuffNode *Node)
{
	FILE *fp;
	unsigned char ch;
	int i, j;
	lenNode = 0;
	fileLen = 0;
	HuffNode node_temp;

	fp = fopen(fileName, "rb");
	// Sleep(1000);

	//fp = fopen("speech.txt", "rb");
	if (fp == NULL || fgetc(fp) == EOF)
	{
		printf("文件不存在或为空，请确认后重新运行程序！\n");
		exit(1);
	}
	printf("正在读取文件，获得权重，请稍后。。。\n\n");
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
		fileLen++;
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
void HuffmanEncode()
{

}

int CompressHuffCode(char *open_file, char *zip_Name, HuffmanTree *HT) {
	FILE *fp_open;
	FILE *fp_zip;
	int i;
	unsigned int code_len;
	unsigned char Charactor;
	char code_buf[256] = "\0";
	fp_zip = fopen(zip_Name, "wb");					// 打开压缩后将生成的文件
	fwrite((char *)&lenNode, sizeof(unsigned int), 1, fp_zip);		// 写入字符种类
	for (i = 1; i < lenNode; ++i)
	{
		fwrite((char *)&HT[i].uch, sizeof(unsigned char), 1, fp_zip);			// 写入字符（已排序，读出后顺序不变）
		fwrite((char *)&HT[i].weight, sizeof(unsigned long), 1, fp_zip);		// 写入字符对应权重
	}

	// 紧接着字符和权重信息后面写入文件长度和字符编码
	fwrite((char *)&fileLen, sizeof(unsigned long), 1, fp_zip);		// 写入文件长度
	fp_open = fopen(open_file, "rb");		// 以二进制形式打开待压缩的文件
	if (fp_open == NULL || fgetc(fp_open) == EOF)
	{
		printf("打开目标文件失败！\n");
		return 0;
	}
	fread((char *)&Charactor, sizeof(unsigned char), 1, fp_open);     // 每次读取8bits
	while (!feof(fp_open))
	{
		// 匹配字符对应编码
		for (i = 1; i <= lenNode; ++i)
			if (Charactor == HT[i].uch)
				strcat(code_buf, HT[i].code);

		// 以8位（一个字节长度）为处理单元
		while (strlen(code_buf) >= 8)
		{
			Charactor = '\0';		// 清空字符暂存空间，改为暂存字符对应编码
			for (i = 0; i < 8; ++i)
			{
				Charactor <<= 1;		// 左移一位，为下一个bit腾出位置
				if (code_buf[i] == '1')
					Charactor |= 1;		// 当编码为"1"，通过或操作符将其添加到字节的最低位
			}
			fwrite((char *)&Charactor, sizeof(unsigned char), 1, fp_zip);		// 将字节对应编码存入文件
			strcpy(code_buf, code_buf + 8);		// 编码缓存去除已处理的前八位
		}
		fread((char *)&Charactor, sizeof(unsigned char), 1, fp_open);     // 每次读取8bits
	}
	// 处理最后不足8bits编码
	code_len = strlen(code_buf);
	if (code_len > 0)
	{
		Charactor = '\0';
		for (i = 0; i < code_len; ++i)
		{
			Charactor <<= 1;
			if (code_buf[i] == '1')
				Charactor |= 1;
		}
		Charactor <<= 8 - code_len;       // 将编码字段从尾部移到字节的高位
		fwrite((char *)&Charactor, sizeof(unsigned char), 1, fp_zip);       // 存入最后一个字节
	}

	// 关闭文件
	fclose(fp_open);
	fclose(fp_zip);

	// 释放内存
	for (i = 1; i < lenNode; ++i)
		free(HT[i].code);
	free(HT);

}//compress

int extractHuffCode(char *zip_Name, char *extract_file)
{
	unsigned int i;
	unsigned long file_len;
	unsigned long writen_len = 0;		// 控制文件写入长度
	FILE *fp_zip;
	FILE *fp_out;
	unsigned int char_kinds;		// 存储字符种类
	unsigned int node_num;
	HuffmanTree *HT;
	unsigned char code_temp;		// 暂存8bits编码
	unsigned int root;		// 保存根节点索引，供匹配编码使用

	fp_zip = fopen(zip_Name, "rb");		// 以二进制方式打开压缩文件
										// 判断输入文件是否存在
	if (fp_zip == NULL)
		return -1;

	// 读取压缩文件前端的字符及对应编码，用于重建哈夫曼树
	fread((char *)&char_kinds, sizeof(unsigned int), 1, fp_zip);     // 读取字符种类数

	node_num = 2 * char_kinds - 1;		// 根据字符种类数，计算建立哈夫曼树所需结点数 
	HT = (HuffmanTree *)malloc(node_num * sizeof(HuffmanTree));		// 动态分配哈夫曼树结点空间
																	// 读取字符及对应权重，存入哈夫曼树节点
	for (i = 0; i < char_kinds; ++i)
	{
		fread((char *)&HT[i].uch, sizeof(unsigned char), 1, fp_zip);		// 读入字符
		fread((char *)&HT[i].weight, sizeof(unsigned long), 1, fp_zip);	// 读入字符对应权重
		HT[i].parent = 0;
	}
	// 初始化后node_num-char_kins个结点的parent
	for (; i < node_num; ++i)
		HT[i].parent = 0;

	CreateTree(HT, char_kinds, node_num);		// 重建哈夫曼树（与压缩时的一致）

												// 读完字符和权重信息，紧接着读取文件长度和编码，进行解码
	fread((char *)&file_len, sizeof(unsigned long), 1, fp_zip);	// 读入文件长度
	fp_out = fopen(extract_file, "wb");		// 打开压缩后将生成的文件
	root = node_num - 1;
	while (1)
	{
		fread((char *)&code_temp, sizeof(unsigned char), 1, fp_zip);		// 读取一个字符长度的编码

																			// 处理读取的一个字符长度的编码（通常为8位）
		for (i = 0; i < 8; ++i)
		{
			// 由根向下直至叶节点正向匹配编码对应字符
			if (code_temp & 128)
				root = HT[root].rchild;
			else
				root = HT[root].lchild;

			if (root < char_kinds)
			{
				fwrite((char *)&HT[root].uch, sizeof(unsigned char), 1, fp_out);
				++writen_len;
				if (writen_len == file_len) break;		// 控制文件长度，跳出内层循环
				root = node_num - 1;        // 复位为根索引，匹配下一个字符
			}
			code_temp <<= 1;		// 将编码缓存的下一位移到最高位，供匹配
		}
		if (writen_len == file_len) break;		// 控制文件长度，跳出外层循环
	}

	// 关闭文件
	fclose(fp_zip);
	fclose(fp_out);

	// 释放内存
	free(HT);
	return 1;
}//extract()
