//
// Created by avatar01 on 7/09/18.
//

#include "analyzer.h"

analyzer::analyzer() {};

analyzer::analyzer(const string &path) : path(path) {
};

void analyzer::loadImage(string _path) {
    path = _path;
    inputImg = imread(_path, CV_LOAD_IMAGE_GRAYSCALE);
}

void analyzer::loadImage() {
    inputImg = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
}

analyzer::~analyzer() {};

