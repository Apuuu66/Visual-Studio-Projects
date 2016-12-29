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

// ͳ���ַ�Ƶ�ȵ���ʱ���
typedef struct {
	unsigned char uch;			// ��8bitsΪ��Ԫ���޷����ַ�
	long int weight;					// ÿ���ַ�����Ƶ��
} HuffNode;
int lenNode;//������
unsigned long fileLen;//�ļ�����
void GetNode(char *fileName, HuffNode *Node);//��ȡȨֵ
void CreatHuffmanTree(HuffmanTree *HT, HuffNode *Node);//����Huffman��
void GetHuffCode(HuffmanTree *HT);  //��ȡ����
void Select(HuffmanTree *HT, unsigned int n, int *s1, int *s2);//ѡ����С����Ȩֵ
void HuffmanEncode();//����
void HuffmanDecode(HuffmanTree *HT);//����
int CompressHuffCode(char *open_file, char *zip_Name, HuffmanTree *HT);//ѹ��Huffman����
int extractHuffCode(char *zip_Name, char *extract_file);//��ѹHuffman����
void Menu(HuffmanTree *HT, int weight[], char Code[][N]);//�˵�
void PrintCode(HuffmanTree *HT);//���

int main()
{
	HuffNode *Node;
	HuffmanTree *HT;
	char open_file[30];
	char zip_Name[30];
	char fileName[30];
	char extract_file[30];
	printf("�������ļ���(*.souce)��");
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

//��ȡȨֵ
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
		printf("�ļ������ڻ�Ϊ�գ���ȷ�Ϻ��������г���\n");
		exit(1);
	}
	printf("���ڶ�ȡ�ļ������Ȩ�أ����Ժ󡣡���\n\n");
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
		fileLen++;
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
	fp_zip = fopen(zip_Name, "wb");					// ��ѹ�������ɵ��ļ�
	fwrite((char *)&lenNode, sizeof(unsigned int), 1, fp_zip);		// д���ַ�����
	for (i = 1; i < lenNode; ++i)
	{
		fwrite((char *)&HT[i].uch, sizeof(unsigned char), 1, fp_zip);			// д���ַ��������򣬶�����˳�򲻱䣩
		fwrite((char *)&HT[i].weight, sizeof(unsigned long), 1, fp_zip);		// д���ַ���ӦȨ��
	}

	// �������ַ���Ȩ����Ϣ����д���ļ����Ⱥ��ַ�����
	fwrite((char *)&fileLen, sizeof(unsigned long), 1, fp_zip);		// д���ļ�����
	fp_open = fopen(open_file, "rb");		// �Զ�������ʽ�򿪴�ѹ�����ļ�
	if (fp_open == NULL || fgetc(fp_open) == EOF)
	{
		printf("��Ŀ���ļ�ʧ�ܣ�\n");
		return 0;
	}
	fread((char *)&Charactor, sizeof(unsigned char), 1, fp_open);     // ÿ�ζ�ȡ8bits
	while (!feof(fp_open))
	{
		// ƥ���ַ���Ӧ����
		for (i = 1; i <= lenNode; ++i)
			if (Charactor == HT[i].uch)
				strcat(code_buf, HT[i].code);

		// ��8λ��һ���ֽڳ��ȣ�Ϊ����Ԫ
		while (strlen(code_buf) >= 8)
		{
			Charactor = '\0';		// ����ַ��ݴ�ռ䣬��Ϊ�ݴ��ַ���Ӧ����
			for (i = 0; i < 8; ++i)
			{
				Charactor <<= 1;		// ����һλ��Ϊ��һ��bit�ڳ�λ��
				if (code_buf[i] == '1')
					Charactor |= 1;		// ������Ϊ"1"��ͨ���������������ӵ��ֽڵ����λ
			}
			fwrite((char *)&Charactor, sizeof(unsigned char), 1, fp_zip);		// ���ֽڶ�Ӧ��������ļ�
			strcpy(code_buf, code_buf + 8);		// ���뻺��ȥ���Ѵ����ǰ��λ
		}
		fread((char *)&Charactor, sizeof(unsigned char), 1, fp_open);     // ÿ�ζ�ȡ8bits
	}
	// ���������8bits����
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
		Charactor <<= 8 - code_len;       // �������ֶδ�β���Ƶ��ֽڵĸ�λ
		fwrite((char *)&Charactor, sizeof(unsigned char), 1, fp_zip);       // �������һ���ֽ�
	}

	// �ر��ļ�
	fclose(fp_open);
	fclose(fp_zip);

	// �ͷ��ڴ�
	for (i = 1; i < lenNode; ++i)
		free(HT[i].code);
	free(HT);

}//compress

