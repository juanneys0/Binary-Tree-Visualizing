#ifndef BIN_TREE_H
#define BIN_TREE_H

#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<functional>
#include<algorithm>
#include<cstdlib>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class BT {
public:
    struct BTNode;
    struct BTEdge;
    // draw matrix
    vector<vector<char>> pic_matrix;

    vector<BTEdge> BTEdgeVec;
    vector<BTNode*> BTNVec;
    vector<BTNode*> LeafNodeVec;

    vector<char> inorderSeq;
    vector<char> postorderSeq;
    vector<char> preorderSeq;

    int height = 0;
    int visitMode = 0;

    int readFromFile(string file_path); //return length of dataset
    void constructTree();

    void inorderTravRec(BTNode* node);
    void preorderTravRec(BTNode* node);
    void postorderTravRec(BTNode* node);

    void visit(BTNode* node); //do some operation, cout or count?


    int getHeight();
    void getLeafNodes();
    int getDepth(BTNode* node, BTNode* target, int depth);

    int search(vector<char> vec, char key); // get index of root
    vector<char> StrToVec(string str);
    void InPostConstructVec(vector<char> InVec, vector<char> PostVec);
    void InPredConstructVec(vector<char> InVec, vector<char> PreVec);
    void InPredConstructStr(string InStr, string PreStr);
    void InPostConstructStr(string InStr, string PostStr);

    void constructPicMatrix();
    void drawPic();
    void drawText(double scale, Mat& image, Point p1, Point p2, char textchar);
    Mat _drawViaCV(double scale); 
    void drawViaCV(); // with display
    void saveFig(string file_path, double scale);

    ~BT() {
        // for (size_t i = 0; i < BTNVec.size(); i++)
        // {
        //     delete BTNVec[i];
        // } 
    }
};


struct BT::BTEdge {
    char pa;
    char lc;
    char rc;

    BTEdge(char p, char l, char r) : pa(p), lc(l), rc(r) {}
};

struct BT::BTNode {
    char key;

    struct BTNode* ppa;
    struct BTNode* plc;
    struct BTNode* prc;

    int as_pa;
    int as_lc;
    int as_rc;

    int xind;
    int yind;

    BTNode() :ppa(nullptr), plc(nullptr), prc(nullptr) {};

    ~BTNode() {
        delete ppa;
        delete plc;
        delete prc;
    }
};

#endif // !BIN_TREE_H
