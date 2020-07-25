#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

#include "src/test_sift.cpp"

using namespace cv;
using namespace std;

void drawText(Mat &image);

int main() {
    cout << "Built with OpenCV " << CV_VERSION << endl;
    Mat image1;
    VideoCapture capture1;
    capture1.open(0);

    Mat image2;
    VideoCapture capture2;
    capture2.open(1);


    if (capture1.isOpened()) {
        cout << "Capture is opened" << endl;
        for (;;) {
            capture1 >> image1;
//            if (image1.empty())
//                break;



            capture2 >> image2;
            if (image1.empty() || image2.empty())
                break;



//            drawText(image1);
//            drawText(image2);
//            imshow("Sample1", image1);
//            imshow("Sample2", image2);

            Mat result = test_sift(image1, image2);
            imshow("Sample1", result);
            if (waitKey(10) >= 0)
                break;
        }
    } else {
        cout << "No capture" << endl;
        image1 = Mat::zeros(480, 640, CV_8UC1);
        drawText(image1);
        imshow("Sample", image1);
        waitKey(0);
    }
    return 0;
}

void drawText(Mat &image) {
    putText(image, "Test CV",
            Point(20, 50),
            FONT_HERSHEY_COMPLEX, 1, // font face and scale
            Scalar(255, 255, 255), // white
            1, LINE_AA); // line thickness and type
}

