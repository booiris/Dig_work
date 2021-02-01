#include <opencv2/opencv.hpp>

#include <string>
#include <cstdio>
#include <iostream>
using namespace std;
using namespace cv;

class img_Graph
{
    // private:
    //     double
public:
};

int main()
{
    Mat pic = imread("data.PNG", IMREAD_GRAYSCALE);
    cout << pic.channels() << endl;
    cout << pic.rows << " " << pic.cols << endl;

}