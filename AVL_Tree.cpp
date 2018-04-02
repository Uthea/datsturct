/*
	AVL TREE (SELF BALANCED BST TREE)
	EXAMPLE CODE
	OPERATION ON THE TREE = DELETION AND INSERTION
	DOCUMENTED ON 20 MARCH 2018
*/

#include <stdlib.h>
#include <stdio.h>

struct Node //create binary node with 2 pointer left and right
{
	int height;
	int num;
	struct Node *left, *right;
};


int max (int left, int right) // finding max value for calHeight function
{
	if (left > right) return left;
	else return right;
} 

int minTreeValue (struct Node *root) //used in BST node deletion
{
	while (root->left !=  NULL) root = root->left;
	return root->num;
} 

int maxTreeValue (struct Node *root) //used in BST node deletion
{
	while (root->right !=  NULL) root = root->right;
	return root->num;
}

int calHeight (struct Node *root) // calculate the height of the node
{
	int hl, hr; // 2 int variable to contain value from right and left tree

	if (root == NULL) return 0; // when reaching leaf node return -1 or 0 (im using 0 here)
	hl = calHeight(root->left);
	hr = calHeight(root->right);

	return max(hl,hr) + 1; // add 1 before returning to the upper node 
}

struct Node * createNode (int num) //create new node allocated in heap memory (dynamic allocation)
{
	struct Node *newNode = (struct Node *) malloc (sizeof(struct Node));
	newNode->left = newNode->right = NULL;
	newNode->num = num;
	newNode->height = 0;
	return newNode;
}

struct Node * leftRotation(struct Node *root) //used in balancing tree
{
	struct Node *newRoot; //choosing the new root for doing rotation 

	newRoot = root->right; //assign the new root node
	root->right = newRoot->left; //assign current root node right pointer to the newRoot left
	newRoot->left = root; //assign root in the left position of newRoot

	root->height = calHeight(root); //adjust old root height
	newRoot->height = calHeight(newRoot); //adjust newRoot height

	return newRoot;
}

struct Node * rightRotation (struct Node *root)
{
	struct Node *newRoot; //choosing the new root for doing rotation 

	newRoot = root->left; //assign the new root
	root->left = newRoot->right; //assign left pointer of old root to the address that was pointed by newRoot right pointer 
	newRoot->right = root; // assgin newRoot right pointer pointing to the old root

	root->height = calHeight(root); //adjust old root height
	newRoot->height = calHeight(newRoot); //adjust new root height

	return newRoot;
}

struct Node * insertion (struct Node *root, int num)
{
	//normal BST insertion part

	if (root == NULL) //if reached null , assign new created node
	{
		root = createNode(num);
		return root;
	}
	else if (num < root->num) root->left = insertion(root->left, num); //if value that want to be inserted is less than current value
	else if (num > root->num) root->right = insertion(root->right, num); // the opposite of the above

	//end of BST insertion


	//adjust height of the node that were traversed
	root->height = calHeight(root->left) - calHeight(root->right);

	//check the height of tree, if height is bigger than 1 or less than -1 then do rotation
	if (root->height < -1 && num > root->right->num) root = leftRotation(root); // LL rotation 
	else if (root->height > 1 &&  num < root->left->num) root = rightRotation(root); // RR rotation
	else if (root->height > 1 && num > root->left->num) //LR rotation
	{
		root->left = leftRotation(root->left); //left rotation first
		root = rightRotation(root); 
	}
	else if (root->height < -1 &&  num < root->right->num)//RL rotation
	{
		root->right = rightRotation(root->right); //right rotatio first
		root = leftRotation(root);
	}

	return root;

}

struct Node * deletion (struct Node *root, int num)
{
	//normal BST deletion
	if (root == NULL) return root;
	else if (root->num > num) root->left = deletion(root->left, num);
	else if (root->num < num) root->right = deletion(root->right, num);
	else
	{
		struct Node *temp;

		if (root->right == NULL && root->left == NULL) //leaf node
		{
			free(root);
			root = NULL;
		}

		else if (root->right != NULL && root->left != NULL) // two node in both right and left subtree
		{
			/*root->num = minTreeValue(root->right);
			root->right = deletion(root->right, root->num);*/   //using the min value in right subtree

			root->num = maxTreeValue(root->left);
			root->left = deletion(root->left, root->num); //using the max value in left subtree
		}

		else if (root->right != NULL) // one node in right subtree
		{
			temp = root;
			root = root->right;
			free(temp);
		}

		else if (root->left != NULL) //one node in left subtree
		{
			temp = root;
			root = root->left;
			free(temp);
		}
	}
	//end of BST deletion 

	//AVL tree part
	if (root != NULL) 
	{
		//adjust node height after deletion (every traversed node after the BST deletion)
		root->height = calHeight(root->left) - calHeight(root->right);

		

		if (root->height < -1) //check if tree become unbalanced /*/comparing the root->num with the deleted num doesn't work here (like in the insert function)/*/
		{
			if (calHeight(root->right->right) > calHeight(root->right->left)) root = leftRotation(root); //LL rotation
			else //LR rotation
			{
				root->right = rightRotation(root->right);
				root = leftRotation(root);
			}
		}

		else if (root->height > 1)
		{
			if (root->height > 1 &&  calHeight(root->left->left) > calHeight(root->left->right))  root = rightRotation(root); //RR rotation
			else //RL rotation
			{
				root->left = leftRotation(root->left);
				root = rightRotation(root);
			}
		}
	}

	return root;
}

int main ()
{
	struct Node *root = NULL;

	root = insertion(root, 21);
	root = insertion(root, 26);
	root = insertion(root, 30);
	root = insertion(root, 9);
	root = insertion(root, 4);
	root = insertion(root, 14);
	root = insertion(root, 28);
	root = insertion(root, 18);
	root = insertion(root, 15);
	root = insertion(root, 10);
	root = insertion(root, 2);
	root = insertion(root, 3);
	root = insertion(root, 7);

	root = deletion(root, 21);
	root = deletion(root, 26);
	root = deletion(root, 30);
	root = deletion(root, 9);
	root = deletion(root, 4);
	root = deletion(root, 2);
	root = deletion(root, 3);
	root = deletion(root, 7);
	root = deletion(root, 14);
	root = deletion(root, 28);
	root = deletion(root, 18);
	root = deletion(root, 15);
	root = deletion(root, 10);
	
	getchar();
	return 0;
}