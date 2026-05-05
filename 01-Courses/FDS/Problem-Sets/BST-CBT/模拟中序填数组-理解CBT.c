#include<stdio.h>
#include<stdlib.h>
#define MAX 1002
int com_binarytree[MAX];
void fill_inorder(int i, int N, int *temp);
int compare( const void* a, const void* b )
{
   return ( *(int*)a-*(int*)b ) ; 
}
int main()
{
	int N;
	scanf("%d",&N);
	int temp[MAX];
	temp[0] = 0;//intialize
	for ( int i = 1 ;i <= N ;i++ )
	scanf("%d",&temp[i]);
	qsort(temp+1, N, sizeof(int), compare);
	int k = 1;
	fill_inorder( k, N, temp);
	for ( int i = 1 ;i <= N ;i++ )
	{
		if ( i > 1 )
		printf(" ");
		printf("%d",com_binarytree[i]);
	}
}
int count = 1;
void fill_inorder(int k, int N, int * temp )
{
	if ( k > N ) return;
	fill_inorder(2*k, N, temp);
	com_binarytree[k] = temp[count++];
	fill_inorder(2*k+1, N, temp);
}
