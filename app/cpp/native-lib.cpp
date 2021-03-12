#include <jni.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <vector>

//
// Created by ahmedmamdouh13 on 3/11/21.
//
using namespace std;
using namespace cv;

static void findSquares(const Mat& image, vector<vector<Point> >& squares);


extern "C"
JNIEXPORT void JNICALL
Java_com_ahmedmamdouh13_MainActivity_drawFeatures(JNIEnv *env, jobject thiz, jlong mat_rgb,
                                                  jlong mat_gray) {

    Mat &mGr = *(Mat *) mat_gray;
    Mat &mRgb = *(Mat *) mat_rgb;
    vector<vector<Point>> v;

//    vector<vector<Point> > squares;
//    findSquares(mRgb, squares);

//    polylines(mRgb, squares, true, Scalar(0, 255, 0), 3, LINE_AA);

//
    blur(mGr, mGr, Size(3,3));


    Mat cannyOutput;

    int thresh = 100;

    Canny(mGr, cannyOutput, thresh, thresh*2,3);

    vector<Vec4i> hier;


    findContours(cannyOutput, v, hier, RETR_TREE, CHAIN_APPROX_NONE, Point(0,0));

////    Ptr<FeatureDetector> detector = FastFeatureDetector::create(60);
//////  Ptr<SimpleBlobDetector> detector =  SimpleBlobDetector::create();
//////    Ptr<FeatureDetector> detector = FastFeatureDetector::create(50);
////    detector->detect(mGr, v);
//
    for (int i = 0; i < v.size(); i++) {
////        const KeyPoint &kp = v[i - 1];
////        const KeyPoint &kp2 = v[i];
////        circle(mRgb, Point(kp.pt.x, kp.pt.y), 30, Scalar(255, 0, 0, 255));
////        line(mRgb, Point(kp.pt.x, kp.pt.y), Point(kp2.pt.x, kp2.pt.y), Scalar(255, 0, 0, 255));
////        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//

        drawContours(mRgb, v, i,Scalar(255, 0, 0, 255),2,8);
    }


}




int thresh = 50, N = 11;
const char* wndname = "Square Detection Demo";
// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}
// returns sequence of squares detected on the image.
static void findSquares( const Mat& image, vector<vector<Point> >& squares )
{
    squares.clear();
    Mat pyr, timg, gray0(image.size(), CV_8U), gray;
    // down-scale and upscale the image to filter out the noise
    pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
    pyrUp(pyr, timg, image.size());
    vector<vector<Point> > contours;
    // find squares in every color plane of the image
//    for( int c = 0; c < 3; c++ )
//    {
int c = 0;
         int ch[] = {c, 0};
        mixChannels(&timg, 1, &gray0, 1, ch, 1);
        // try several threshold levels
//        for( int l = 0; l < N; l++ )
//        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            int l = 0;
    Canny(gray0, gray, 0, thresh, 5);
    dilate(gray, gray, Mat(), Point(-1, -1));
    // find contours and store them all as a list
            findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
            vector<Point> approx;
            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(contours[i], approx, arcLength(contours[i], true)*0.02, true);
                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                    fabs(contourArea(approx)) > 1000 &&
                    isContourConvex(approx) )
                {
                    double maxCosine = 0;
                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }
                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( maxCosine < 0.3 )
                        squares.push_back(approx);
                }
//            }
        }
//    }
}