#include "BinTree.h"


int BT::readFromFile(string file_path)
{
    ifstream file(file_path);
    if (!file.is_open())
    {
        cerr << "Failed to open file" << endl;
        return -1;
    }

    char pa, lc, rc;
    while (file >> pa >> lc >> rc)
    {
        BTEdge BTEdgeInst(pa, lc, rc);
        BTEdgeVec.push_back(BTEdgeInst);
    }
    return BTEdgeVec.size();
}


void BT::constructTree()
{
    int len = BTEdgeVec.size();

    for (size_t i = 0; i < len; i++)
    {
        BTNode* BTN = new BTNode();
        BTN->key = BTEdgeVec[i].pa;
        if (BTEdgeVec[i].lc != '0' && BTEdgeVec[i].rc != '0')
            BTN->as_pa = 1;
        if (BTEdgeVec[i].lc == '0' && BTEdgeVec[i].rc == '0') {
            LeafNodeVec.push_back(BTN);
        }
        BTNVec.push_back(BTN);
    }

    for (size_t i = 0; i < BTNVec.size(); i++)
    {
        BTNVec[i]->as_lc = 0;
        BTNVec[i]->as_rc = 0;
        for (size_t j = 0; j < len; j++)
        {
            if (BTNVec[i]->key == BTEdgeVec[j].lc)
                BTNVec[i]->as_lc++;
            if (BTNVec[i]->key == BTEdgeVec[j].rc)
                BTNVec[i]->as_rc++;
        }

    }

    //check legality
    for (size_t i = 0; i < BTNVec.size(); i++)
    {
        int sum_of_rc_lc = BTNVec[i]->as_lc + BTNVec[i]->as_rc;
        if (sum_of_rc_lc > 1)
        {
            cerr << "Can't construct from illegal data! " << "error in: " << i << endl;
            cerr << "as_lc: " << BTNVec[i]->as_lc << " as_rc: " << BTNVec[i]->as_rc << endl;
            abort();
        }
    }

    BTNVec[0]->ppa = nullptr;
    // connect pointers
    for (size_t i = 0; i < len; i++)
    {
        for (size_t j = 0; j < BTNVec.size(); j++)
        {
            if (BTEdgeVec[i].lc == BTNVec[j]->key)
            {
                BTNVec[i]->plc = BTNVec[j];
                BTNVec[j]->ppa = BTNVec[i];
            }
            if (BTEdgeVec[i].rc == BTNVec[j]->key)
            {
                BTNVec[i]->prc = BTNVec[j];
                BTNVec[j]->ppa = BTNVec[i];
            }
        }

    }
}


void BT::visit(BTNode* node)
{
    if (visitMode == 0)
    {
        cout << node->key << " ";
    }

    if (visitMode == 1)  // pred
    {
        preorderSeq.push_back(node->key);
    }
    if (visitMode == 2) //inorder
    {
        inorderSeq.push_back(node->key);
    }
    if (visitMode == 3) //post
    {
        postorderSeq.push_back(node->key);
    }
}

void BT::inorderTravRec(BTNode* node)
{
    if (node == nullptr)
        return;

    inorderTravRec(node->plc);
    visit(node);
    inorderTravRec(node->prc);
}

void BT::preorderTravRec(BTNode* node)
{
    if (node == nullptr)
        return;
    visit(node);
    preorderTravRec(node->plc);
    preorderTravRec(node->prc);
}

void BT::postorderTravRec(BTNode* node)
{
    if (node == nullptr)
        return;
    postorderTravRec(node->plc);
    postorderTravRec(node->prc);
    visit(node);
}


void BT::getLeafNodes()
{
    for (size_t i = 0; i < LeafNodeVec.size(); i++)
    {
        cout << LeafNodeVec[i]->key << " ";
    }
    cout << endl;
}

int BT::getDepth(BTNode* node, BTNode* target, int depth = 0)
{
    if (node == nullptr)
        return -1;
    if (node == target)
        return depth;
    int leftDepth = getDepth(node->plc, target, depth + 1);
    if (leftDepth != -1)
        return leftDepth;
    int rightDepth = getDepth(node->prc, target, depth + 1);
    if (rightDepth != -1)
        return rightDepth;
    return -1;
}

int BT::getHeight()
{
    int max = 0;
    for (size_t i = 0; i < LeafNodeVec.size(); i++)
    {
        int depth = getDepth(BTNVec[0], LeafNodeVec[i]);
        if (depth > max)
            max = depth;
    }
    height = max;
    return max;
}

int BT::search(vector<char> vec, char key) {
    for (size_t i = 0; i < vec.size(); i++)
    {
        if (vec[i] == key)
        {
            return i;
        }

    }
    return -1;
}

