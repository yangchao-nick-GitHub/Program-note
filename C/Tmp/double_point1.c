#include <stdio.h>


void func1()
{
	int arr[][3] = {{1,2,3},{4,5,6}};
	int (*p)[3] = arr;
	printf("arr[0][0]=%p\n",&arr[0][0]);
	printf("arr[0][1]=%p\n",&arr[0][1]);
	printf("arr[0][2]=%p\n",&arr[0][2]);
	printf("arr[1][0]=%p\n",&arr[1][0]);
	printf("arr[1][1]=%p\n",&arr[1][1]);
	printf("arr[1][2]=%p\n",&arr[1][2]);
	printf("%p\n",*(p+1));//arr[1]数组地址 基类型变成int长度
	printf("%p\n",p+1);//arr[1]数组地址	基类型为arr[3]长度
	printf("%p\n",*(p+1)+1);//arr[1][1]数组地址 基类型变成int长度

	printf("arr=%p\n",arr);//基类型为arr[3]长度
	printf("arr+1=%p\n",arr+1);
	printf("*arr=%p\n",*arr);
	printf("(*arr)+1=%p\n",(*arr)+1);//基类型为arr[3]长度
}

void average1(int *arr,int num)
{
	int average = 0;
	int count = 0;
	int *ret = arr;
	int *end = ret+num-1;
	fprintf(stdout,"arr=%p\n",arr);
	fprintf(stdout,"end=%p\n",end);
	while(ret<=end){
		fprintf(stdout,"*(ret)=%d\n",*(ret));
		count = count + *(ret);
		ret++;		
	}
	fprintf(stdout,"count=%d\n",count);
	average = count/num;
	fprintf(stdout,"average=%d\n",average);

}

void average2(int (*arr)[3],int num)
{
	int average = 0;
	int count = 0;
	int *ret = *arr;
	int *end = ret+num-1;
	fprintf(stdout,"arr=%p\n",arr);
	fprintf(stdout,"end=%p\n",end);
	while(ret<=end){
		fprintf(stdout,"*(ret)=%d\n",*(ret));
		count = count + *(ret);
		ret++;		
	}
	fprintf(stdout,"count=%d\n",count);
	average = count/num;
	fprintf(stdout,"average=%d\n",average);

}


void func2()
{
	int arr[][3] = {{1,2,3},{4,5,6}};
	printf("arr[0][0]=%p\n",&arr[0][0]);
	printf("arr[0][1]=%p\n",&arr[0][1]);
	printf("arr[0][2]=%p\n",&arr[0][2]);
	printf("arr[1][0]=%p\n",&arr[1][0]);
	printf("arr[1][1]=%p\n",&arr[1][1]);
	printf("arr[1][2]=%p\n",&arr[1][2]);
	printf("arr=%p\n",arr);
	printf("arr+1=%p\n",arr+1);
	printf("*arr=%p\n",*arr);
	printf("(*arr)+1=%p\n",(*arr)+1);
	average1(*arr,6);//将数组地址 基类型变成int长度
	average2(arr,6);//基类型为arr[3]长度:12个字节
}




int main()
{
	
	func2();
	return 0;
}
