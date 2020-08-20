#include "imgviz.hpp"

int main(int argc, char **argv) {
  assert(argc == 2);

  std::string data_dir = std::string(argv[1]);
  cv::Mat bgr = cv::imread(data_dir + "/rgb.png");
  cv::Mat class_label = cv::imread(data_dir + "/class_label.png", -1);
  assert(class_label.type() == CV_16UC1);
  class_label.convertTo(class_label, CV_32SC1);

  cv::Mat gray;
  cv::cvtColor(bgr, gray, cv::COLOR_BGR2GRAY);
  cv::Mat class_label_bgr = imgviz::labelToBgr(class_label, gray);

  bgr = imgviz::textInRectangle(bgr, "bgr");
  class_label_bgr = imgviz::textInRectangle(class_label_bgr, "class_label");

  cv::Mat viz = imgviz::tile({bgr, class_label_bgr}, cv::Vec2i(1, 2));

  cv::imwrite("label_to_bgr.png", viz);
  cv::imshow("label_to_bgr", viz);
  cv::waitKey(0);
}
