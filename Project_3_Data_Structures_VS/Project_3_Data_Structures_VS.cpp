#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <chrono>
#include <map>
#include <unordered_map>
using namespace std;

class Product {						//make sure the types match up. so ids are strings and scores are ints, etc.
public:
    int ID = 0;
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

struct bestreviews {
    int operator()(Product* product1, Product* product2) {
        return product1->score < product2->score;
    }
};

struct worstreviews {
    int operator()(Product* product1, Product* product2) {
        return product1->score > product2->score;
    }
};

class MinHeap {
private:
    priority_queue<Product*, vector<Product*>, timecompare> pq;
public:
    priority_queue<Product*, vector<Product*>, timecompare> GetPQ() { return pq; }
    void SetPQ(priority_queue<Product*, vector<Product*>, timecompare> newpq) { pq = newpq; }
    void MakeHeap(vector<Product*>);
};

void MinHeap::MakeHeap(vector<Product*> products) {
    for (int i = 0; i < products.size(); i++) {
        pq.push(products[i]);
    }
}

map<string, priority_queue<Product*, vector<Product*>, timecompare>> MakeMap(vector<Product*> products) {
    map<string, priority_queue<Product*, vector<Product*>, timecompare>> mymap;
    for (int i = 0; i < products.size(); i++) {
        if (mymap.find(products[i]->productID) != mymap.end()) {
            mymap.find(products[i]->productID)->second.push(products[i]);
        }
        else {
            priority_queue<Product*, vector<Product*>, timecompare> temp;
            temp.push(products[i]);
            mymap.emplace(products[i]->productID, temp);
        }
    }
    return mymap;
}

unordered_map<string, priority_queue<Product*, vector<Product*>, timecompare>> MakeUnorderedMap(vector<Product*> products) {
    unordered_map<string, priority_queue<Product*, vector<Product*>, timecompare>> mymap;
    for (int i = 0; i < products.size(); i++) {
        if (mymap.find(products[i]->productID) != mymap.end()) {
            mymap.find(products[i]->productID)->second.push(products[i]);
        }
        else {
            priority_queue<Product*, vector<Product*>, timecompare> temp;
            temp.push(products[i]);
            mymap.emplace(products[i]->productID, temp);
        }
    }
    return mymap;
}

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

void PrintInOrderTree(Product* root) {
    if (!root) {
        return;
    }
    PrintInOrderTree(root->left);
    cout << root->time << " ";
    PrintInOrderTree(root->right);
}

void PrintHeap(MinHeap* heap) {
    priority_queue<Product*, vector<Product*>, timecompare> temp = heap->GetPQ();
    for (int i = 0; i < temp.size(); i++) {
        cout << temp.top()->ID;
        temp.pop();
    }
}

bool isValidName(string name, unordered_map<string, priority_queue<Product*, vector<Product*>, timecompare>> temp) {
    if (temp.find(name) != temp.end()) {
        return true;
    }
    return false;
}

void SearchTree(AVLTree* tree, string prodid, int num) {
    priority_queue<Product*, vector<Product*>, timecompare> products;
    queue<Product*> q;
    q.push(tree->GetRoot());

    while (!q.empty()) {
        if (q.front()->productID == prodid) {
            products.push(q.front());
        }
        if (q.front()->left) {
            q.push(q.front()->left);
        }
        if (q.front()->right) {
            q.push(q.front()->right);
        }
        q.pop();
    }

    int size = products.size();
    int tempsize = 0;
    int count = num;
    int sum = 0;
    int tempsum = 0;
    for (int i = 0; i < size; i++) {
        if (count == 0) {
            sum += products.top()->score;
        }
        else {
            cout << products.top()->score << " " << products.top()->summary << endl;
            sum += products.top()->score;
            tempsum += products.top()->score;
            tempsize++;
            products.pop();
            count--;
        }
    }
    cout << "Average score of reviews: " << float(tempsum) / tempsize << endl;
    cout << "Amazon average of reviews: " << float(sum) / size;
}

void SearchMinHeap(MinHeap* heap, string prodid, int num) {
    priority_queue<Product*, vector<Product*>, timecompare> temp = heap->GetPQ();
    int size = temp.size();
    int tempsize = 0;
    int totalsize = 0;
    int count = num;
    int sum = 0;
    int tempsum = 0;
    for (int i = 0; i < size; i++) {
        if (temp.top()->productID == prodid) {
            if (count == 0) {
                sum += temp.top()->score;
                totalsize++;
            }
            else {
                cout << temp.top()->score << " " << temp.top()->summary << endl;
                sum += temp.top()->score;
                tempsum += temp.top()->score;
                tempsize++;
                totalsize++;
                count--;
            }
        }
        temp.pop();
    }
    cout << "Average score of reviews: " << float(tempsum) / tempsize << endl;
    cout << "Amazon average of reviews: " << float(sum) / totalsize;
}

void SearchMap(map<string, priority_queue<Product*, vector<Product*>, timecompare>> map, string prodid, int num) {
    auto temp = map.find(prodid);
    int tempsize = 0;
    int totalsize = 0;
    int count = num;
    int sum = 0;
    int tempsum = 0;
    if (temp != map.end()) {
        int size = temp->second.size();
        for (int i = 0; i < size; i++) {
            if (count == 0) {
                sum += temp->second.top()->score;
                totalsize++;
            }
            else {
                cout << temp->second.top()->score << " " << temp->second.top()->summary << endl;
                sum += temp->second.top()->score;
                tempsum += temp->second.top()->score;
                tempsize++;
                totalsize++;
                count--;
            }
            temp->second.pop();
        }
    }
    cout << "Average score of reviews: " << float(tempsum) / tempsize << endl;
    cout << "Amazon average of reviews: " << float(sum) / totalsize;
}

void SearchUnorderedMap(unordered_map<string, priority_queue<Product*, vector<Product*>, timecompare>> map, string prodid, int num) {
    auto temp = map.find(prodid);
    int tempsize = 0;
    int totalsize = 0;
    int count = num;
    int sum = 0;
    int tempsum = 0;
    if (temp != map.end()) {
        int size = temp->second.size();
        for (int i = 0; i < size; i++) {
            if (count == 0) {
                sum += temp->second.top()->score;
                totalsize++;
            }
            else {
                cout << temp->second.top()->score << " " << temp->second.top()->summary << endl;
                sum += temp->second.top()->score;
                tempsum += temp->second.top()->score;
                tempsize++;
                totalsize++;
                count--;
            }
            temp->second.pop();
        }
    }
    cout << "Average score of reviews: " << float(tempsum) / tempsize << endl;
    cout << "Amazon average of reviews: " << float(sum) / totalsize;
}

void ReviewsTree(AVLTree* tree, string prodid, int bestworst, int num) {
    if (bestworst == 0) {
        priority_queue<Product*, vector<Product*>, bestreviews> products;
        queue<Product*> q;
        q.push(tree->GetRoot());

        while (!q.empty()) {
            if (q.front()->productID == prodid) {
                products.push(q.front());
            }
            if (q.front()->left) {
                q.push(q.front()->left);
            }
            if (q.front()->right) {
                q.push(q.front()->right);
            }
            q.pop();
        }

        int size = products.size();
        int tempsize = 0;
        int count = num;
        int sum = 0;
        int tempsum = 0;
        for (int i = 0; i < size; i++) {
            if (count == 0) {
                sum += products.top()->score;
            }
            else {
                cout << products.top()->score << " " << products.top()->summary << endl;
                sum += products.top()->score;
                tempsum += products.top()->score;
                tempsize++;
                products.pop();
                count--;
            }
        }
        cout << "Average score of reviews: " << float(tempsum) / tempsize << endl;
        cout << "Amazon average of reviews: " << float(sum) / size;
    }
    else {
        priority_queue<Product*, vector<Product*>, worstreviews> products;
        queue<Product*> q;
        q.push(tree->GetRoot());

        while (!q.empty()) {
            if (q.front()->productID == prodid) {
                products.push(q.front());
            }
            if (q.front()->left) {
                q.push(q.front()->left);
            }
            if (q.front()->right) {
                q.push(q.front()->right);
            }
            q.pop();
        }

        int size = products.size();
        int tempsize = 0;
        int count = num;
        int sum = 0;
        int tempsum = 0;
        for (int i = 0; i < size; i++) {
            if (count == 0) {
                sum += products.top()->score;
            }
            else {
                cout << products.top()->score << " " << products.top()->summary << endl;
                sum += products.top()->score;
                tempsum += products.top()->score;
                tempsize++;
                products.pop();
                count--;
            }
        }
        cout << "Average score of reviews: " << float(tempsum) / tempsize << endl;
        cout << "Amazon average of reviews: " << float(sum) / size;
    }
}

void ReviewsMinHeap(MinHeap* heap, string prodid, int bestworst, int num) {
    priority_queue<Product*, vector<Product*>, timecompare> temppq = heap->GetPQ();
    priority_queue<Product*, vector<Product*>, bestreviews> newheap;
    int tempsize = temppq.size();
    for (int i = 0; i < tempsize; i++) {
        if (temppq.top()->productID == prodid) {
            newheap.push(temppq.top());
        }
        temppq.pop();
    }

    int size = newheap.size();
    if (bestworst == 0) { //0 is best reviews
        int count = num;
        int sum = 0;
        int tempsum = 0;
        int totalsize = 0;
        int tempsize = 0;
        for (int i = 0; i < size; i++) {
            if (count == 0) {
                sum += newheap.top()->score;
                totalsize++;
            }
            else if(newheap.top()->productID == prodid) {
                cout << newheap.top()->score << " " << newheap.top()->summary << endl;
                sum += newheap.top()->score;
                tempsum += newheap.top()->score;
                tempsize++;
                totalsize++;
                count--;
            }
            newheap.pop();
        }
        cout << "Average score of reviews: " << float(tempsum) / tempsize << endl;
        cout << "Amazon average of reviews: " << float(sum) / totalsize;
    }
    else {
        int size = heap->GetPQ().size();
        priority_queue<Product*, vector<Product*>, worstreviews> newheap;
        for (int i = 0; i < size; i++) {
            newheap.push(heap->GetPQ().top());
            heap->GetPQ().pop();
        }
        
        int count = num;
        int sum = 0;
        int tempsum = 0;
        int totalsize = 0;
        int tempsize = 0;
        for (int i = 0; i < size; i++) {
            if (count == 0) {
                sum += newheap.top()->score;
                totalsize++;
            }
            else {
                cout << newheap.top()->score << " " << newheap.top()->summary << endl;
                sum += newheap.top()->score;
                tempsum += newheap.top()->score;
                tempsize++;
                totalsize++;
                count--;
            }
            newheap.pop();
        }
        cout << "Average score of reviews: " << float(tempsum) / tempsize << endl;
        cout << "Amazon average of reviews: " << float(sum) / totalsize;
    }
}

void ReviewsMap(map<string, priority_queue<Product*, vector<Product*>, timecompare>> map, string prodid, int bestworst, int num) {
    if (bestworst == 0) { //0 is the case for best reviews
        priority_queue<Product*, vector<Product*>, bestreviews> pq;
        auto temp = map.find(prodid)->second;
        int size = temp.size();
        for (int i = 0; i < size; i++) {
            pq.push(temp.top());
            temp.pop();
        }
        int pqsize = pq.size();
        int count = num;
        int sum = 0;
        int tempsum = 0;
        int totalsize = 0;
        int tempsize = 0;
        for (int i = 0; i < pqsize; i++) {
            if (count == 0) {
                sum += pq.top()->score;
                totalsize++;
            }
            else {
                cout << pq.top()->score << " " << pq.top()->summary << endl;
                sum += pq.top()->score;
                tempsum += pq.top()->score;
                tempsize++;
                totalsize++;
                count--;
            }
            pq.pop();
        }
        cout << "Average score of reviews: " << float(tempsum) / tempsize << endl;
        cout << "Amazon average of reviews: " << float(sum) / totalsize;
    }
    else {
        priority_queue<Product*, vector<Product*>, worstreviews> pq;
        auto temp = map.find(prodid)->second;
        int size = temp.size();
        for (int i = 0; i < size; i++) {
            pq.push(temp.top());
            temp.pop();
        }
        int pqsize = pq.size();
        int count = num;
        int sum = 0;
        int tempsum = 0;
        int totalsize = 0;
        int tempsize = 0;
        for (int i = 0; i < pqsize; i++) {
            if (count == 0) {
                sum += pq.top()->score;
                totalsize++;
            }
            else {
                cout << pq.top()->score << " " << pq.top()->summary << endl;
                sum += pq.top()->score;
                tempsum += pq.top()->score;
                tempsize++;
                totalsize++;
                count--;
            }
            pq.pop();
        }
        cout << "Average score of reviews: " << float(tempsum) / tempsize << endl;
        cout << "Amazon average of reviews: " << float(sum) / totalsize;
    }
}

void ReviewsUnorderedMap(unordered_map<string, priority_queue<Product*, vector<Product*>, timecompare>> map, string prodid, int bestworst, int num) {
    if (bestworst == 0) { //0 is the case for best reviews
        priority_queue<Product*, vector<Product*>, bestreviews> pq;
        auto temp = map.find(prodid)->second;
        int size = temp.size();
        for (int i = 0; i < size; i++) {
            pq.push(temp.top());
            temp.pop();
        }
        int pqsize = pq.size();
        int count = num;
        int sum = 0;
        int tempsum = 0;
        int totalsize = 0;
        int tempsize = 0;
        for (int i = 0; i < pqsize; i++) {
            if (count == 0) {
                sum += pq.top()->score;
                totalsize++;
            }
            else {
                cout << pq.top()->score << " " << pq.top()->summary << endl;
                sum += pq.top()->score;
                tempsum += pq.top()->score;
                tempsize++;
                totalsize++;
                count--;
            }
            pq.pop();
        }
        cout << "Average score of reviews: " << float(tempsum) / tempsize << endl;
        cout << "Amazon average of reviews: " << float(sum) / totalsize;
    }
    else {
        priority_queue<Product*, vector<Product*>, worstreviews> pq;
        auto temp = map.find(prodid)->second;
        int size = temp.size();
        for (int i = 0; i < size; i++) {
            pq.push(temp.top());
            temp.pop();
        }
        int pqsize = pq.size();
        int count = num;
        int sum = 0;
        int tempsum = 0;
        int totalsize = 0;
        int tempsize = 0;
        for (int i = 0; i < pqsize; i++) {
            if (count == 0) {
                sum += pq.top()->score;
                totalsize++;
            }
            else {
                cout << pq.top()->score << " " << pq.top()->summary << endl;
                sum += pq.top()->score;
                tempsum += pq.top()->score;
                tempsize++;
                totalsize++;
                count--;
            }
            pq.pop();
        }
        cout << "Average score of reviews: " << float(tempsum) / tempsize << endl;
        cout << "Amazon average of reviews: " << float(sum) / totalsize;
    }
}

int main()
{
    AVLTree* myTree = new AVLTree();
    MinHeap* myHeap = new MinHeap();
    vector<Product*> products;
    products = ReadCSV("C:/Users/rball/Data_Struct_Projects/Data/Reviews.csv");

    //make tree
    myTree->MakeTree(products);
    //PrintInOrderTree(myTree->GetRoot());

    //make heap
    myHeap->MakeHeap(products);
    //PrintHeap(myHeap);

    //make map
    map<string, priority_queue<Product*, vector<Product*>, timecompare>> mymap = MakeMap(products);

    //make unordered map
    unordered_map<string, priority_queue<Product*, vector<Product*>, timecompare>> myunorderedmap = MakeUnorderedMap(products);




    int option = 0;
    string productname = "";
    cout << "Amazon Recent Food Reviews!" << endl;
    while (option != 6) {
        cout << "***************************************************************************" << endl;                      //standard menu
        cout << "Please select what you would like to do." << endl;
        cout << "Option 1: Find all of the reviews for a product." << endl;
        cout << "Option 2: Find the most recent reviews for a product." << endl;
        cout << "Option 3: Find the best reviews for a product." << endl;
        cout << "Option 4: Find the worst reviews for a product." << endl;
        cout << "Option 5: Exit" << endl;
        cout << "***************************************************************************" << endl;
        cout << "I would like to choose ";

        //asks for option
        cin >> option;                                                                                               //catches the case for option 6, then asks for product id
        cout << endl;
        if (option == 5) {
            cout << "See you later!" << endl;
            break;
        }
        else {
            cout << "What is the ID of the product? ";
        }

        //asks for product name
        cin >> productname;
        cout << endl;

        //checks to see if the product id matches one in the data
        if (!isValidName(productname, myunorderedmap)) {
            cout << "There are currently no reviews for this product." << endl;
            cout << endl;
            continue;
        }

        int max = INT_MAX;
        auto time1 = chrono::system_clock::now();
        auto time2 = chrono::system_clock::now();
        chrono::duration<double> timedif = (time2 - time1);
        if (option == 1) {
            cout << "Using a tree, the ratings for this product are: " << endl;                           //This might not work for a tree, since it would take too long for the tree to search
            time1 = chrono::system_clock::now();
            SearchTree(myTree, productname, max);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << "---------------------------------------------------------------------------" << endl;

            cout << "Using a min heap, the ratings for this product are: " << endl;
            time1 = chrono::system_clock::now();
            SearchMinHeap(myHeap, productname, max);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << "---------------------------------------------------------------------------" << endl;

            cout << "Using a map, the ratings for this product are: " << endl;
            time1 = chrono::system_clock::now();
            SearchMap(mymap, productname, max);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << "---------------------------------------------------------------------------" << endl;

            cout << "Using an unordered map, the ratings for this product are: " << endl;
            time1 = chrono::system_clock::now();
            SearchUnorderedMap(myunorderedmap, productname, max);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << endl;
        }
        if (option == 2) {
            cout << "How many recent reviews would you like to view? ";
            int num = 0;
            cin >> num;
            cout << "---------------------------------------------------------------------------" << endl;

            cout << "Using a tree, the ratings for this product are: " << endl;                           //This might not work for a tree, since it would take too long for the tree to search
            time1 = chrono::system_clock::now();
            SearchTree(myTree, productname, num);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << "---------------------------------------------------------------------------" << endl;

            cout << "Using a min heap, the ratings for this product are: " << endl;
            time1 = chrono::system_clock::now();
            SearchMinHeap(myHeap, productname, num);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << "---------------------------------------------------------------------------" << endl;

            cout << "Using a map, the ratings for this product are: " << endl;
            time1 = chrono::system_clock::now();
            SearchMap(mymap, productname, num);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << "---------------------------------------------------------------------------" << endl;

            cout << "Using an unordered map, the ratings for this product are: " << endl;
            time1 = chrono::system_clock::now();
            SearchUnorderedMap(myunorderedmap, productname, num);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << endl;
        }
        if (option == 3) {
            cout << "How many recent reviews would you like to view? ";
            int num = 0;
            cin >> num;
            cout << endl;

            cout << "Using a tree, the ratings for this product are: " << endl;                           //This might not work for a tree, since it would take too long for the tree to search
            time1 = chrono::system_clock::now();
            ReviewsTree(myTree, productname, 0, num);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << "---------------------------------------------------------------------------" << endl;

            cout << "Using a min heap, the ratings for this product are: " << endl;
            time1 = chrono::system_clock::now();
            ReviewsMinHeap(myHeap, productname, 0, num);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << "---------------------------------------------------------------------------" << endl;

            cout << "Using a map, the ratings for this product are: " << endl;
            time1 = chrono::system_clock::now();
            ReviewsMap(mymap, productname, 0, num);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << "---------------------------------------------------------------------------" << endl;

            cout << "Using an unordered map, the ratings for this product are: " << endl;
            time1 = chrono::system_clock::now();
            ReviewsUnorderedMap(myunorderedmap, productname, 0, num);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << endl;
        }
        if (option == 4) {
            cout << "How many recent reviews would you like to view? ";
            int num = 0;
            cin >> num;
            cout << endl;

            cout << "Using a tree, the ratings for this product are: " << endl;                           //This might not work for a tree, since it would take too long for the tree to search
            time1 = chrono::system_clock::now();
            ReviewsTree(myTree, productname, 1, num);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << "---------------------------------------------------------------------------" << endl;

            cout << "Using a min heap, the ratings for this product are: " << endl;
            time1 = chrono::system_clock::now();
            ReviewsMinHeap(myHeap, productname, 1, num);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << "---------------------------------------------------------------------------" << endl;

            cout << "Using a map, the ratings for this product are: " << endl;
            time1 = chrono::system_clock::now();
            ReviewsMap(mymap, productname, 1, num);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << "---------------------------------------------------------------------------" << endl;

            cout << "Using an unordered map, the ratings for this product are: " << endl;
            time1 = chrono::system_clock::now();
            ReviewsUnorderedMap(myunorderedmap, productname, 1, num);
            time2 = chrono::system_clock::now();
            cout << endl;
            timedif = (time2 - time1);
            cout << "The time taken for this in ms is: " << (timedif.count() * 1000) << " ms" << endl;
            cout << endl;
        }
        cout << endl;
    }
    return 0;
}