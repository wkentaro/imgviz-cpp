#include "imgviz.hpp"

int main(int argc, char **argv) {
  assert(argc == 2);

  std::string data_dir = std::string(argv[1]);
  cv::Mat bgr = cv::imread(data_dir + "/rgb.png");
  cv::Mat class_label = cv::imread(data_dir + "/class_label.png", -1);
  assert(class_label.type() == CV_16UC1);
  class_label.convertTo(class_label, CV_32SC1);
  cv::Mat depth = cv::imread(data_dir + "/depth.png", -1);
  assert(depth.type() == CV_16UC1);
  depth.convertTo(depth, CV_32FC1);
  depth.setTo(NAN, depth == 0);
  depth /= 1000;

  cv::Mat depth_normalized =
      imgviz::normalize(depth, /*min_val=*/0.3, /*max_val=*/1);
  depth_normalized *= 255;
  depth_normalized.convertTo(depth_normalized, CV_8UC1);

  cv::Mat depth_bgr = imgviz::depthToBgr(depth, 0.3, 1);

  cv::Mat gray;
  cv::cvtColor(bgr, gray, cv::COLOR_BGR2GRAY);
  cv::Mat class_label_bgr = imgviz::labelToBgr(class_label, gray);

  bgr = imgviz::textInRectangle(bgr, "bgr");
  depth_normalized =
      imgviz::textInRectangle(depth_normalized, "depth_normalized");
  depth_bgr = imgviz::textInRectangle(depth_bgr, "depth");
  class_label_bgr = imgviz::textInRectangle(class_label_bgr, "class_label");

  cv::Mat viz = imgviz::tile(
      {bgr, depth_normalized, depth_bgr, class_label_bgr}, cv::Vec2i(1, 4));

  cv::imwrite("getting_started.png", viz);
  if (!std::getenv("CI")) {
    cv::imshow(argv[0], viz);
    cv::waitKey(0);
  }
}
