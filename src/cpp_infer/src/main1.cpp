// Copyright (c) 2020 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <include/args.h>
#include <include/paddleocr.h>
#include <include/paddlestructure.h>

using namespace PaddleOCR;

struct Student {
};
// 导出可以找到
// extern "C" {
std::vector<OCRPredictResult> recognize_w(cv::Mat mat_img);
std::vector<OCRPredictResult> ocr_recognize(std::string image_dir);// 只返回一张图片的结果 
// }

void check_params() {
  if (FLAGS_det) {
    if (FLAGS_det_model_dir.empty() || FLAGS_image_dir.empty()) {
      std::cout << "Usage[det]: ./ppocr "
                   "--det_model_dir=/PATH/TO/DET_INFERENCE_MODEL/ "
                << "--image_dir=/PATH/TO/INPUT/IMAGE/" << std::endl;
      exit(1);
    }
  }
  if (FLAGS_rec) {
    std::cout
        << "In PP-OCRv3, rec_image_shape parameter defaults to '3, 48, 320',"
           "if you are using recognition model with PP-OCRv2 or an older "
           "version, "
           "please set --rec_image_shape='3,32,320"
        << std::endl;
    if (FLAGS_rec_model_dir.empty() || FLAGS_image_dir.empty()) {
      std::cout << "Usage[rec]: ./ppocr "
                   "--rec_model_dir=/PATH/TO/REC_INFERENCE_MODEL/ "
                << "--image_dir=/PATH/TO/INPUT/IMAGE/" << std::endl;
      exit(1);
    }
  }
  if (FLAGS_cls && FLAGS_use_angle_cls) {
    if (FLAGS_cls_model_dir.empty() || FLAGS_image_dir.empty()) {
      std::cout << "Usage[cls]: ./ppocr "
                << "--cls_model_dir=/PATH/TO/REC_INFERENCE_MODEL/ "
                << "--image_dir=/PATH/TO/INPUT/IMAGE/" << std::endl;
      exit(1);
    }
  }
  if (FLAGS_table) {
    if (FLAGS_table_model_dir.empty() || FLAGS_det_model_dir.empty() ||
        FLAGS_rec_model_dir.empty() || FLAGS_image_dir.empty()) {
      std::cout << "Usage[table]: ./ppocr "
                << "--det_model_dir=/PATH/TO/DET_INFERENCE_MODEL/ "
                << "--rec_model_dir=/PATH/TO/REC_INFERENCE_MODEL/ "
                << "--table_model_dir=/PATH/TO/TABLE_INFERENCE_MODEL/ "
                << "--image_dir=/PATH/TO/INPUT/IMAGE/" << std::endl;
      exit(1);
    }
  }
  if (FLAGS_layout) {
    if (FLAGS_layout_model_dir.empty() || FLAGS_image_dir.empty()) {
      std::cout << "Usage[layout]: ./ppocr "
                << "--layout_model_dir=/PATH/TO/LAYOUT_INFERENCE_MODEL/ "
                << "--image_dir=/PATH/TO/INPUT/IMAGE/" << std::endl;
      exit(1);
    }
  }
  if (FLAGS_precision != "fp32" && FLAGS_precision != "fp16" &&
      FLAGS_precision != "int8") {
    std::cout << "precison should be 'fp32'(default), 'fp16' or 'int8'. "
              << std::endl;
    exit(1);
  }
}
// -------------------------------------------这里核心---------------------------------------------
std::vector<std::vector<OCRPredictResult>> ocr(std::vector<cv::String> &cv_all_img_names) {
  PPOCR ocr;

  if (FLAGS_benchmark) {
    ocr.reset_timer();
  }

  std::vector<cv::Mat> img_list;    //图片矩阵列表
  std::vector<cv::String> img_names;  //图片名字列表
  for (int i = 0; i < cv_all_img_names.size(); ++i) {
    cv::Mat img = cv::imread  
(cv_all_img_names[i], cv::IMREAD_COLOR);
    if (!img.data) {
      std::cerr << "[ERROR] image read failed! image path: "
                << cv_all_img_names[i] << std::endl;
      continue;
    }
    img_list.push_back(img);
    img_names.push_back(cv_all_img_names[i]);
  }
        // 这里开始推理
  std::vector<std::vector<OCRPredictResult>> ocr_results =
      ocr.ocr(img_list, FLAGS_det, FLAGS_rec, FLAGS_cls);

  for (int i = 0; i < img_names.size(); ++i) {    //我用的就一张
    std::cout << "predict img: " << cv_all_img_names[i] << std::endl;
    Utility::print_result(ocr_results[i]);
    if (FLAGS_visualize && FLAGS_det) {
      std::cout<<"if (FLAGS_visualize && FLAGS_det)******************************************"<<std::endl;
      std::string file_name = Utility::basename(img_names[i]);
      cv::Mat srcimg = img_list[i];
      Utility::VisualizeBboxes(srcimg, ocr_results[i],
                               FLAGS_output + "/" + file_name);
    }
  }
  if (FLAGS_benchmark) {
    ocr.benchmark_log(cv_all_img_names.size());
  }
  return ocr_results;
}

