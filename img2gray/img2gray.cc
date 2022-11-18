#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
using namespace std;

int main() {
  cv::Mat img_raw = cv::imread("../raw.jpg", cv::IMREAD_GRAYSCALE);
  cv::Mat img_out;
  img_out.create(img_raw.size(), img_raw.type());

  for (int i = 0; i < img_raw.rows; i++)
    for (int j = 0; j < img_raw.cols; j++) {
      uint8_t data = img_raw.at<uint8_t>(i, j);
      if (data < 0xf0)
        img_out.at<uint8_t>(i, j) = 0;
      else
        img_out.at<uint8_t>(i, j) = data;
    }

  cv::imwrite("raw_1.jpg", img_out);
}