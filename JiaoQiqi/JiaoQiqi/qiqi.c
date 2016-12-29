#include<stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define X 2 //选手人数 
#define Y 5 //评委人数 
typedef struct play
{
	float Score[Y];
	float Ave;
	float max;
	float min;
	float sum;
}Player;
void Vote()
{
	int i, j;
	Player player[X], temp;
	for (i = 0; i < X; i++)
	{
		player[i].max = 0;
		player[i].min = 1000;
		player[i].sum = 0;
		for (j = 0; j < Y; j++)
		{
			printf("\n请输入第%d位评委给予%d选手的打分：\n", j + 1, i + 1);
			scanf("%f", &player[i].Score[j]);
			player[i].sum += player[i].Score[j];
			if (player[i].Score[j] > player[i].max)
				player[i].max = player[i].Score[j];
			if (player[i].min > player[i].Score[j])
				player[i].min = player[i].Score[j];
		}
		player[i].sum -= player[i].min + player[i].max;

		player[i].Ave = player[i].sum / (Y - 2);
	}
	printf("\n去掉最高分%f,去掉最低分%f\n选手最后得分：%.2f\n", player[i].max, player[i].min, player[i].Ave);
	printf("\n\t评分一\t评分二\t评分三\t评分四\t评分五\t总分\t平均分\n");
	for (i = 0; i < X; i++)
	{
		printf("选手%d:\t", i + 1);
		for (j = 0; j < Y; j++)
		{
			printf("%.2f\t", player[i].Score[j]);
		}
		printf("%.2f\t%.2f\n", player[i].sum, player[i].Ave);
	}
	for (i = 0; i < X - 1; i++)
		for (j = 0; j < X - i - 1; j++)
		{
			if (player[j].Ave<player[j + 1].Ave)
			{
				temp = player[j];
				player[j] = player[j + 1];
				player[j + 1] = temp;
			}
		}
	for (i = 0; i < X; i++)
	{
		printf(" 第%d:\t", i + 1);
		for (j = 0; j < Y; j++)
		{
			printf("%.2f\t", player[i].Score[j]);
		}
		printf("%.2f\t%.2f\n", player[i].sum, player[i].Ave);
	}
}