#include "imgviz.hpp"

int main(int argc, char **argv) {
  assert(argc == 2);

  std::string data_dir = std::string(argv[1]);
  cv::Mat bgr = cv::imread(data_dir + "/rgb.png");

  cv::Mat bgr90;
  cv::rotate(bgr, bgr90, cv::ROTATE_90_CLOCKWISE);

  bgr90 = imgviz::centerize(bgr90, bgr.size());

  bgr = imgviz::textInRectangle(bgr, "original", "lt");
  bgr90 = imgviz::textInRectangle(bgr90, "centerize", "lt");

  std::vector<cv::Mat> images = {bgr, bgr90};
  cv::Mat viz = imgviz::tile(images, /*shape=*/cv::Vec2i(1, 2));

  cv::imwrite("centerize.png", viz);
  cv::imshow(argv[0], viz);
  cv::waitKey(0);
}
