#include<iostream>
using namespace std;

struct TrieTreeN
{
	TrieTreeN *arr[26];
	bool isEndOfWord;
    // if isEndOfWprd is true then values of cordinates are valid
    int cordinates[4][2]{0};
};

struct Node
{
    Node *left,*right;
    string name = "";
    string bankName;
    int xOy;
    int cordinates[2];
};

struct TrieTreeB
{
	TrieTreeB *arr[26];
	bool isEndOfWord;
    // if isEndOfWprd is true then values of cordinates and branchs are valid
    int cordinates[2];
    Node *branchs = NULL ;
};

struct TrieTreeN *createNaiveNeighbourhoodNode()
{
	struct TrieTreeN *tmp = new TrieTreeN;
	tmp->isEndOfWord = false;
	for (int i = 0; i < 26; i++)
		tmp->arr[i] = NULL;

	return tmp;
}

void insertNeighbourhood(TrieTreeN *root, string name,int cordinates[4][2])
{
	struct TrieTreeN *tmp = root;
	for (int i = 0; i < name.length(); i++)
	{
		int index = name[i] - 'a';
		if (!tmp->arr[index])
			tmp->arr[index] = createNaiveNeighbourhoodNode();
		tmp = tmp->arr[index];
	}
    for(int i=0;i<8;++i)
        tmp->cordinates[i/2][i%2]=cordinates[i/2][i%2];
	tmp->isEndOfWord = true ;
}

void addNeighbourhood(TrieTreeN *root)
{
    string name;
    cout<<"\tReady for adding a Neighbourhood\n\tPlease Enter a name for Neighbourhod\n\t";
    cin>>name;
    int cordinates[4][2];
    cout<<"\tNow Enter 4 cordinates of your Neighbourhod first X second Y\n";
    for(int i=0;i<4;++i)
    {
        cout<<"\tX: ";
        cin>>cordinates[i][0];
        cout<<"\tY: ";
        cin>>cordinates[i][1];
    }
    insertNeighbourhood(root,name,cordinates);
    cout<<"\tNew Neighbourhood added successfully\n\n";
}

struct Node *newNode(int arr[],int depth,string name,string bankName)
{
	struct Node* temp = new Node;
	for (int i=0; i<2; i++)
	    temp->cordinates[i] = arr[i];
	temp->left =NULL;
    temp->right = NULL;
    temp->xOy=depth%2;
    temp->name = name ;
    temp->bankName = bankName ;
	return temp;
}

struct Node *insertNode(Node *root, int point[],string name,string bankName,int depth = 0)
{
	if (root == NULL)
	    return newNode(point,depth,name,bankName);

	if (point[depth % 2] < (root->cordinates[depth%2]))
		root->left = insertNode(root->left, point, name , bankName ,depth + 1);
	else
		root->right = insertNode(root->right, point,name,bankName,depth + 1);
    if(bankName != "")
        root->bankName = bankName ;
	return root;
}

struct TrieTreeB *search(struct TrieTreeB *root, string name)
{
	struct TrieTreeB *tmp = root;

	for (int i = 0; i < name.length(); i++)
	{
		int index = name[i] - 'a';
		if (!tmp->arr[index])
			return NULL;

		tmp = tmp->arr[index];
	}

	return tmp ;
}

struct Node *kdSearch(Node *root, int cordinate[])
{
    if (root == NULL)
        return NULL ;
    if (root->cordinates[0] == cordinate[0] && root->cordinates[1] == cordinate[1])
        return root;

    if (cordinate[root->xOy] < root->cordinates[root->xOy])
        return kdSearch(root->left, cordinate);
  
    return kdSearch(root->right, cordinate);
}

