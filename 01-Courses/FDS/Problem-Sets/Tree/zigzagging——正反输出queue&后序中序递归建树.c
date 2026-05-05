#include <stdio.h>
#include <stdlib.h>
#define MAX 2000
typedef struct tree_node
{
	int element;
	struct tree_node* left;
	struct tree_node* right;
}tree_node;
tree_node *treebuild(int *inorder, int *postorder, int in_left, int in_right, int post_left, int post_right);
void print_zigzag_order(tree_node *root,int N);
int main()
{
	int N,i = 0;
	scanf("%d",&N);//total nodes
	int inorder[N],postorder[N];

	while (i < N) 
    {
      scanf("%d",&inorder[i]);
      i++;
    }
	i = 0;
	while (i < N)
    {
      scanf("%d",&postorder[i]);
      i++;
    }
	struct tree_node *root = treebuild(inorder,postorder,0,N-1,0,N-1);
	print_zigzag_order(root,N);
	return 0;
}
//set up tree
struct tree_node *treebuild(int *inorder,int *postorder,int in_left,int in_right,int post_left,int post_right)
{
	if ( in_right < in_left )
	{
		return NULL;
	}//should before malloc__prevent memory leak
	tree_node* node = (tree_node *)malloc(sizeof(struct tree_node));
	node->left = NULL;
	node->right = NULL;
	int num_left = 0;
	for (int i = in_left;i <= in_right ;i++,num_left++ )
	{
		if ( inorder[i] == postorder[post_right] )
		break;
	}
	node->element = postorder[post_right];
	node->left = treebuild(inorder,postorder,in_left,in_left+num_left-1,post_left,post_left+num_left-1);//left tree
	node->right = treebuild(inorder,postorder,in_left+num_left+1,in_right,post_left+num_left,post_right-1);//right tree
	return node;//return 'root'
}
void print_zigzag_order(tree_node *node,int N)
{
	if ( !node ) return;
	struct tree_node* queue[MAX];
	int temp[MAX];
	int isodd = 0,level_num;
	int rear = 0,front = 0;
	queue[rear++] = node;//rear = 1
	int sum = 0;
	while ( front < rear )
	{
		int level_num = rear - front;
		
		for (int i = 0;i < level_num ;i++ )
		{
			struct tree_node *curr = queue[front++];//front++,logic gap
			int index = isodd ? i : level_num-1-i;
			temp[index] = curr->element;
		   if ( curr->left ) queue[rear++] = curr->left;
		   if ( curr->right ) queue[rear++] = curr->right;
      }
      isodd = !isodd;
      for (int i = 0;i < level_num ;i++ )
     	{
     	   printf("%d",temp[i]);
     	   if( sum+i < N-1 )
         {
           	printf(" ");
         }
    	}
    	sum += level_num;
	}	
	return;
}
