#include<stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define X 2 //ѡ������ 
#define Y 5 //��ί���� 
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
			printf("\n�������%dλ��ί����%dѡ�ֵĴ�֣�\n", j + 1, i + 1);
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
	printf("\nȥ����߷�%f,ȥ����ͷ�%f\nѡ�����÷֣�%.2f\n", player[i].max, player[i].min, player[i].Ave);
	printf("\n\t����һ\t���ֶ�\t������\t������\t������\t�ܷ�\tƽ����\n");
	for (i = 0; i < X; i++)
	{
		printf("ѡ��%d:\t", i + 1);
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
		printf(" ��%d:\t", i + 1);
		for (j = 0; j < Y; j++)
		{
			printf("%.2f\t", player[i].Score[j]);
		}
		printf("%.2f\t%.2f\n", player[i].sum, player[i].Ave);
	}
}