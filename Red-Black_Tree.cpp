///RBT DOUBLE POINTER IMPLEMENTATION , THANKS TO WIKIPEDIA FOR EASY EXPLANATION FOR INSERTION AND DELETION
//BIG THANKS TO STACKOVERFLOW USER FOR THE RED AND BLACK NODES VERIFYING CODE 

#include <stdio.h>
#include <stdlib.h>

struct Node 
{
    struct Node *left, *right, *parent;
    int num;
    char color;
};

struct Node * max_leftsubtree (struct Node *curr)
{
    if (curr == NULL) return NULL;
    while (curr->right != NULL) curr = curr->right;
    return curr;
}

struct Node * parent (struct Node ** N) 
{
    if (*N == NULL) return NULL;
    return (*N)->parent;
}

struct Node * grand_parent (struct Node **N) 
{
    if (parent(N) == NULL) return NULL;
    else return parent(N)->parent;
}

struct Node * sibling (struct Node **N)
{
    //if return value is NULL then no sibling
    if (parent(N) == NULL) return NULL;
    if (parent(N)->left != NULL && parent(N)->left == *N) return parent(N)->right;
    if (parent(N)->right != NULL && parent(N)->right == *N) return parent(N)->left;
    return NULL;
}

struct Node * uncle (struct Node **N)
{
    if (parent(N) == NULL || grand_parent(N) == NULL) return NULL;
    if (grand_parent(N)->left != NULL && grand_parent(N)->left == parent(N)) return grand_parent(N)->right;
    if (grand_parent(N)->right != NULL && grand_parent(N)->right == parent(N)) return grand_parent(N)->left;
    return NULL;
}

void rotate_left (struct Node **N)
{
    struct Node *NN = (*N)->right;
    struct Node *Nparent =  (*N)->parent;

    //switch color
    char temp = NN->color;
    NN->color = (*N)->color;
    (*N)->color = temp;

    //rotation
    (*N)->right = NN->left; 
    NN->left = *N;
    
    //change new node parent, N parent and N new right child parent
    NN->parent = Nparent;
    (*N)->parent = NN;
    if ((*N)->right != NULL) (*N)->right->parent = *N;

    //change Nparent left or right child
    if (Nparent != NULL)
    {
        if (Nparent->left == *N) Nparent->left = NN;
        else Nparent->right = NN;
    }


}

void rotate_right (struct Node **N)
{
    struct Node *NN = (*N)->left;
    struct Node *Nparent =  (*N)->parent;

    //switch color
    char temp = NN->color;
    NN->color = (*N)->color;
    (*N)->color = temp;

    //rotation
    (*N)->left = NN->right; 
    NN->right = *N;
    
    //change new node parent, N parent and N new right child parent
    NN->parent = Nparent;
    (*N)->parent = NN;
    if ((*N)->left != NULL) (*N)->left->parent = *N;

    //change Nparent left or right child
    if (Nparent != NULL)
    {
        if (Nparent->left == *N) Nparent->left = NN;
        else Nparent->right = NN;
    }

}

struct Node *createNewNode (int num)
{
    struct Node *NN =  malloc(sizeof(struct Node));
    NN->color = 'r';
    NN->num = num;
    NN->left = NN->right = NULL;
    NN->parent = NULL;
    return NN;
}

void insert_recursively (struct Node **root, struct Node **NN, struct Node *p)
{
    if (*root == NULL)
    {
        *root = *NN;
        (*NN)->parent = p;
    }
    else if ((*root)->num > (*NN)->num) insert_recursively(&(*root)->left, NN, *root);
    else if ((*root)->num < (*NN)->num) insert_recursively(&(*root)->right, NN, *root);
}

void insert_case1(struct Node **N);
void insert_case2(struct Node **N);
void insert_case3(struct Node **N);
void insert_case4_1stphase(struct Node **N);
void insert_case4_2ndphase(struct Node **N);
void insertion_case(struct Node **N);

void insert_case1(struct Node **N) {(*N)->color = 'b';}

void insert_case2(struct Node **N) {/*do nothing*/}

void insert_case3(struct Node **N) 
{
    struct Node *gp =  grand_parent(N);
    parent(N)->color = uncle(N)->color = 'b';
    grand_parent(N)->color = 'r';
    insertion_case(&gp); 
}

void insert_case4_1stphase(struct Node **N)
{
    if (grand_parent(N)->left == parent(N) && parent(N)->right == *N) 
    {
        struct Node *par = parent(N);
        rotate_left(&par);
        N = &(*N)->left;
    }
    else if (grand_parent(N)->right == parent(N) && parent(N)->left == *N) 
    {
        struct Node *par = parent(N);
        rotate_right(&par);
        N = &(*N)->right;
    }
    insert_case4_2ndphase(N);
}

