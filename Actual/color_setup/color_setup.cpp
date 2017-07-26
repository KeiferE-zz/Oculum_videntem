/* HSV Color Code:

    Color               Hue      Saturation    Value
    Red     ...     163 - 179    161 - 228   116 - 169
    Orange  ...     0 - 12       112 - 227   134 - 255
    Yellow  ...     22 - 34      38 - 244    160 - 255
    Green   ...     45 - 108     39 - 153    92 - 255
    Blue    ...     92 - 135     62 - 176    32 - 126
    Purple  ...     109 - 139    58 - 148    43 - 87

    **Note: Color May Vary depending on lighting!
*/
/*
    Includes
*/
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

/*
    Global Variables
*/
Mat imgOriginal;
Mat imgCircles;
Mat imgThresholded;

vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

//starting with orange, color vars
int iLowH = 0;
int iHighH = 14;

int iLowS = 201;
int iHighS = 255;

int iLowV = 180;
int iHighV = 255;

//functions
void color();
void display();
void image_converter();
void contour();
void draw();


int main()
{

    display();
    color();

    return 0;
}

void color()
{
    VideoCapture cam(0); //start video feed

    while(1)
    {
        cam.read(imgOriginal);

        imshow("Thresholded Image", imgThresholded); //show the thresholded image
        imshow("Camera", imgOriginal); //show the original image

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "Who pressed the ESC Key?!" << endl;
            destroyAllWindows();
            break;
        }
    }
}

void display()
{
    namedWindow("Control", CV_WINDOW_NORMAL); //create a window called "Control"
    namedWindow("Camera", CV_WINDOW_NORMAL);

    cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 179);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);
}

void image_converter()
{
    //ros stuff
}

void contour()
{
    drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
}

void draw()
{
    imgCircles = Mat::zeros( imgOriginal.size(), CV_8UC3 );
    Moments oMoments = moments(imgThresholded);

    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double dArea = oMoments.m00;
}