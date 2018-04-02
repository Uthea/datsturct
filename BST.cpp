/************************************************************************
	BST USING PARENT AND FOR DELETION WITHOUT COPY (POINTER MANIPULATION)
	ALL USING SIGLE POINTER
 ************************************************************************/


#include <stdio.h>
#include <stdlib.h>


struct Node 
{
	int num;
	struct Node *left, *right, *parent;
};

struct Node *createNode (struct Node *root, struct Node *parent, int num)  //create a newNode
{
	struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));
	newNode->left = newNode->right = NULL;
	newNode->parent = parent;
	newNode->num = num;
	return newNode;
}

struct Node *search (struct Node *root, int num)
{
	if (root != NULL)
	{
		if (root->num < num) return search(root->right, num);
		else if (root->num > num) return search(root->left, num);
		else return root;
	}

	return root;
} 

struct Node *max (struct Node *root) // find max (use the left subtree (left child of root)
{
	while (root->right != NULL) root = root->right;
	return root;
}

struct Node * BSTinsertion(struct Node *root, struct Node *parent, int num)
{
	if (root != NULL) //check if not NULL (equivalent to if (root)
	{
		if (root->num < num) root->right = BSTinsertion(root->right, root, num); //go to right subtree if num bigger lesser than root num
		else if (root->num > num) root->left = BSTinsertion(root->left, root, num); // the opposite of the above
	}

	else return createNode(root, parent, num); //when root reached NULL, insert new node and return it
	return root; // return root when recursive call reach the end of line
}

struct Node * BSTdeletion(struct Node *root, int num)
{
	/**************************************************************************
		NOTE THAT THIS FUNCTION USE SINGEL POINTER AND NOT DOUBLE POINTER 
		SO THAT THE ROOT NODE THAT PASSED HERE IS NOT BY REFERENCE BUT BY VALUE
		AND THAT ALSO MEAN THE ROOT NOTE HERE IS THE SAME AS CURR NODE WHICH IS
		NOT THE REAL ROOT
	 ***************************************************************************/
		
	if (root != NULL) // when root is not equal null 
	{
		if (root->num > num) root->left = BSTdeletion(root->left, num); //go to right subtree if num bigger lesser than root num
		else if (root->num < num) root->right = BSTdeletion(root->right, num); // the opposite of the above
		else if (root->num == num) //if the node that we want to delete is found
		{
			struct Node *del = root; 
			if (del != NULL)
			{
				

				if (del->right != NULL && del->left != NULL) //case 1 : if the node that want to be deleted have 2 children
				{
					struct Node *replace = max(root->left); // replace varible as max value of left subtree 

					//check if the replace node is the left or right child of parent and then cut the link of parent left or right pointer to the left child of replace node
					if(replace->parent->right == replace) replace->parent->right = replace->left; 
					else if (replace->parent->left == replace) replace->parent->left = replace->left;

					//if replace node have left child then change left child parent to the perent of replace
					if (replace->left != NULL) replace->left->parent = replace->parent;

					//assign replace node right, left, and parent pointer as the same as the node that want to be deleted
					replace->right = del->right;
					replace->left = del->left;
					replace->parent = del->parent;

					//check if the left or right child of replace node are not NULL and if they are then change their parent to the replaced node 
					if (replace->right != NULL) replace->right->parent = replace;
					if (replace->left != NULL) replace->left->parent = replace;

					//assign max value (replace node) as the root 
					root = replace; 
				}

				else if (del->right != NULL) // case 2: the node that want to be deleted have one child (right child)
				{ 
					del->right->parent = del->parent; //assign right child parent as the parent of node that want to be deleted
					root = del->right; //set right child as the replaced node

				}

				else if (del->left != NULL) //case 3: the node that want to be deleted have one child (left child)
				{
					del->left->parent = del->parent; //assign left child parent as the parent of node that want to be deleted
					root = del->left; //set left child as the replaced node
				}

				else root = NULL; //the node that want to be deleted is a leaf node

				free(del); //delete the old node (the node that want to be deleted)
			}
		}

	}

	else printf("Node not found"); // root equal null, which mean the node that we want to search can't be found
	return root; //return root for each end of recursive call
}

void inorder(struct Node *root)
{
	if (root != NULL)
	{
		inorder(root->left);
		printf("%d ", root->num);
		if (root->parent != NULL ) printf("parent is %d\n", root->parent->num);
		else printf("parent is NULL\n");
		inorder(root->right);
	}
}



int main ()
{
	struct Node *root = NULL;

	root = BSTinsertion(root, NULL, 50);
	root = BSTinsertion(root, NULL, 30);
	root = BSTinsertion(root, NULL, 20);
	root = BSTinsertion(root, NULL, 40);
	root = BSTinsertion(root, NULL, 70);
	root = BSTinsertion(root, NULL, 60);
	root = BSTinsertion(root, NULL, 80);
	root = BSTinsertion(root, NULL, 39);

	root = BSTdeletion(root, 50);
	printf("inorder: \n");
	inorder(root);
	getchar();
	return 0;
}