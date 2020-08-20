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

  cv::Mat depth_normalized =
      imgviz::normalize(depth, /*min_val=*/0.3, /*max_val=*/1);
  depth_normalized *= 255;
  depth_normalized.convertTo(depth_normalized, CV_8UC1);

  cv::Mat depth_bgr = imgviz::depthToBgr(depth, /*min_val=*/0.3, /*max_val=*/1);

  bgr = imgviz::textInRectangle(bgr, "bgr");
  depth_normalized =
      imgviz::textInRectangle(depth_normalized, "depth_normalized");
  depth_bgr = imgviz::textInRectangle(depth_bgr, "depth_bgr");

  std::vector<cv::Mat> images = {bgr, depth_normalized, depth_bgr};
  cv::Mat viz = imgviz::tile(images, /*shape=*/cv::Vec2i(1, 3));

  cv::imwrite("depth_to_bgr.png", viz);
  if (!std::getenv("CI")) {
    cv::imshow(argv[0], viz);
    cv::waitKey(0);
  }
}