struct Node *addBranch(Node *kdtree,TrieTreeB *trietree,Node *kdBanks)
{
    string bankName;
    cout<<"\tEnter Bank's name of this Branch\n\t";
    cin>>bankName;
    struct TrieTreeB *tmp = search(trietree,bankName);
    if(tmp==NULL)
    {
        cout<<"\tthere isn't any Bank with name: "<<bankName<<" Try again later :/\n";
        return kdtree ;
    }
    int cordinate[2];
    cout<<"\tEnter Branch's cordinate\n\tX: ";
    cin>>cordinate[0];
    cout<<"\tY: ";
    cin>>cordinate[1];
    if(kdSearch(kdtree,cordinate) != NULL)
    {
        cout<<"\tone Branch is there\n";
        return kdtree ;
    }
    if(kdSearch(kdBanks,cordinate) != NULL)
    {
        cout<<"\tone Bank is there\n";
        return kdtree ;
    }
    string branchName;
    cout<<"\tNow Enter Branch's name\n\t";
    cin>>branchName;
    // insert new branch to it's kd-tree
    kdtree = insertNode(kdtree,cordinate,branchName,bankName);
    // insert new branch to trie-tree of banks
    if(tmp->branchs==NULL)
    {
        // struct Node *ghost = new Node;
        // tmp->branchs = ghost ;
        // ghost->left = insertNode(NULL,cordinate,branchName,bankName);
        // ghost->right = ghost->left ;
        tmp->branchs = new Node ;
        tmp->branchs->left = insertNode(tmp->branchs->left,cordinate,branchName,bankName);
        tmp->branchs->right = tmp->branchs->left ;
    }
    else
    {
        tmp->branchs->left = insertNode(tmp->branchs->left,cordinate,branchName,bankName);
        tmp->branchs->right = tmp->branchs->left ;
    }
    cout<<"\tNew Branch added successfully\n\n";
    return kdtree ;
}

struct TrieTreeB *createNaiveBanks()
{
    struct TrieTreeB *tmp = new TrieTreeB;
	tmp->isEndOfWord = false;
	for (int i = 0; i < 26; i++)
		tmp->arr[i] = NULL;

	return tmp;
}

void insertBanksToTrie(TrieTreeB *root,string name)
{
    struct TrieTreeB *tmp = root;
        for (int i = 0; i < name.length(); i++)
        {
            int index = name[i] - 'a';
            if (!tmp->arr[index])
                tmp->arr[index] = createNaiveBanks();
            tmp = tmp->arr[index];
        }
        tmp->isEndOfWord = true;
}

struct Node *addBank(TrieTreeB *root,Node *kdTreeB)
{
    int cordinates[2];
    cout<<"\tEnter Bank's cordinate\n\tX: ";
    cin>>cordinates[0];
    cout<<"\tY: ";
    cin>>cordinates[1];
    if(kdSearch(kdTreeB,cordinates)!=NULL)
    {
        cout<<"\n\tone Bank has this cordinate\n" ;
        // return addBank(root , kdTreeB) ;
        return kdTreeB ;
    }
    string name;
    cout<<"\n\tEnter Bank's name\n\t";
    cin>>name;
    insertBanksToTrie(root,name);
    kdTreeB = insertNode(kdTreeB,cordinates,name,name);
    cout<<"\n\tnew Bank added successfully\n\n";
    return(kdTreeB);
}

struct TrieTreeN *searchN(struct TrieTreeN *root, string name)
{
	struct TrieTreeN *tmp = root;

	for (int i = 0; i < name.length(); i++)
	{
		int index = name[i] - 'a';
		if (!tmp->arr[index])
			return NULL;

		tmp = tmp->arr[index];
	}

	return tmp ;
}

int *findingMinAndMax(int cr[4][2])
{
    int minx = cr[0][0];
    int maxx = cr[0][0];
    int miny = cr[0][1];
    int maxy = cr[0][1];
    for(int i=0;i<4;++i)
    {
        if(cr[i][0]<minx)
            minx = cr[i][0];
        if(cr[i][0]>maxx)
            maxx = cr[i][0];
        if(cr[i][1]<miny)
            miny = cr[i][1];
        if(cr[i][1]>maxy)
            maxy = cr[i][1];
    }
    int result[] = {minx,maxx,miny,maxy};
    int *ptr = result ;
    return ptr ;
}

