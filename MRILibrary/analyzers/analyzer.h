//
// Created by avatar01 on 7/09/18.
//

#ifndef MRILIBRARY_ANALYZER_H
#define MRILIBRARY_ANALYZER_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <string>

using namespace cv;
using namespace std;

class analyzer {
protected:
    string path;
    Mat inputImg;

public:
    analyzer(const string &path);

    analyzer();

    void loadImage();

    void loadImage(string _path);

    virtual ~analyzer() = 0;

};


#endif //MRILIBRARY_ANALYZER_H
