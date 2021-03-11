#include <jni.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <vector>

//
// Created by ahmedmamdouh13 on 3/11/21.
//
using namespace std;
using namespace cv;

RNG rng(12345);


extern "C"
JNIEXPORT jstring JNICALL
Java_com_ahmedmamdouh13_MainActivity_drawFeatures(JNIEnv *env, jobject thiz, jlong mat_rgb,
                                                  jlong mat_gray) {


    Mat &mGr = *(Mat *) mat_gray;
    Mat &mRgb = *(Mat *) mat_rgb;
    vector<vector<Point>> v;

    blur(mGr, mGr, Size(3,3) );


    Mat cannyOutput;

    int thresh = 100;

    Canny(mGr, cannyOutput, thresh, thresh*2,3);

    vector<Vec4i> hier;


    findContours(cannyOutput, v, hier, RETR_TREE, CHAIN_APPROX_NONE, Point(0,0));

//    Ptr<FeatureDetector> detector = FastFeatureDetector::create(60);
////  Ptr<SimpleBlobDetector> detector =  SimpleBlobDetector::create();
////    Ptr<FeatureDetector> detector = FastFeatureDetector::create(50);
//    detector->detect(mGr, v);

    for (int i = 0; i < v.size(); i++) {
//        const KeyPoint &kp = v[i - 1];
//        const KeyPoint &kp2 = v[i];
//        circle(mRgb, Point(kp.pt.x, kp.pt.y), 30, Scalar(255, 0, 0, 255));
//        line(mRgb, Point(kp.pt.x, kp.pt.y), Point(kp2.pt.x, kp2.pt.y), Scalar(255, 0, 0, 255));
//        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours(mRgb, v, i,Scalar(255, 0, 0, 255),2,8);
    }


}