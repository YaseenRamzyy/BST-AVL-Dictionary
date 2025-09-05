#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ANSI color codes
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"
#define RESET   "\033[0m"

typedef struct node{
char word[100];
struct node*left;
struct node*right;
int height;
}node;

int maximum(int a, int b){
    int x;
    x=(a>b)?a:b;
    return x;
}

int height(node*n){
if(n==NULL) return 0;
return n->height;
}

node*newnode(char *word){
node*n=(node*)malloc(sizeof(node));
strcpy(n->word,word);
n->left=NULL;
n->right=NULL;
n->height=1;
return n;
}

//right rotation

node*rightrotation(node*y){
node*x=y->left;
node*t2=x->right;

x->right=y;
y->left=t2;

y->height=maximum(height(y->left),height(y->right))+1;
x->height=maximum(height(x->left),height(x->right))+1;
return x;
}

//left rotation

node*leftrotation(node*x){
node*y=x->right;
node*t2=y->left;

y->left=x;
x->right=t2;

x->height=maximum(height(x->left),height(x->right))+1;
y->height=maximum(height(y->left),height(y->right))+1;
return y;
}

int getbalance(node*n){
if(n==NULL)return 0;
return height(n->left)-height(n->right);
}

node*insert(node*n,char*word){
if(n==NULL){
    return newnode(word);
}

// we will use the strcasecmp func because we want to ignore the upper and lower cases conditions

else if(strcasecmp(word,n->word)<0){
    n->left=insert(n->left,word);
}
else if(strcasecmp(word,n->word)>0){
    n->right=insert(n->right,word);
}
else return n;

n->height=maximum(height(n->left),height(n->right))+1;

int bf=getbalance(n);

//left left

if(bf>1 && strcasecmp(word,n->left->word)<0){
    return rightrotation(n);
}

//right right

if(bf<-1 && strcasecmp(word,n->right->word)>0){
    return leftrotation(n);
}

// left right
if(bf>1 && strcasecmp(word,n->left->word)>0){
    n->left=leftrotation(n->left);
    return rightrotation(n);
}

//right left
if(bf<-1 && strcasecmp(word,n->right->word)<0){
    n->right=rightrotation(n->right);
    return leftrotation(n);
}
return n;

}

node*load(const char*filename){ // we used const char for safety in order not make mistake or change anything while reading the file
FILE*file=fopen(filename,"r");
if(!file){
    printf("File not found!");
    exit(1);
}
char word[100];
node*root=NULL;

while(fgets(word,sizeof(word),file)){
    word[strcspn(word,"\n")]='\0';   //remove \n
    if(strlen(word)>0){
       root=insert(root,word);
    }

}

fclose(file);
return root;


}

// function to get the size of the BST tree

int size(node*n){
if(n==NULL){
    return 0;
}
else
    return 1+size(n->left)+size(n->right);
}

// function to get the inorder successor and predecessor

node* FindMin(node*n){
if(n && n->left!=NULL)
    n=n->left;
else
    return n;
}

node* FindMax(node*n){
if(n && n->right!=NULL)
    n=n->right;
else
    return n;
}

node* InorderSuccessor(node*n,char*word){
node*succ=NULL;
while(n!=NULL){
    if(strcasecmp(word,n->word)<0){
        succ=n;
        n=n->left;
    }
    else
        n=n->right;
}
return succ;
}

node* InorderPredecessor(node*n,char*word){
node*pre=NULL;
while(n!=NULL){
    if(strcasecmp(word,n->word)>0){
        pre=n;
        n=n->right;
    }
    else
        n=n->left;
}
return pre;
}

// function to search for the word and keep tracing of the last visited node (for suggestions)

node* LastVisited=NULL;

node* SearchWithLastVisited(node*n,char*word){
LastVisited=NULL;
while(n!=NULL){
    LastVisited=n; //keep trace of the last visited node
    if(strcasecmp(word,n->word)==0){
        return n;
    }
    else if(strcasecmp(word,n->word)<0)
        n=n->left;
    else
        n=n->right;
}
    return NULL; //The word is incorrect
}

// function to check if the word in correct

void CheckWord(node*n,char*str){
char* token=strtok(str," \t\n.,;:?!");
while(token!=NULL){
    node*found=SearchWithLastVisited(n,token);
    if(found)
        printf(GREEN "\n%s - CORRECT\n" RESET,token);
    else{
        printf(RED "\n%s - INCORRECT\n" RESET, token);
        printf(YELLOW "Suggestions:\n" RESET);
        printf(CYAN "A. %s\n" RESET, LastVisited ? LastVisited->word : "None");
        node*pred = InorderPredecessor(n, LastVisited->word);
        printf(MAGENTA "B. %s\n" RESET, pred ? pred->word : "None");
        node*succ = InorderSuccessor(n, LastVisited->word);
        printf(BLUE "C. %s\n" RESET,succ?succ->word:"None");
    }
    token=strtok(NULL," \t\n.,;:?!");
}
}

void printHeader(){
    printf(CYAN "\n======================================================\n");
    printf("           BINARY SEARCH TREE DICTIONARY\n");
    printf("               SPELL CHECKING SYSTEM\n");
    printf("======================================================\n" RESET);
}

void printMenu(){
    printf(YELLOW "\n======================================================\n");
    printf("                      MAIN MENU\n");
    printf("------------------------------------------------------\n");
    printf("   1. Check a sentence\n");
    printf("   2. Show dictionary statistics\n");
    printf("   3. Exit\n");
    printf("======================================================\n" RESET);
    printf(GREEN "Enter your choice (1-3): " RESET);
}

void printSeparator(){
    printf(MAGENTA "\n------------------------------------------------------\n" RESET);
}

void clearInputBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main()
{   char sentence[2005];
    int choice;

    printHeader();

    node*root=load("Dictionary.txt");
    printf(GREEN "Dictionary loaded successfully.\n" RESET);

    do {
        printMenu();
        scanf("%d", &choice);
        clearInputBuffer();

        switch(choice){
            case 1:
                printSeparator();
                printf(GREEN "Enter a sentence :\n" RESET);
                printf(YELLOW "> " RESET);
                fgets(sentence,sizeof(sentence),stdin);
                sentence[strcspn(sentence,"\n")] = '\0';

                printSeparator();
                printf("Checking: \"%s\"\n", sentence);
                printSeparator();

                CheckWord(root,sentence);
                printSeparator();
                break;

            case 2:
                printSeparator();
                printf("DICTIONARY STATISTICS:\n");
                printSeparator();
                printf("Size = %d\n",size(root));
                printf("Height = %d\n",height(root));
                printSeparator();
                break;

            case 3:
                printf(GREEN "Thank you for using the BST Dictionary!\n" RESET);
                break;

            default:
                printf(RED "Invalid choice! Please enter 1, 2, or 3.\n" RESET);
        }

    } while(choice != 3);

    return 0;
}
