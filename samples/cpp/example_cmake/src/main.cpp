#include<opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>        //SIFT SURF

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"


#include<iostream>
#include<vector>


using namespace cv;
using namespace std;

void drawText(Mat &image);


void drawText(Mat &image) {
    putText(image, "Test CV",
            Point(20, 50),
            FONT_HERSHEY_COMPLEX, 1, // font face and scale
            Scalar(255, 255, 255), // white
            1, LINE_AA); // line thickness and type
}



//constexpr auto path0 = "F:\\workspace\\opencv\\2_xfeature2d\\pic\\0.png";
//constexpr auto path1 = "F:\\workspace\\opencv\\2_xfeature2d\\pic\\1.png";

Mat test_sift(Mat &image0, Mat &image1) {
//    cv::Mat image0 = cv::imread(path0, 1);
//    cv::Mat image1 = cv::imread(path1, 1);

//    cv::imshow("image0", image0);
//    cv::imshow("image1", image1);
    /*
    step1:特征检测器
    */
    cv::Ptr<cv::xfeatures2d::SURF> detector;
    detector = cv::xfeatures2d::SURF::create(800);  //800为海塞矩阵阈值，越大越精准

    /*
    -----SURF----
    cv::Ptr<cv::xfeatures2d::SURF> detector;
    detector = cv::xfeatures2d::SURF::create(800);  //800为海塞矩阵阈值，越大越精准
    -----SIFT-----
    cv::Ptr<cv::xfeatures2d::SIFT> detector;
    detector = cv::xfeatures2d::SIFT::create(800);//800为保留的点数
    -----ORB------
    cv::Ptr<cv::ORB> detector;
    detector  = cv::ORB::create(800);//保留点数
    -----STAR-----
    cv::Ptr<cv::xfeatures2d::StarDetector> detector;
    detector = cv::xfeatures2d::StarDetector::create();
    -----MSD-----
    cv::Ptr<cv::xfeatures2d::MSDDetector> detector;
    detector = cv::xfeatures2d::MSDDetector::create();
    */
    std::vector<cv::KeyPoint> key0;
    std::vector<cv::KeyPoint> key1;
    detector->detect(image0, key0, cv::noArray());
    detector->detect(image1, key1, cv::noArray());

    /*
    step2:描述子提取器
    */
    cv::Ptr<cv::xfeatures2d::SURF> Extractor;
    Extractor = cv::xfeatures2d::SURF::create(800);
    /*
       以下都是xfeature2d中的提取器
    -----SURF-----
    -----SIFT-----
    -----LUCID----
    -----BriefDescriptorExtractor----
    -----VGG-----
    -----BoostDesc-----
    */
    cv::Mat descriptor0, descriptor1;
    Extractor->compute(image0, key0, descriptor0);
    Extractor->compute(image1, key1, descriptor1);

    /*
    step3:匹配器
    */
    cv::BFMatcher matcher;//暴力匹配器
    std::vector<cv::DMatch> matches; // 存放匹配结果
    std::vector<cv::DMatch> good_matches; //存放好的匹配结果

    matcher.match(descriptor0, descriptor1, matches);
    std::sort(matches.begin(), matches.end());     //筛选匹配点，根据match里面特征对的距离从小到大排序

    int ptsPairs = std::min(50, (int) (matches.size() * 0.15));
    std::cout << "匹配点数为" << ptsPairs << std::endl;
    for (int i = 0; i < ptsPairs; i++) {
        good_matches.push_back(matches[i]);              //距离最小的50个压入新的DMatch
    }

    cv::Mat result;

    cv::drawMatches(image0, key0,
                    image1, key1,
                    good_matches, result,
                    cv::Scalar::all(-1), cv::Scalar::all(-1),
                    std::vector<char>(),
                    cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);  //绘制匹配点

//    cv::imshow("result", result);
    return result;
}


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