void insert_case4_2ndphase(struct Node **N)
{
    struct Node *gp = grand_parent(N);
    if (parent(N)->left == *N) rotate_right(&gp);
    else rotate_left(&gp);
}


void insertion_case(struct Node **N)
{
    if (parent(N) == NULL) insert_case1(N);
    else if (parent(N) != NULL && parent(N)->color == 'b') insert_case2(N);
    else if (uncle(N) != NULL && uncle(N)->color == 'r') insert_case3(N);
    else  insert_case4_1stphase(N);
}

struct Node * fixroot (struct Node **N)
{
    struct Node *true_root = (*N);
    while (true_root->parent != NULL) true_root = true_root->parent;
    return true_root;
}

void insert(int num, struct Node **root)
{
    struct Node *NN = createNewNode(num);

    //insert NN to become a child of a leaf node
    insert_recursively(root, &NN, NULL);

    //check properties of rbt caused by new node insertion
    insertion_case(&NN);

    //fix root pointer placement
    *root = fixroot(root);
}

void replace_node (struct Node **N, struct Node **child)
{
    (*child)->parent = (*N)->parent;

    //if ((*child)->parent->left == *child) (*child)->parent->left = NULL;
    //else if ((*child)->parent->right == *child) (*child)->parent->right = NULL;

    if ((*N)->left != *child) (*child)->left = (*N)->left;
    else (*child)->right = (*N)->right;

    if (parent(N) != NULL)
    {
        if ((*N)->parent->left == *N) (*N)->parent->left = *child;
        else (*N)->parent->right = *child;
    }

}

struct Node *search_node (int num, struct Node **root)
{
    struct Node *rvalue = *root;
    struct Node *leaf = NULL;
    
    while (rvalue != NULL)
    {
        if (rvalue->num > num) rvalue = rvalue->left;
        else if (rvalue->num < num) rvalue = rvalue->right;
        else break;
    }

    if (rvalue->left != NULL) 
    {
        leaf = max_leftsubtree(rvalue->left);
        rvalue->num = leaf->num;
        rvalue = leaf;
    }
    return rvalue;
}

void delete_case1 (struct Node **N, struct Node **root);
void delete_case2 (struct Node **N, struct Node **root);
void delete_case3 (struct Node **N, struct Node **root);
void delete_case4 (struct Node **N, struct Node **root);
void delete_case5 (struct Node **N, struct Node **root);
void delete_case6 (struct Node **N, struct Node **root);
void del (int num, struct Node **root);



int isRed(struct Node *N)
{
    if (N == NULL || N->color == 'b') return 0;
    else return 1; 
}

int isBlack(struct Node *N)
{
    if (N == NULL || N->color == 'b') return 1;
    else return 0; 
}

void preorder(struct Node *root)
{
    if (root == NULL) return;
    printf("%d, %c\n", root->num, root->color);
    if (root->left != NULL) preorder(root->left);
    if (root->right != NULL) preorder(root->right);
}


void del (int num, struct Node **root)
{
    //search the node to be deleted
    struct Node *del = search_node(num, root);

    //assign the child of dell to be the replacement
    struct Node *child = NULL;
    if (del->left != NULL)  child = del->left;
    else child = del->right;

    //replace deleted node with child node
    if (child != NULL) replace_node(&del, &child);


    if (child == NULL && del->parent == NULL)
    {
        *root = NULL;
        free(del);
        return;
    }

    if (isBlack(del))
    {
       if (isRed(child)) child->color = 'b';
       else if (child != NULL) delete_case1(&child, root);
       else delete_case1(&del, root);
    }

    if (child == NULL) 
    {
        *root = fixroot(&del);
        if (del->parent->left == del) del->parent->left = NULL;
        else  del->parent->right = NULL;
    }
    else *root = fixroot(&child);
    free(del);
}


void delete_case1 (struct Node **N, struct Node **root)
{
    if (parent(N) != NULL) delete_case2(N, root);
}

void delete_case2 (struct Node **N, struct Node **root)
{
    struct Node *s = sibling(N);
    struct Node *p = parent(N);
    if (isRed(s))
    {
        if (p->left == *N) rotate_left(&p);
        else rotate_right(&p);
    }
    delete_case3(N, root);
}

void delete_case3 (struct Node **N, struct Node **root)
{
    if (isBlack(parent(N)) && isBlack(sibling(N)) && isBlack(sibling(N)->left) && isBlack(sibling(N)->right))
    {
        if (sibling(N) != NULL) sibling(N)->color = 'r';
        delete_case1(&(*N)->parent, root);
    }
    else delete_case4(N, root);
}

void delete_case4(struct Node **N, struct Node **root)
{
    if (isRed(parent(N)) && isBlack(sibling(N)) && isBlack(sibling(N)->left) && isBlack(sibling(N)->right))
    {
        parent(N)->color = 'b';
        if (sibling(N) != NULL) sibling(N)->color = 'r';
    }
    else delete_case5(N, root);
}

