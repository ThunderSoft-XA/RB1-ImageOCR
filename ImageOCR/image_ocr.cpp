/*The code in this repository is licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/

#include <iostream>
#include <vector>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
int main(int argc, char* argv[]) {
    // 图片读取
    cv::Mat image = cv::imread(argv[1]);
    cv::resize(image, image, cv::Size(), 0.5, 0.5);
    // 转化为灰度图
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    // 二值化
    cv::Mat binary;
    threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    // 去除噪声
    cv::Mat P1;
    cv::GaussianBlur(binary, P1, cv::Size(3, 3), 0);
    // 形态学处理
    //cv::Mat P2;
    //cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    //cv::morphologyEx(P1, P2, cv::MORPH_CLOSE, element);
    //imwrite("result.jpg", result);
    // 创建tesseract实例
    tesseract::TessBaseAPI tess;
    if(atoi(argv[2]) == 1){
        tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    }else{
        tess.Init(NULL, "chi_sim", tesseract::OEM_DEFAULT);
    }
    tess.SetPageSegMode(tesseract::PageSegMode::PSM_AUTO);
    // 识别文字
    tess.SetImage(P1.data, P1.cols, P1.rows, 1, P1.cols);
    char* out = tess.GetUTF8Text();

    // 打印输出
    std::cout << out << std::endl;

    // 保存文件
    std::ofstream file("output.txt");
    file << out;
    file.close();

    // 释放内存
    delete[] out;

    return 0;
}
