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

  cv::Mat depth_normalized = normalize(depth, /*min_val=*/0.3, /*max_val=*/1);

  printMat("bgr             ", bgr);
  printMat("depth           ", depth);
  printMat("depth_normalized", depth_normalized);

  cv::Mat depth_gray;
  depth_gray = depth_normalized * 255;
  depth_gray.convertTo(depth_gray, CV_8UC1);

  cv::Mat depth_bgr;
  cv::applyColorMap(depth_gray, depth_bgr, cv::COLORMAP_JET);
  depth_bgr.setTo(0, ~isfinite(depth));

  std::vector<cv::Mat> images = {bgr, depth_gray, depth_bgr};
  cv::Mat tiled = tile(images, /*shape=*/std::make_tuple(1, 3));

  cv::imshow("bgr, depth_gray, depth_bgr", tiled);
  cv::waitKey(0);
}