void delete_case5(struct Node **N, struct Node **root)
{
    struct Node *s = sibling(N);

    if (isBlack(s))
    {
        if (parent(N)->right == s && isRed(s->left) && isBlack(s->right)) rotate_right(&s);
        else if (parent(N)->left == s && isRed(s->right) && isBlack(s->left)) rotate_left(&s);
    }

    delete_case6(N, root);
}

void delete_case6(struct Node **N, struct Node **root)
{
    struct Node *s = sibling(N);
    if (parent(N)->left == *N && isBlack(s) && isRed(s->right)) 
    {
        s->right->color = 'b';
        rotate_left(&(*N)->parent);
    }
    else 
    {
        s->left->color = 'b';
        rotate_right(&(*N)->parent);
    }
}

int computeBlackHeight(struct Node* currNode) {
    // For an empty subtree the answer is obvious
    if (currNode == NULL)
        return 0; 
    // Computes the height for the left and right child recursively
    int leftHeight = computeBlackHeight(currNode->left);
    int rightHeight = computeBlackHeight(currNode->right);
    int add = currNode->color == 'b' ? 1 : 0;
    // The current subtree is not a red black tree if and only if
    // one or more of current node's children is a root of an invalid tree
    // or they contain different number of black nodes on a path to a null node.
    if (leftHeight == -1 || rightHeight == -1 || leftHeight != rightHeight)
        return -1; 
    else
        return leftHeight + add;
}

int CheckRedProperty(struct Node *root)
{
    if (root == NULL) 
        return 1;

    if (!CheckRedProperty(root->left))
        return 0;

    if (!CheckRedProperty(root->right))
        return 0;

    if (isRed(root))
    {
        if (isRed(root->left) || isRed(root->right)) return 0;
    }
    return 1;
}

void verify(struct Node * root)
{
    if (computeBlackHeight(root) != -1 && CheckRedProperty(root) == 1) printf("Passed\n\n");
    else printf("Failed\n\n");
}

void input_list (struct Node **root)
{
    insert(29, root);
    insert(72, root);
    insert(2, root);
    insert(57, root);
    insert(1, root);
    insert(99, root);
    insert(53, root);
    insert(82, root);
    insert(5, root);
    insert(7, root);
    insert(9, root);
    insert(17, root);
    insert(22, root);
    insert(33, root);
    insert(16, root);
    insert(4, root);
    insert(45 , root);
    insert(89 , root);
    insert(31 , root);
    insert(75  , root);
    insert(96  , root);
    insert(44  , root);
    insert(81  , root);
    insert(91  , root);
    insert(21  , root);
    insert(10  , root);
    insert(37  , root);
    insert(73  , root);
    insert(42  , root);
    insert(36  , root);
    preorder(*root);
    verify(*root);
}

void delete_list (struct Node **root)
{
    del(29, root);
    preorder(*root);
    verify(*root);
    del(72, root);
    preorder(*root);
    verify(*root);
    del(2, root);
    preorder(*root);
    verify(*root);
    del(57, root);
    preorder(*root);
    verify(*root);
    del(1, root);
    preorder(*root);
    verify(*root);
    del(99, root);
    preorder(*root);
    verify(*root);
    del(53, root);
    preorder(*root);
    verify(*root);
    del(82, root);
    preorder(*root);
    verify(*root);
    del(5, root);
    preorder(*root);
    verify(*root);
    del(7, root);
    preorder(*root);
    verify(*root);
    del(9, root);
    preorder(*root);
    verify(*root);
    del(17, root);
    preorder(*root);
    verify(*root);
    del(22, root);
    preorder(*root);
    verify(*root);
    del(33, root);
    preorder(*root);
    verify(*root);
    del(16, root);
    preorder(*root);
    verify(*root);
    del(4, root);
    preorder(*root);
    verify(*root);
    del(45 , root);
    preorder(*root);
    verify(*root);
    del(89 , root);
    preorder(*root);
    verify(*root);
    del(31 , root);
    preorder(*root);
    verify(*root);
    del(75  , root);
    preorder(*root);
    verify(*root);
    del(96  , root);
    preorder(*root);
    verify(*root);
    del(44  , root);
    preorder(*root);
    verify(*root);
    del(81  , root);
    preorder(*root);
    verify(*root);
    del(91  , root);
    preorder(*root);
    verify(*root);
    del(21  , root);
    preorder(*root);
    verify(*root);
    del(10  , root);
    preorder(*root);
    verify(*root);
    del(37  , root);
    preorder(*root);
    verify(*root);
    del(73  , root);
    preorder(*root);
    verify(*root);
    del(42  , root);
    preorder(*root);
    verify(*root);
    del(36  , root);
    preorder(*root);
    verify(*root);
}



int main ()
{
    struct Node *root = NULL;
    input_list(&root);
    delete_list(&root);
    getchar();
}