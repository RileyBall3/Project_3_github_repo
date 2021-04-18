#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <chrono>
using namespace std;

class Product {						//make sure the types match up. so ids are strings and scores are ints, etc.
public:
    int ID;
    string productID = "";
    string userID = "";
    string profileName = "";
    int helpNum = 0;
    int helpDenom = 0;
    int score = 0;
    int time = 0;
    string summary = "";
    string text = "";
    Product* left = nullptr;
    Product* right = nullptr;
};

class AVLTree {
private:
    Product* root;
public:
    AVLTree() : root(nullptr) {}
    AVLTree(Product* temp) : root(temp) {}
    void MakeTree(vector<Product*> products);
    Product* GetRoot();
    void SetRoot(Product* temp);
    int Height(Product* root);
    Product* RotateLeft(Product* root);
    Product* RotateRight(Product* root);
    Product* RotateLeftRight(Product* root);
    Product* RotateRightLeft(Product* root);
    Product* Balance(Product* root);
    Product* Insert(Product* root, Product* input);
    Product* InsertAndBalance(Product* root, Product* input);
};

Product* AVLTree::GetRoot() { //root getter
    return root;
}

void AVLTree::SetRoot(Product* temp) { //root setter
    root = temp;
}

int AVLTree::Height(Product* root) { //Finds the height of root so the tree can be balanced.
    if (!root) {
        return 0;
    }
    return 1 + max(Height(root->left), Height(root->right)); //returns the max height of each subtree
}

Product* AVLTree::RotateLeft(Product* root) { //standard left rotation
    Product* temp = root->right;
    root->right = root->right->left;
    temp->left = root;
    return temp;
}

Product* AVLTree::RotateRight(Product* root) { //standard right rotation
    Product* temp = root->left;
    root->left = root->left->right;
    temp->right = root;
    return temp;
}

Product* AVLTree::RotateLeftRight(Product* root) { //standard left right rotation 
    Product* temp = root->left;
    root->left = temp->right;
    temp->right = temp->right->left;
    root->left->left = temp;
    temp = root->left;
    root->left = temp->right;
    temp->right = root;
    return temp;
}

Product* AVLTree::RotateRightLeft(Product* root) { //standard right left rotaion 
    Product* temp = root->right;
    root->right = temp->left;
    temp->left = temp->left->right;
    root->right->right = temp;
    temp = root->right;
    root->right = temp->left;
    temp->left = root;
    return temp;
}

Product* AVLTree::Balance(Product* root) {
    if (!root) {
        return nullptr;
    }
    if ((Height(root->left) - Height(root->right)) < -1) { //when the tree has a right imbalance
        if (root->right->left) { //The product causing an imbalance can either be on the left, right, or not exist
            if (root->right->left->left) {
                root->right = RotateRight(root->right);
            }
            else if (root->right->left->right) {
                root->right = RotateLeftRight(root->right);
            }
            else {
                root = RotateRightLeft(root);
            }
        }
        else { //The product causing an imbalance can be either on the right, left, or not exist
            if (root->right->right->right) {
                root->right = RotateLeft(root->right);
            }
            else if (root->right->right->left) {
                root->right = RotateRightLeft(root->right);
            }
            else {
                root = RotateLeft(root);
            }
        }
        root = Balance(root);
    }
    if ((Height(root->left) - Height(root->right)) > 1) { //when the tree has a left imbalance
        if (root->left->right) { //The product causing an imbalance can be either on the right, left, or not exist
            if (root->left->right->right) {
                root->left = RotateLeft(root->left);
            }
            else if (root->left->right->left) {
                root->left = RotateRightLeft(root->left);
            }
            else {
                root = RotateLeftRight(root);
            }
        }
        else { //The product causing an imbalance can either be on the left, right, or not exist
            if (root->left->left->left) {
                root->left = RotateRight(root->left);
            }
            else if (root->left->left->right) {
                root->left = RotateLeftRight(root->left);
            }
            else {
                root = RotateRight(root);
            }
        }
        root = Balance(root);
    }
    root->left = Balance(root->left);
    root->right = Balance(root->right);
    return root;
}

Product* AVLTree::Insert(Product* root, Product* input) { //Inserts a Product with an already valid id
    if (!root) {
        return input;
    }
    if (input->time < root->time) {
        root->left = Insert(root->left, input);
    }
    if (input->time > root->time) {
        root->right = Insert(root->right, input);
    }
    return root;
}

Product* AVLTree::InsertAndBalance(Product* root, Product* product) { //Called this method instead of calling insert and balance every time I insert a Product
    root = Insert(root, product);
    root = Balance(root);
    return root;
}

void AVLTree::MakeTree(vector<Product*> products) {
    for (int i = 0; i < products.size(); i++) {
        root = InsertAndBalance(root, products[i]);
    }
}

struct timecompare {
    int operator()(Product* product1, Product* product2) {
        return product1->time > product2->time;
    }
};

class MinHeap {
private:
    priority_queue<Product, vector<Product>, timecompare> pq;
public:
    priority_queue<Product, vector<Product>, timecompare> GetPQ() { return pq; }
    void SetPQ(priority_queue<Product, vector<Product>, timecompare> newpq) { pq = newpq; }
};

vector<Product*> ReadCSV(string filename) {
    vector<Product*> tempvec;
    ifstream filestream(filename);
    string filestring = "";
    getline(filestream, filestring);
    int i = 0;                                                              //for testing purposes

    while (filestream) {
        if (i == 20) {
            break;
        }
        i++;

        getline(filestream, filestring);
        stringstream linestream(filestring);
        Product* newProduct = new Product();

        //ID
        string id = "";
        getline(linestream, id, ',');
        newProduct->ID = stoi(id);

        //Product ID
        string pid = "";
        getline(linestream, pid, ',');
        newProduct->productID = pid;

        //User ID
        string uid = "";
        getline(linestream, uid, ',');
        newProduct->userID = uid;

        //Profile Name and Helpfulness Numerator
        string n = "";
        string hn = "";
        getline(linestream, n, ',');
        getline(linestream, hn, ',');
        while (!isdigit(hn[0])) {
            n.append("," + hn);
            getline(linestream, hn, ',');
        }

        newProduct->profileName = n;

        newProduct->helpNum = stoi(hn);

        //Helpfulness Denominator
        string hd = "";
        getline(linestream, hd, ',');
        newProduct->helpDenom = stoi(hd);

        //Score
        string s = "";
        getline(linestream, s, ',');
        newProduct->score = stoi(s);

        //Time
        string t = "";
        getline(linestream, t, ',');
        newProduct->time = stoi(t);

        //Summary
        string sum = "";						//figure out how to catch the case with commas in the text
        getline(linestream, sum, ',');
        newProduct->summary = sum;

        //Text
        string txt = "";
        getline(linestream, txt, ',');
        newProduct->text = txt;

        tempvec.push_back(newProduct);
    }
    filestream.close();
    return tempvec;
}

void PrintInOrder(Product* root) {
    if (!root) {
        return;
    }
    PrintInOrder(root->left);
    cout << root->time << " ";
    PrintInOrder(root->right);
}

int main()
{
    AVLTree* myTree = new AVLTree();
    MinHeap* myHeap = new MinHeap();
    vector<Product*> products;
    products = ReadCSV("C:/Users/rball/Data_Struct_Projects/Data/Reviews.csv");
    //cout << products[0]->productID;
    cout << "yes";
    myTree->MakeTree(products);
    PrintInOrder(myTree->GetRoot());
    return 0;
}