void BT::InPostConstructVec(vector<char> InVec, vector<char> PostVec) {
    if (InVec.size() < 1 || PostVec.size() < 1)
    {
        return;
    }

    char root = PostVec.back();
    int root_idx = search(InVec, root); // the position of root in PostVec unknow
    if (root_idx == -1)
    {
        cerr << "Invalid given sequence!" << endl;
        abort();
    }
    vector<char> InLcVec(InVec.begin(), InVec.begin() + root_idx);
    vector<char> InRcVec(InVec.begin() + root_idx + 1, InVec.end());
    vector<char> PoLcVec(PostVec.begin(), PostVec.begin() + root_idx);
    vector<char> PoRcVec(PostVec.begin() + root_idx, PostVec.end() - 1);
    char lc_root, rc_root;
    if (PoLcVec.empty())
        lc_root = '0';
    else
        lc_root = PoLcVec.back();
    if (PoRcVec.empty())
        rc_root = '0';
    else
        rc_root = PoRcVec.back();
    BTEdge BTEdgeInst(root, lc_root, rc_root);
    BTEdgeVec.push_back(BTEdgeInst);
    // recursion
    InPostConstructVec(InLcVec, PoLcVec); // left subtree
    InPostConstructVec(InRcVec, PoRcVec); // right subtree
}

void BT::InPredConstructVec(vector<char> InVec, vector<char> PredVec) {
    if (InVec.size() < 1 || PredVec.size() < 1)
    {
        return;
    }

    char root = PredVec.front();
    int root_idx = search(InVec, root); // the position of root in PostVec unknow
    if (root_idx == -1)
    {
        cerr << "Invalid given sequence!" << endl;
        abort();
    }
    vector<char> InLcVec(InVec.begin(), InVec.begin() + root_idx);
    vector<char> InRcVec(InVec.begin() + root_idx + 1, InVec.end());
    vector<char> PrLcVec(PredVec.begin() + 1, PredVec.begin() + root_idx + 1);
    vector<char> PrRcVec(PredVec.begin() + root_idx + 1, PredVec.end());
    char lc_root, rc_root;
    if (PrLcVec.empty())
        lc_root = '0';
    else
        lc_root = PrLcVec.front();
    if (PrRcVec.empty())
        rc_root = '0';
    else
        rc_root = PrRcVec.front();
    BTEdge BTEdgeInst(root, lc_root, rc_root);
    BTEdgeVec.push_back(BTEdgeInst);
    // recursion
    InPredConstructVec(InLcVec, PrLcVec); // left subtree
    InPredConstructVec(InRcVec, PrRcVec); // right subtree
}

void BT::constructPicMatrix()
{
    char fillingChar;
    fillingChar = ' ';
    inorderSeq.clear();
    visitMode = 2;
    inorderTravRec(BTNVec[0]);
    int x_len = inorderSeq.size() * 2;
    visitMode = 0; //recover
    int y_len = getHeight() * 2;
    pic_matrix.resize(y_len + 1, vector<char>(x_len + 1, fillingChar)); // initialize
    for (size_t i = 0; i < BTNVec.size(); i++)
    {
        BTNVec[i]->xind = search(inorderSeq, BTNVec[i]->key) * 2;
        BTNVec[i]->yind = getDepth(BTNVec[0], BTNVec[i]) * 2;
    }
    for (size_t i = 0; i < BTNVec.size(); i++)
    {
        pic_matrix[BTNVec[i]->yind][BTNVec[i]->xind] = BTNVec[i]->key;
        if (BTNVec[i]->as_lc == 1)
        {
            pic_matrix[BTNVec[i]->yind - 1][BTNVec[i]->xind] = '|';
            int x_offset = 0;
            while (pic_matrix[BTNVec[i]->yind - 2][BTNVec[i]->xind + x_offset] == ' ')
            {
                pic_matrix[BTNVec[i]->yind - 2][BTNVec[i]->xind + x_offset] = '-';
                x_offset++;
            }
        }
        if (BTNVec[i]->as_rc == 1)
        {
            pic_matrix[BTNVec[i]->yind - 1][BTNVec[i]->xind] = '|';
            int x_offset = 0;
            while (pic_matrix[BTNVec[i]->yind - 2][BTNVec[i]->xind - x_offset] == ' ')
            {
                pic_matrix[BTNVec[i]->yind - 2][BTNVec[i]->xind - x_offset] = '-';
                x_offset++;
            }
        }
    }
}