void validateRec(int minx,int maxx,int miny,int maxy, Node *root)
{
    if (root != NULL)
    {
        if(root->xOy==1)
        {
            if(root->cordinates[root->xOy] > maxy)
                return validateRec(minx,maxx,miny,maxy,root->left);
            if(root->cordinates[root->xOy] < miny)
                return validateRec(minx,maxx,miny,maxy,root->right);
            if(root->cordinates[root->xOy] < maxy && root->cordinates[root->xOy] > miny)
            {
                if(root->cordinates[0] < maxx && root->cordinates[0] > minx)
                    cout<<"\tBank is: "<<root->name<<" with cordinate: ["<<root->cordinates[0]<<","<<root->cordinates[1]<<"]\n";
                validateRec(minx,maxx,miny,maxy,root->left);
                validateRec(minx,maxx,miny,maxy,root->right);
            }
        }
        if(root->xOy==0)
        {
            if(root->cordinates[root->xOy] > maxx)
                return validateRec(minx,maxx,miny,maxy,root->left);
            if(root->cordinates[root->xOy] < minx)
                return validateRec(minx,maxx,miny,maxy,root->right);
            if(root->cordinates[root->xOy] < maxx && root->cordinates[root->xOy] > minx)
            {
                if(root->cordinates[1] < maxy && root->cordinates[1] > miny)
                    cout<<"\tBank is: "<<root->name<<" with cordinate: ["<<root->cordinates[0]<<","<<root->cordinates[1]<<"]\n";
                validateRec(minx,maxx,miny,maxy,root->left);
                validateRec(minx,maxx,miny,maxy,root->right);
            }
        }
    }
}

void listBanksNeighbourhood(TrieTreeN *root,Node *banks)
{
    string neighName;
    cout<<"\tPlease Enter Neighbourhood's name\n";
    cin>>neighName;
    struct TrieTreeN *tmp = searchN(root,neighName);
    if(tmp == NULL)
    {
        cout<<"\tThere isn't any Neighbourhood with Name: "<<neighName<<" try again later :/\n";
        return ;
    }
    struct Node *ptr = banks ;
    int *cr = findingMinAndMax(tmp->cordinates);
    int minx = *cr;
    int maxx = *(cr+1);
    int miny = *(cr+2);
    int maxy = *(cr+3);
    validateRec(minx,maxx,miny,maxy,ptr);
}

void validateLikeInorder(struct Node* root)
{
    if (root != NULL)
        if(root->name!="")
        {
            validateLikeInorder(root->left);
            cout<<"\t"<<root->name<<" with cordinate ["<<root->cordinates[0]<<","<<root->cordinates[1]<<"]\n";
            validateLikeInorder(root->right);
        }
}

void listBranchBank(TrieTreeB *root)
{
    string bankName;
    cout<<"\tEnter Bank's name\n\t";
    cin>>bankName;
    struct TrieTreeB *tmp = search(root,bankName);
    if(tmp == NULL)
    {
        cout<<"\tthere isn't any Bank with this name:\n";
        return ;
    }
    if(tmp->branchs != NULL)
    {
        cout<<"\tBanks:\n";
        validateLikeInorder(tmp->branchs->left);
    }
    else
        cout<<"\tthis bank hasn't any branch :/";
    cout<<"\n";
}

long distance(Node *goal,Node *root)
{
    long x = abs(goal->cordinates[0]-root->cordinates[0]);
    long y = abs(goal->cordinates[1]-root->cordinates[1]);
    long result = x*x + y*y;
    return result;
}

struct Node *nearest(Node *root,Node *goal,Node *best)
{
    if(root == NULL)
        return best;
    if(distance(goal,root)<distance(goal,best))
        best = root;
    Node *goodSide;
    Node *badSide;
    if(goal->cordinates[root->xOy]<root->cordinates[root->xOy])
    {
        goodSide = root->right;
        badSide = root->left;
    }
    else
    {
        goodSide = root->left;
        badSide = root->right;
    }
    best = nearest(goodSide,goal,best);
    if(abs(root->cordinates[root->xOy]-goal->cordinates[root->xOy])<distance(goal,best));
        best = nearest(badSide,goal,best);
    return best;
}

