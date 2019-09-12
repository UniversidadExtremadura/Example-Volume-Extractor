//
// Created by avatar01 on 10/09/18.
//

#ifndef MRILIBRARY_ROIEXTRACTOR_H
#define MRILIBRARY_ROIEXTRACTOR_H

#include "extractor.h"

class roiExtractor : public extractor {
private:

public:
    roiExtractor(const string &path);

    roiExtractor();

    Mat extractROI(std::string path_contorno);

    virtual ~roiExtractor();
};


#endif //MRILIBRARY_ROIEXTRACTOR_H
