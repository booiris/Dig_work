#ifndef _HOUGH_H
#define _HOUGH_H
struct point
{
    int x, y;
};
void hough(cv::Mat &out, cv::Mat &pic, int sx, int ex, int r, int g, int b, int e, std::vector<point> &ans);

#endif