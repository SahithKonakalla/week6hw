#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;

int main(int argc, char* argv[]) {
  std::string fileLocation = argv[1];
  Mat img_rgb = imread(fileLocation);

  Mat img_hsv;
  cvtColor(img_rgb, img_hsv, CV_BGR2HSV);
  std::vector<cv::Mat> channels;
  split(img_hsv, channels);

  Mat hueOrig = channels.at(0).clone();
  Mat threshLower, threshUpper;
  Mat result;

  threshold(hueOrig, threshLower, 0, 255, CV_THRESH_BINARY);
  threshold(hueOrig, threshUpper, 50, 255, CV_THRESH_BINARY_INV);

  result = threshLower & threshUpper;
  imshow("Hue", result);

  Mat edges;
  std::vector<std::vector<Point> > contours;

  Canny(result, edges, 0, 25, 3, false);

  imshow("Cannied", edges);

  cv::findContours(edges, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

  std::vector<Point> approxContour;

  for(int i = 0; i < contours.size(); i++) {

    approxPolyDP(contours[i], approxContour, cv::arcLength(cv::Mat(contours.at(i)), true) * 0.02, true);

    if (approxContour.size() == 8 && (cv::contourArea(approxContour, false) > 20000)) {

      cv::drawContours(img_rgb, contours, i , Scalar(255,0,0), 5);

      std::cout << cv::contourArea(approxContour, false);

      imshow("Drawn", img_rgb);
    }
  }
  waitKey(0);

}
