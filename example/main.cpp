//
// Created by juanp on 24/07/18.
//

//#include "analyzers/textureAnalyzer.h"
//#include <MRILibrary.h>
#include <dirent.h>
#include <memory>
#include <MRILibrary/extractors/voiExtractor.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

std::vector<std::string> GetDirectoryFiles(const std::string &dir) {
    std::vector<std::string> files;
    std::shared_ptr<DIR> directory_ptr(opendir(dir.c_str()), [](DIR *dir) { dir && closedir(dir); });
    struct dirent *dirent_ptr;
    if (!directory_ptr) {
        std::cout << "Error opening : " << std::strerror(errno) << dir << std::endl;
        return files;
    }

    while ((dirent_ptr = readdir(directory_ptr.get())) != nullptr) {
        files.push_back(std::string(dirent_ptr->d_name));
    }
    return files;
}

using namespace cv;

int main()
{
    voiExtractor ve;
    std::vector<pair<string, string>> l = {{"testImages/B1P1/0.jpg", "testImages/B1P1/0_canny.png"},{"testImages/B1P1/1.jpg", "testImages/B1P1/1_canny.png"},{"testImages/B1P1/2.jpg", "testImages/B1P1/2_canny.png"}};
    ve.setListPaths(l);
    auto lr = ve.extractVOI();
    int i = 0;
    for (auto &img:lr) {
        imshow(to_string(i), img);
        i++;
    }
    cvWaitKey(0);

}
