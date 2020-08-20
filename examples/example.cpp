#include <opencv2/opencv.hpp>

#include "imgviz.hpp"

int main(int argc, char **argv) {
  assert(argc == 2);

  std::string data_dir = std::string(argv[1]);
  cv::Mat bgr = cv::imread(data_dir + "/rgb.png");
  cv::Mat depth = cv::imread(data_dir + "/depth.png", -1);
  assert(depth.type() == CV_16UC1);
  depth.convertTo(depth, CV_32FC1);
  depth.setTo(NAN, depth == 0);
  depth /= 1000;

  cv::Mat depth_bgr = depthToBgr(depth, /*min_val=*/0.3, /*max_val=*/1);

  bgr = textInRectangle(bgr, "bgr", "lt");
  depth_bgr = textInRectangle(depth_bgr, "depth_bgr", "lt");

  std::vector<cv::Mat> images = {bgr, depth_bgr};
  cv::Mat tiled = tile(images, /*shape=*/cv::Vec2i(1, 2));

  cv::imshow("example", tiled);
  cv::waitKey(0);
}
