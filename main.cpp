#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include"BinTree.h"

using namespace cv;
using namespace std;


int main() {
    
    string in_str = "gjakbehidcf";
    string po_str = "gajkhecfdib";
    string pre_str = "bkjgaiehdfc";

    int mode = 2;
    BT t1;
    // create tree via sequence
    if (mode == 1 || mode == 3) {
        t1.InPredConstructStr(in_str, pre_str);
        t1.constructTree();
        t1.constructPicMatrix();
        t1.drawViaCV();
        t1.saveFig("G:\\DS\\tree\\bt1.jpg", 10.0); // path and scale(resolution)
    }
    if (mode == 2) {
        BT t2;
        t2.readFromFile("G:\\DS\\tree\\t2_data.txt"); // using file to constrcut tree
        t2.constructTree();
        t2.constructPicMatrix();
        t2.drawViaCV();
        t2.saveFig("G:\\DS\\tree\\bt2.jpg", 10.0);
    }
    if (mode == 3)
    {
        t1.drawPic(); // console output
    }
    
    return 0;
}