void nearestBank(Node *kdtreeb)
{
    if(kdtreeb == NULL)
    {
        cout<<"\tthere isn't any Bank yet -_-\n\n";
        return ;
    }
    struct Node *target = new Node;
    cout<<"\tEnter query cordinate\n\tX: ";
    cin>>target->cordinates[0];
    cout<<"\tY: ";
    cin>>target->cordinates[1];
    target->left=NULL;
    target->right=NULL;
    struct Node *best = nearest(kdtreeb,target,kdtreeb);
    cout<<"\t"<<best->name
        <<" with cordinate ["<<best->cordinates[0]<<","
        <<best->cordinates[1]<<"]\n\n";
}

void nearestBranch(Node *root)
{
    if(root == NULL)
    {
        cout<<"\tthere isn't any Branch yet -_-\n\n";
        return ;
    }
    struct Node *target = new Node;
    cout<<"\tEnter query cordinate\n\tX: ";
    cin>>target->cordinates[0];
    cout<<"\tY: ";
    cin>>target->cordinates[1];
    target->left=NULL;
    target->right=NULL;
    struct Node *best = nearest(root,target,root);
    cout<<"\tNearest Branch from cordinate ["<<target->cordinates[0]<<","
        <<target->cordinates[1]<<"] is "<<best->name
        <<" Bank with cordinate ["<<best->cordinates[0]<<","
        <<best->cordinates[1]<<"]\n\n";
}

void validateCir(Node *center , int radius,int minx,int maxx,int miny,int maxy, Node *root)
{
    if (root != NULL)
    {
        if(root->xOy==1)
        {
            if(root->cordinates[root->xOy] > maxy)
                return validateCir(center,radius,minx,maxx,miny,maxy,root->left);
            if(root->cordinates[root->xOy] < miny)
                return validateCir(center,radius,minx,maxx,miny,maxy,root->right);
            if(root->cordinates[root->xOy] < maxy && root->cordinates[root->xOy] > miny)
            {
                if((root->cordinates[0] < maxx && root->cordinates[0] > minx ) && distance(center,root)<=radius)
                    cout<<"\tBank is: "<<root->name<<" with cordinate: ["<<root->cordinates[0]<<","<<root->cordinates[1]<<"]\n";
                validateCir(center,radius,minx,maxx,miny,maxy,root->left);
                validateCir(center,radius,minx,maxx,miny,maxy,root->right);
            }
        }
        if(root->xOy==0)
        {
            if(root->cordinates[root->xOy] > maxx)
                return validateCir(center,radius,minx,maxx,miny,maxy,root->left);
            if(root->cordinates[root->xOy] < minx)
                return validateCir(center,radius,minx,maxx,miny,maxy,root->right);
            if(root->cordinates[root->xOy] < maxx && root->cordinates[root->xOy] > minx)
            {
                if((root->cordinates[1] < maxy && root->cordinates[1] > miny) && distance(center,root)<radius)
                    cout<<"\tBank is: "<<root->name<<" with cordinate: ["<<root->cordinates[0]<<","<<root->cordinates[1]<<"]\n";
                validateCir(center,radius,minx,maxx,miny,maxy,root->left);
                validateCir(center,radius,minx,maxx,miny,maxy,root->right);
            }
        }
    }
}

void availableBank(Node *root)
{
    int x,y;
    cout<<"\tPlease enter a Radius\n\tR: ";
    int R;
    cin>>R;
    R *=R;
    cout<<"\tEnter query cordinate\n\tX: ";
    cin>>x;
    cout<<"\tY: ";
    cin>>y;
    int minx = x-R;
    int maxx = x+R;
    int miny = y-R;
    int maxy = y+R;
    struct Node *tmp = new Node;
    tmp->cordinates[0] = x;
    tmp->cordinates[1] = y;
    validateCir(tmp,R,minx,maxx,miny,maxy,root);
}

