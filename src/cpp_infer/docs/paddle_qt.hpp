#include <vector>

#include "opencv2/core.hpp"
struct OCRPredictResult {
  std::vector<std::vector<int>> box;
  std::string text;
  float score = -1.0;
  float cls_score;
  int cls_label = -1;
};

std::vector<OCRPredictResult> ocr_recognize(char* image_dir); // 只返回一张图片的结果 
std::vector<OCRPredictResult> recognize_w(cv::Mat mat_img);     // 入cv::mat 出 结果