void BT::drawPic()
{
    cout << endl << "------The Configuration of Binary Tree------" << endl << endl;
    for (size_t i = 0; i < pic_matrix.size(); i++)
    {
        for (size_t j = 0; j < pic_matrix[i].size(); j++)
        {
            cout << pic_matrix[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

vector<char> BT::StrToVec(string str)
{
    vector<char> vec(str.length());
    for (size_t i = 0; i < str.length(); i++)
    {
        vec[i] = str[i];
    }
    return vec;
}

void BT::InPredConstructStr(string InStr, string PreStr) {
    vector<char> InVec = StrToVec(InStr);
    vector<char> PreVec = StrToVec(PreStr);
    InPredConstructVec(InVec, PreVec);
}


void BT::InPostConstructStr(string InStr, string PostStr) {
    vector<char> InVec = StrToVec(InStr);
    vector<char> PostVec = StrToVec(PostStr);
    InPostConstructVec(InVec, PostVec);
}

void BT::drawViaCV()
{
    double scale = 3;
    Mat image = _drawViaCV(scale);
    const char* window_name = "Binary Tree Image";
    namedWindow(window_name, WINDOW_AUTOSIZE);
    imshow(window_name, image);
    cout << "Press any key to save and exit..." << endl;
    waitKey(0);
}

void BT::drawText(double scale, Mat& image, Point p1, Point p2, char textchar)
{
    string text(1, textchar);
    int fontFace = FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.5 * scale;
    int thickness = 2;
    Size textSize = getTextSize(text, fontFace, fontScale, thickness, nullptr);
    Point textOrg((p1.x + p2.x - textSize.width) / 2, (2 * p2.y - textSize.height) / 2);
    putText(image, text, textOrg, fontFace, fontScale, Scalar(0, 0, 0), thickness);
}

Mat BT::_drawViaCV(double scale)
{
    double triangScale = 0.05;
    int cellSize = 20; 
    vector<vector<char>>  charMatrix = pic_matrix;
    int width = static_cast<int>((charMatrix[0].size() + 1) * cellSize * scale);
    int height = static_cast<int>((charMatrix.size() + 2) * cellSize * scale);

    // create image
    Mat image = Mat::zeros(height, width, CV_8UC3);
    image.setTo(Scalar(255, 255, 255)); 

    for (size_t i = 0; i < charMatrix.size(); i++)
    {
        for (size_t j = 0; j < charMatrix[i].size(); j++)
        {
            vector<Point> triangPoint;
            int x_offset = 2;
            if (charMatrix[i][j] != ' ')
            {
                // point∂ÓÕ‚+1±‹√‚ªÊ÷∆æÿ–ŒøÚÕº‘⁄Õº∆¨±ﬂΩÁ
                Point p1((j + 1) * cellSize * scale, (i + 1) * cellSize * scale);
                Point p2((j + 2) * cellSize * scale, (i + 2) * cellSize * scale);
                if (charMatrix[i][j] != '|' && charMatrix[i][j] != '-') {
                    rectangle(image, p1, p2, Scalar(0, 0, 0));
                    drawText(scale, image, p1, p2, charMatrix[i][j]);
                }
                if (charMatrix[i][j] == '|')
                {
                    Point pl1((j + 1.5) * cellSize * scale, (i + 1) * cellSize * scale);
                    Point pl2((j + 1.5) * cellSize * scale, (i + 2) * cellSize * scale);
                    line(image, pl1, pl2, Scalar(0, 0, 0));
                    Point left((j + 1.5 - triangScale * 1) * cellSize * scale, (i + 2 - triangScale * sqrt(3)) * cellSize * scale);
                    Point right((j + 1.5 + triangScale * 1) * cellSize * scale, (i + 2 - triangScale * sqrt(3)) * cellSize * scale);
                    triangPoint.push_back(pl2);
                    triangPoint.push_back(left);
                    triangPoint.push_back(right);
                    vector<vector<Point>> Triangles;
                    Triangles.push_back(triangPoint);
                    fillPoly(image, Triangles, Scalar(0, 0, 0));
                }
                if (charMatrix[i][j] == '-')
                {
                    if (charMatrix[i + 1][j] == '|')
                    {
                        if (charMatrix[i][j + 1] == '-')
                            x_offset = 2;
                        else
                            x_offset = 1;
                        Point pl1((j + 1.5) * cellSize * scale, (i + 1.5) * cellSize * scale);
                        Point pl2((j + x_offset) * cellSize * scale, (i + 1.5) * cellSize * scale);
                        line(image, pl1, pl2, Scalar(0, 0, 0));
                        Point pl3((j + 1.5) * cellSize * scale, (i + 2) * cellSize * scale);
                        line(image, pl3, pl1, Scalar(0, 0, 0));
                    }
                    else
                    {
                        Point pl1((j + 1) * cellSize * scale, (i + 1.5) * cellSize * scale);
                        Point pl2((j + 2) * cellSize * scale, (i + 1.5) * cellSize * scale);
                        line(image, pl1, pl2, Scalar(0, 0, 0));
                    }
                }
            }
        }

    }
    return image;
}

void BT::saveFig(string file_path, double scale)
{
    Mat image = _drawViaCV(scale);
    cout << "Press any key to save and exit..." << endl;
    waitKey(0);

    // save
    imwrite(file_path, image);
}