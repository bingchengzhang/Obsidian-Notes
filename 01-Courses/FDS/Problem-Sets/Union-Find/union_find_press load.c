#include<stdio.h>
#define MAX 10002
int Find(int c);//find c is of which set
void Union_size(int c1, int c2);//build union table
void judge_connect(int N);//count the number of components and judge if "The network is connected." or not
int set[MAX];//use array to imply union
int state[MAX]; 
//and: set[MAX] is set as 0 initially！！！(deault)
int main()
{
    int N;
    scanf("%d",&N);
    char op;
    for(int i = 1; i <= N; i++)
    set[i] = -1;
    //assume every element is root,attention that the initail index is 1
    int c1,c2;//set computers
    int j = 0; //j imply that the times of op==c
    while(1)
    {
        scanf(" %c",&op);
        if( op == 'S' ) // 'S'
        break;
        scanf("%d %d",&c1, &c2);
        if ( op == 'C' ) //case:find respectively set of c1 & c2 and check it 'C'
        {
            if( Find(c1) == Find(c2) )
            {
                state[j++] = 1;//printf("yes\n");
            }
            else
            state[j++] = 0;   //printf("no\n");
        }
        else//case:insert connection 'I' 
        {
            Union_size(c1, c2);
        }
    }
    for(int i=0;i < j;i++)
    {
    	if( state[i] )
    	printf("yes\n");
    	else
    	printf("no\n");
    }
    judge_connect(N);
}
void Union_size(int c1, int c2)//union_size 
{
    int root_c1 = Find(c1);
    int root_c2 = Find(c2);
    if( root_c1 == root_c2 ) return ;
    else if( set[root_c1] <= set[root_c2] )
    {
        set[root_c1] += set[root_c2];//union
        set[root_c2] = root_c1;
    }
    else
    {
        set[root_c2] += set[root_c1];
        set[root_c1] = root_c2;
    }
}
int Find(int c) //用压缩记忆已过路径
{
    int root;
    int trial,lead;
    for(root = c; set[root] > 0 ; root = set[root]);//find the root node
    for(trial = c;set[trial] > 0;trial = lead )
    {
        lead = set[trial];
        set[trial] = root;
    }//压缩
    return root;//return root node
}
void judge_connect(int N)
{
    int count1 = 0,count2 = 0;//the num of components
    for(int i = 1; i <= N; i++)
    {
        if( set[i] < count1 ) 
        {
            count1 = set[i];
        }
        if( set[i] < 0 )
        {
        	   count2++;
        }
    }
    if( -count1 == N )
    printf("The network is connected.");
    else
    printf("There are %d components.", count2);}//或者说去掉count1，只用count2来看，当根只有一个的时候说明全联通
}