int extractHuffCode(char *zip_Name, char *extract_file)
{
	unsigned int i;
	unsigned long file_len;
	unsigned long writen_len = 0;		// �����ļ�д�볤��
	FILE *fp_zip;
	FILE *fp_out;
	unsigned int char_kinds;		// �洢�ַ�����
	unsigned int node_num;
	HuffmanTree *HT;
	unsigned char code_temp;		// �ݴ�8bits����
	unsigned int root;		// ������ڵ���������ƥ�����ʹ��

	fp_zip = fopen(zip_Name, "rb");		// �Զ����Ʒ�ʽ��ѹ���ļ�
										// �ж������ļ��Ƿ����
	if (fp_zip == NULL)
		return -1;

	// ��ȡѹ���ļ�ǰ�˵��ַ�����Ӧ���룬�����ؽ���������
	fread((char *)&char_kinds, sizeof(unsigned int), 1, fp_zip);     // ��ȡ�ַ�������

	node_num = 2 * char_kinds - 1;		// �����ַ������������㽨������������������ 
	HT = (HuffmanTree *)malloc(node_num * sizeof(HuffmanTree));		// ��̬��������������ռ�
																	// ��ȡ�ַ�����ӦȨ�أ�������������ڵ�
	for (i = 0; i < char_kinds; ++i)
	{
		fread((char *)&HT[i].uch, sizeof(unsigned char), 1, fp_zip);		// �����ַ�
		fread((char *)&HT[i].weight, sizeof(unsigned long), 1, fp_zip);	// �����ַ���ӦȨ��
		HT[i].parent = 0;
	}
	// ��ʼ����node_num-char_kins������parent
	for (; i < node_num; ++i)
		HT[i].parent = 0;

	CreateTree(HT, char_kinds, node_num);		// �ؽ�������������ѹ��ʱ��һ�£�

												// �����ַ���Ȩ����Ϣ�������Ŷ�ȡ�ļ����Ⱥͱ��룬���н���
	fread((char *)&file_len, sizeof(unsigned long), 1, fp_zip);	// �����ļ�����
	fp_out = fopen(extract_file, "wb");		// ��ѹ�������ɵ��ļ�
	root = node_num - 1;
	while (1)
	{
		fread((char *)&code_temp, sizeof(unsigned char), 1, fp_zip);		// ��ȡһ���ַ����ȵı���

																			// �����ȡ��һ���ַ����ȵı��루ͨ��Ϊ8λ��
		for (i = 0; i < 8; ++i)
		{
			// �ɸ�����ֱ��Ҷ�ڵ�����ƥ������Ӧ�ַ�
			if (code_temp & 128)
				root = HT[root].rchild;
			else
				root = HT[root].lchild;

			if (root < char_kinds)
			{
				fwrite((char *)&HT[root].uch, sizeof(unsigned char), 1, fp_out);
				++writen_len;
				if (writen_len == file_len) break;		// �����ļ����ȣ������ڲ�ѭ��
				root = node_num - 1;        // ��λΪ��������ƥ����һ���ַ�
			}
			code_temp <<= 1;		// �����뻺�����һλ�Ƶ����λ����ƥ��
		}
		if (writen_len == file_len) break;		// �����ļ����ȣ��������ѭ��
	}

	// �ر��ļ�
	fclose(fp_zip);
	fclose(fp_out);

	// �ͷ��ڴ�
	free(HT);
	return 1;
}//extract()
