#include "imgviz.hpp"

int main(int argc, char **argv) {
  assert(argc == 2);

  std::string data_dir = std::string(argv[1]);
  cv::Mat bgr = cv::imread(data_dir + "/rgb.png");

  cv::Mat gray;
  cv::cvtColor(bgr, gray, cv::COLOR_BGR2GRAY);

  cv::Mat edges;
  cv::Canny(gray, edges, /*threshold1=*/50, /*threshold2=*/150);

  cv::Mat hsv;
  cv::cvtColor(bgr, hsv, cv::COLOR_BGR2HSV);

  cv::Mat xyz;
  cv::cvtColor(bgr, xyz, cv::COLOR_BGR2XYZ);

  cv::Mat yuv;
  cv::cvtColor(bgr, yuv, cv::COLOR_BGR2YUV);

  cv::Mat lab;
  cv::cvtColor(bgr, lab, cv::COLOR_BGR2Lab);

  cv::Mat luv;
  cv::cvtColor(bgr, luv, cv::COLOR_BGR2Luv);

  bgr = imgviz::textInRectangle(bgr, "bgr");
  gray = imgviz::textInRectangle(gray, "gray");
  edges = imgviz::textInRectangle(edges, "edges");
  hsv = imgviz::textInRectangle(hsv, "hsv");
  xyz = imgviz::textInRectangle(xyz, "xyz");
  yuv = imgviz::textInRectangle(yuv, "yuv");
  lab = imgviz::textInRectangle(lab, "lab");
  luv = imgviz::textInRectangle(luv, "luv");

  std::vector<cv::Mat> images = {bgr, gray, edges, hsv, xyz, yuv, lab, luv};
  cv::Mat viz = imgviz::tile(images, /*shape=*/cv::Vec2i(2, 4));

  cv::imwrite("tile.png", viz);
  if (!std::getenv("CI")) {
    cv::imshow(argv[0], viz);
    cv::waitKey(0);
  }
}
