/*
	RED BLACK TREEE IMPLEMENTATION
	DELETION PART WAS TOO DIFFUCULT FOR NOW (TOO MANY CASES)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Node 
{
	int num;
	char color;
	struct Node *left, *right, *parent;
};

struct Node * createNode (struct Node *parent, int num)
{
	struct Node *newNode = (struct Node *) malloc (sizeof(struct Node));
	newNode->num = num;
	newNode->left = newNode->right = NULL;
	newNode->parent = parent;
	if(parent != NULL) newNode->color = 'r';
	else newNode->color = 'b';
	return newNode;
}

struct Node * max (struct Node *root)
{
	while (root->right != NULL) root = root->right;
	return root;
}

struct Node * leftRotate(struct Node *root)
{
	struct Node *newRoot = root->right;

	//recoloring
	newRoot->color = 'b';
	root->color = 'r';

	//rotation
	newRoot->parent = root->parent; //assign newRoot parent to the parent of the old root
	root->right = newRoot->left; // assign the right ptr or root to the left child of newRoot
	
	if (newRoot->left != NULL) newRoot->left->parent = root; // assign the left child of newRoot parent to old root (if left child is not NULL)
	
	newRoot->left = root; //assign newRoot left to the old root
	root->parent = newRoot; //assign old root parent as the newRoot
	root = newRoot; //set newRoot as the root node
	return root; //return the new root

}

struct Node * rightRotate(struct Node *root)
{
	struct Node *newRoot = root->left;
	
	//recoloring
	newRoot->color = 'b';
	root->color = 'r';

	//rotation
	newRoot->parent = root->parent;
	root->left = newRoot->right;

	if (newRoot->right != NULL) newRoot->right->parent = root;

	newRoot->right = root;
	root->parent = newRoot;
	root = newRoot;
	return root;
}

struct Node *rbInsertion(struct Node *root, struct Node *par, int num)
{
	//BST insertion part
	if (root == NULL) return createNode(par, num); //if root is NULL then create a new node and pass the parent address
	else if (num > root->num) root->right = rbInsertion(root->right, root, num); //if num is bigger than curr root num, then go to right child (passing root as parent)
	else if (num < root->num) root->left = rbInsertion(root->left, root, num); // if num is lesser than curr root num, then go to left child (passing root as parent)
	//end of BST insertion part

	// check violation
	if (root->color == 'b') //if curr root is black
	{
		if(root->right != NULL && root->left != NULL && root->right->color == 'r' && root->left->color == 'r') // if left child and right child is red (do coloring)
		{
			root->right->color = root->left->color = 'b';
			if (root->parent == NULL) root->color = 'b';
			else root->color = 'r';
		}

		else if (root->right != NULL && (root->left == NULL || root->left->color == 'b')) //if right child exist and the left child is either NULL or black
		{
			if (root->right->right != NULL && root->right->color == 'r' && root->right->right->color == 'r' ) root = leftRotate(root); //LL rotation (or left rotation)
			else if ( root->right->left != NULL && root->right->left->color == 'r' ) // RL
			{
				root->right = rightRotate(root->right);
				root = leftRotate(root);
			}
		}

		else if (root->left != NULL && (root->right == NULL || root->right->color == 'b')) //if left child exist and the right child is either NULL or black
		{
			if (root->left->left != NULL && root->left->color == 'r' && root->left->left->color == 'r') root = rightRotate(root); //RR rotation /(or right rotation)
			else if (root->left->right != NULL && root->left->right->color == 'r' ) // LR
			{
				root->left = leftRotate(root->left); 
				root = rightRotate(root);
			}
			
		}
	}

	return root;
}

struct Node *rbDeletion(struct Node *root, int num)
{
	if (root != NULL)
	{
		if (root->num > num) root->left = rbDeletion(root->left, num);
		else if (root->num < num) root->right = rbDeletion(root->right, num);

		if ((root->left != NULL && root->left->color == 'd') || (root->right != NULL && root->right->color == 'd') )
		{
			
		}

		else 
		{
			/*				          normal BST deletion								*/
			struct Node *replace = NULL;
			struct Node *parent = root->parent;

			if (root->left != NULL && root->right != NULL) //root have two children
			{
				replace = max(root->left);

				if (replace->parent->right == replace) replace->parent->right = replace->left;
				else if (replace->parent->left == replace) replace->parent->left = replace->left;

				if(replace->left != NULL) replace->left->parent = replace->parent;
					
				replace->parent = root->parent;
				replace->right = root->right;
				replace->left = root->left;

				if (replace->right != NULL) replace->right->parent = replace;
				if (replace->left != NULL) replace->left->parent = replace;
			}


			else if (root->left != NULL) // root have one child (left)
			{
				replace = root->left;
				replace->parent = root->parent;
			}

			else if (root->right != NULL) //root have left child (right)
			{
				replace = root->right;
				replace->parent = root->parent;
			}

			//if old root deletion part (if root have no child , its directly go here)
			free(root);
			root = replace;
			/*							end of BST deletion							*/

			if (root == NULL)
			{
				struct Node *terminal = (struct Node *) malloc(sizeof(struct Node));
				terminal->color = 'd';
				terminal->parent = parent;
				terminal->left = terminal->right = NULL;
				root = terminal;
			}


			
		}
	}
	
	return root;
}

int main ()
{
	struct Node *root = NULL;
	root = rbInsertion(root, NULL, 9);
	root = rbInsertion(root, NULL, 8);
	root = rbInsertion(root, NULL, 7);
	root = rbInsertion(root, NULL, 6);
	root = rbInsertion(root, NULL, 5);
	root = rbInsertion(root, NULL, 4);
	root = rbInsertion(root, NULL, 11);
	root = rbInsertion(root, NULL, 15);
	root = rbInsertion(root, NULL, 13);
	root = rbInsertion(root, NULL, 1);
	root = rbInsertion(root, NULL, 3);
	root = rbInsertion(root, NULL, 2);

	root = rbDeletion(root, 6);
	getchar();
	return 0;
}