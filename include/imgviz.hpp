#include <cmath>

#include <opencv2/opencv.hpp>

cv::Mat isfinite(cv::Mat mat) { return mat == mat; }

std::string typeToStr(int type) {
  std::string r;

  unsigned depth = type & CV_MAT_DEPTH_MASK;
  unsigned chans = 1 + (type >> CV_CN_SHIFT);

  switch (depth) {
  case CV_8U:
    r = "8U";
    break;
  case CV_8S:
    r = "8S";
    break;
  case CV_16U:
    r = "16U";
    break;
  case CV_16S:
    r = "16S";
    break;
  case CV_32S:
    r = "32S";
    break;
  case CV_32F:
    r = "32F";
    break;
  case CV_64F:
    r = "64F";
    break;
  default:
    r = "User";
    break;
  }

  r += "C";
  r += (chans + '0');

  return r;
}

void printMat(const std::string &name, const cv::Mat &mat) {
  double min;
  double max;
  cv::minMaxLoc(mat, &min, &max);
  fprintf(stderr,
          "[%s] height=%d, width=%d, channels=%d, type=%s, min=%lf, max=%lf\n",
          name.c_str(), mat.rows, mat.cols, mat.channels(),
          typeToStr(mat.type()).c_str(), min, max);
}

// ----------------------------------------------------------------------------

cv::Mat normalize(cv::Mat src, double min_val, double max_val) {
  assert(src.type() == CV_32FC1);
  return (src - min_val) / (max_val - min_val);
}

cv::Mat normalize(cv::Mat src) {
  assert(src.type() == CV_32FC1);

  double min_val;
  double max_val;
  cv::minMaxLoc(src, &min_val, &max_val);

  return normalize(src, min_val, max_val);
}

std::tuple<unsigned, unsigned> getTileShape(unsigned size,
                                            double hw_ratio = 1) {
  unsigned rows = static_cast<unsigned>(round(sqrt(size / hw_ratio)));
  unsigned cols = 0;
  while (rows * cols < size) {
    cols++;
  }
  while ((rows - 1) * cols >= size) {
    rows--;
  }
  return std::make_tuple(rows, cols);
}

cv::Mat tile(const std::vector<cv::Mat> images,
             std::tuple<unsigned, unsigned> shape = std::make_tuple(0, 0),
             const unsigned border_width = 5,
             const cv::Scalar border_color = cv::Scalar(255, 255, 255)) {
  unsigned height = images[0].rows;
  unsigned width = images[0].cols;

  if (std::get<0>(shape) * std::get<1>(shape) == 0) {
    shape = getTileShape(/*size=*/images.size(),
                         /*hw_ratio=*/static_cast<double>(height) /
                             static_cast<double>(width));
  }
  unsigned rows = std::get<0>(shape);
  unsigned cols = std::get<1>(shape);

  cv::Mat dst;
  for (size_t j = 0; j < rows; j++) {
    cv::Mat dst_row;
    for (size_t i = 0; i < cols; i++) {
      size_t index = j * cols + i;

      cv::Mat image;
      if (index < images.size()) {
        image = images[index];
        assert(image.rows == height);
        assert(image.cols == width);
      } else {
        image = cv::Mat::zeros(height, width, images[0].type());
      }

      if (image.type() == CV_8UC1) {
        cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
      }
      assert(image.type() == CV_8UC3);

      if (i == 0) {
        dst_row = image;
      } else {
        if (border_width != 0) {
          cv::Mat border = cv::Mat(dst_row.rows, border_width, CV_8UC3);
          border.setTo(border_color);
          cv::hconcat(dst_row, border, dst_row);
        }
        cv::hconcat(dst_row, image, dst_row);
      }
    }
    if (j == 0) {
      dst = dst_row;
    } else {
      if (border_width != 0) {
        cv::Mat border = cv::Mat(border_width, dst.cols, CV_8UC3);
        border.setTo(border_color);
        cv::vconcat(dst, border, dst);
      }
      cv::vconcat(dst, dst_row, dst);
    }
  }

  return dst;
}
