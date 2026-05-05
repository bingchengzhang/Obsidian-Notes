#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
typedef struct tree_node
{
	int num;
	struct tree_node* left;
	struct tree_node* right;
}node;
struct tree_node *create_tree(int pre_order[], int in_order[], int , int , int , int);
void print_post_tree(struct tree_node *root,int *);
int main()
{
	int N,i = 0;
	scanf("%d",&N);
	int pre_order[N],pre = 0;	//define preorder array and "pre" to document the position of num
	int in_order[N],in = 0;	//define inorder array and "in" to document the position of num
	int stack[N];		//define "stack" to simulate pop and push
	int top_of_stack = 0;   //define the top point of stack
	char read_op[10];		//define string to read 'pop' & 'push'¡ª¡ª¡ª¡ª¡ª¡ªnot£¡£¡£¡"char *read_op" --contirbute wild pointer
	int count = 0;//control ' ' output
	int val;//accept "push"
	while ( i < 2*N )
	{
		scanf("%s",read_op);
		if ( !strcmp(read_op,"Push") )
		{
			scanf("%d",&val);
			stack[top_of_stack++] = val;//point to next!
			pre_order[pre++] = val;
		}
		else if ( !strcmp(read_op,"Pop") )
		{
			in_order[in++] = stack[--top_of_stack];//first '-' then use
		}
		i++;
	}
	struct tree_node *root = create_tree(pre_order, in_order,0 ,N-1, 0, N-1);//send pre&in
	print_post_tree(root,&count);
	return 0;
}
struct tree_node* create_tree(int pre_order[], int in_order[], int pre_left, int pre_right, int in_left, int in_right)
{
	if ( in_left > in_right )
	{
		return NULL;
	}
	struct tree_node *node = (struct tree_node *)malloc(sizeof(struct tree_node));//malloc to assign space
	node->left = NULL;
	node->right = NULL;
	if ( node != NULL )
	node->num = pre_order[pre_left];
	int k = in_left;
	while ( in_order[k] != pre_order[pre_left] )
   {
      k++;
   };//find "root" of in_order
   int left_size = k - in_left;
	node->left = create_tree(pre_order, in_order, pre_left+1, pre_left+left_size, in_left, k-1); //deal with LEFT
	node->right = create_tree(pre_order, in_order, pre_left+left_size+1, pre_right, k+1, in_right);//deal with RIGHT
   return node; 
}

void print_post_tree(struct tree_node *root,int *count)
{
	if ( root == NULL )
	{
		return ;
	}
	print_post_tree(root->left,count);
	print_post_tree(root->right,count);
	if( (*count)++ )
   {
   	printf(" ");
   }
   printf("%d",root->num);
}