void copyPoint(Node *root , Node *alt)
{
    for (int i=0; i<2; i++)
        root->cordinates[i] = alt->cordinates[i];
    root->bankName = alt->bankName ;
    root->name = alt->name ;
    alt->name="";
}

struct Node *findMin(Node *root)
{
    if(root->left!=NULL)
        return findMin(root->left);
    return root ;
}

struct Node *Delete(Node *root , int data[])
{
    if(root==NULL)
        return root ;
    if(root->cordinates[root->xOy] > data[root->xOy])
        root->left = Delete(root->left , data);
    else if(root->cordinates[root->xOy] < data[root->xOy])
        root->right = Delete(root->right,data);
    else
    {
        if(root->left == NULL && root->right == NULL)
        {
            delete root ;
            root = NULL ;
        }
        else if(root->left == NULL)
        {
            struct Node *temp = root ;
            root = root->right ;
            delete temp ;
        }
        else if(root->right == NULL)
        {
            struct Node *temp = root ;
            root = root->left ;
            delete temp ;
        }
        else 
        {
            struct Node *temp = findMin(root->right);
            copyPoint(root,temp);
            root->right = Delete(root->right,temp->cordinates) ;
        }
    }
    return root ;
}

struct Node *deleteBr(Node *kdbranch,TrieTreeB *trbanks , Node *kdBanks)
{
    if(kdbranch == NULL)
    {
        cout<<"\tThere isn't any branch yet\n";
        return kdbranch ;
    }
    int cordinate[2];
    cout<<"\tEnter Branch's cordinate\n\tX: ";
    cin>>cordinate[0];
    cout<<"\tY: ";
    cin>>cordinate[1];
    Node *root = kdSearch(kdBanks,cordinate);
    if(root != NULL)
    {
        cout<<"\tthis cordinate belonged to one Bank\n";
        return kdbranch ;
    }
    root = kdSearch(kdbranch,cordinate);
    if(root == NULL)
    {
        cout<<"\tthere isn't any Branch with this cordinate\n";
        return kdbranch ;
    }
    struct TrieTreeB *tmp = search(trbanks,root->bankName);
    if(tmp!=NULL)
    {
        if(tmp->branchs!=NULL)
        {
            tmp->branchs->left = Delete(tmp->branchs->left,cordinate);
            tmp->branchs->right = tmp->branchs->left ;
        }
    }
    kdbranch = Delete(kdbranch,cordinate);
    cout<<"\tBranch removed successfully\n";
    return kdbranch ;
}

int main()
{
    struct Node *kdBanks = NULL;
    struct Node *kdBranchs = NULL;
    struct TrieTreeN *treeN = createNaiveNeighbourhoodNode();
    struct TrieTreeB *treeBTBr = createNaiveBanks();
    string command;
    while(true)
    {

        cout<<"commands: addN addB addBr listB listBrs nearB nearBr availB delBr q-> for quit\n\n";
        cin>>command;
        if(command=="val")
            validateLikeInorder(kdBranchs) ;
        if(command=="addN")
            addNeighbourhood(treeN);    //done
        if(command=="addB")
            kdBanks = addBank(treeBTBr,kdBanks);      //done
        if(command=="addBr")
            kdBranchs = addBranch(kdBranchs,treeBTBr,kdBanks);      //done
        if(command=="listB")
            listBanksNeighbourhood(treeN,kdBanks);       //done
        if(command=="listBrs")
            listBranchBank(treeBTBr);       //done
        if(command=="nearB")
            nearestBank(kdBanks);     //done
        if(command=="nearBr")
            nearestBranch(kdBranchs);       //done
        if(command=="availB")
            availableBank(kdBanks);        //done
        if(command=="delBr")
            kdBranchs = deleteBr(kdBranchs,treeBTBr,kdBanks);
        if(command=="q")
            break;
    }
    return 0 ;
}