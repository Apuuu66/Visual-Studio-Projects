#include <stdio.h>
#define N 100
typedef struct Struct {
	int array[N];
	int length;
}Array;
void Test()
{
	int max, cmax;
	int i, j, len, temp;
	Array A;
	printf("输入数组长度(100以内)后回车输入各元素：");
	scanf("%d", &A.length);
	len = A.length;
	for (i = 0; i < len; i++) {
		scanf("%d", &A.array[i]);
	}
	if (len>2) {
		for (i = 0; i < len - 1; i++) {
			for (j = 0; j < len - i - 1; j++)
			{
				if (A.array[j] > A.array[j + 1])
				{
					temp = A.array[j];
					A.array[j] = A.array[j + 1];
					A.array[j + 1] = temp;
				}
			}
		}
	}
	printf("Sorted:\n");
	for (i = 0; i < len; i++) {
		printf("%d ", A.array[i]);
	}
	printf("\n");
	max = cmax = A.array[0];
	if (len > 2) {
		for (i = 0; i < len; i++) {
			if (cmax < max && cmax != max)
				cmax = max;
			if (max < A.array[i])
				max = A.array[i];
		}
		printf("max:%d  cmax:%d\n", max, cmax);
	}
	else if (len == 1) {
		printf("只有一个元素:%d\n", max);
	}
	else
		printf("error!!!\n");
}