// main
std::vector<OCRPredictResult> ocr_recognize(std::string image_dir) { // 只返回一张图片的结果 
  // Parsing command-line
  // int argc2 = 8;
  // char* argv2[8] = {"./build/ppocr", "--det_model_dir=inference/det_db","--rec_model_dir=inference/rec_rcnn","--image_dir=zi4.png",  "--use_angle_cls=false","--det=true","--rec=true","--cls=false"};
  // char **argv21 = argv2;
  // google::ParseCommandLineFlags(&argc, &argv, true);
  // google::ParseCommandLineFlags(&argc2, &argv21, true);
  // 设置参数，检测+识别，不分类
  FLAGS_type = "ocr";
  // FLAGS_image_dir = "zi4.png";
  FLAGS_image_dir = image_dir;

  FLAGS_det_model_dir="inference/det_db";
  FLAGS_rec_model_dir="inference/rec_rcnn";
  FLAGS_det=1;
  FLAGS_rec=1;
  FLAGS_cls=0;
  std::vector<std::vector<OCRPredictResult>> ocr_results; // 保存检测出的结果
  check_params();
  if (!Utility::PathExists(FLAGS_image_dir)) {
    std::cerr << "[ERROR] image path not exist! image_dir: " << FLAGS_image_dir
              << std::endl;
    exit(1);
  }
  std::vector<cv::String> cv_all_img_names;
  cv::glob(FLAGS_image_dir, cv_all_img_names);
  std::cout << "total images num: " << cv_all_img_names.size() << std::endl;

  if (!Utility::PathExists(FLAGS_output)) {
    Utility::CreateDir(FLAGS_output);
  }
  if (FLAGS_type == "ocr") {
    std::cout <<"*****************ocr******************************"<<std::endl;
    ocr_results = ocr(cv_all_img_names);



  } else if (FLAGS_type == "structure") {
    // structure(cv_all_img_names);
    std::cout << "FLAGS_type == structure" << std::endl;
  } else {
    std::cout << "only value in ['ocr','structure'] is supported" << std::endl;
  }

  std::vector<OCRPredictResult> result0 = ocr_results[0];
  std::string str_re;
  std::cout<<"result0.size():"<<result0.size()<<std::endl;
  for (int i=0;i<result0.size();i++)
  {
    // std::cout<<result0[i]<<std::endl;
    std::vector<std::vector<int>> boxes = result0[i].box;   // bounding box [[0,1],[2,3]]
    if (boxes.size() > 0) {
      std::cout << "det boxes: [";
      for (int n = 0; n < boxes.size(); n++) {
        std::cout << '[' << boxes[n][0] << ',' << boxes[n][1] << "]";
        if (n != boxes.size() - 1) {
          std::cout << ',';
        }
      }
    }
    if (result0[i].score != -1.0) {
      str_re = result0[i].text;     // 文本结果
      std::cout << "rec text: " << str_re
                << " rec score: " << result0[i].score << " ";
    }
    std::cout << std::endl;
  }
  return result0;

  // std::cout<<FLAGS_det<<FLAGS_rec<<FLAGS_cls<<std::endl;
  // std::cout<<FLAGS_det_model_dir<<std::endl;
  // std::cout<<FLAGS_type<<std::endl;
  
}
// 检测+识别 入口 
// 入一张图片
// 返回结果列表3

std::vector<OCRPredictResult> recognize_w(cv::Mat mat_img)
{
  FLAGS_type = "ocr";
  // FLAGS_det_model_dir="inference/det_db";
  // FLAGS_rec_model_dir="inference/rec_rcnn";
  FLAGS_det_model_dir="/home/wfy/Documents/OCR/PaddleOCR/inference_lite/det_db";
  FLAGS_rec_model_dir="/home/wfy/Documents/OCR/PaddleOCR/inference_lite/rec_rcnn";
  // FLAGS_det_model_dir="/home/wfy/Documents/OCR/PaddleOCR/inference_full/det_db";
  // FLAGS_rec_model_dir="/home/wfy/Documents/OCR/PaddleOCR/inference_full/rec_rcnn";
  FLAGS_det=1;
  FLAGS_rec=1;
  FLAGS_cls=0;
  FLAGS_use_gpu=1;
  FLAGS_gpu_mem=8000;
  std::vector<OCRPredictResult> ocr_result; // 保存检测出的结果
  PPOCR ocr_w;    // 推理
  ocr_result = ocr_w.ocr(mat_img, FLAGS_det, FLAGS_rec, FLAGS_cls);   // 推理并得到结果
  return ocr_result;
}




// int main(int argc,char **argv)
// {
//   std::string path_img = "zi4.png";
//   // ocr_recognize(path_img);
//   cv::Mat mat_img = cv::imread(path_img);
  
//   std::vector<OCRPredictResult> ocr_result = recognize_w(mat_img);

//   std::vector<std::vector<int>> box;   // bounding box [[0,1],[2,3]]  // 这是一个框
//   std::string str_re;                                                 // 这是一个字符串
//   for (int i=0;i<ocr_result.size();i++)
//   {
//     // std::cout<<result0[i]<<std::endl;
//     box = ocr_result[i].box;   // bounding box [[0,1],[2,3]]
//     if (box.size() > 0) {
//       std::cout << "det boxes: [";
//       for (int n = 0; n < box.size(); n++) {
//         std::cout << '[' << box[n][0] << ',' << box[n][1] << "]";
//         if (n != box.size() - 1) {
//           std::cout << ',';
//         }
//       }
//     }
//     if (ocr_result[i].score != -1.0) {
//       str_re = ocr_result[i].text;     // 文本结果
//       std::cout << "rec text: " << str_re
//                 << " rec score: " << ocr_result[i].score << " ";
//     }
//     std::cout << std::endl;
//   }
// }

