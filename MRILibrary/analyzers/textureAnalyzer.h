//
// Created by avatar01 on 7/09/18.
//

#ifndef MRILIBRARY_TEXTUREANALYZER_H
#define MRILIBRARY_TEXTUREANALYZER_H

#include "analyzer.h"
#include <iostream>
#include <math.h>
#include "../datatypes/texture.h"

using namespace std;
using namespace cv;

class textureAnalyzer : public analyzer {
private:
    void algoritmoGLCM(vectorGLCM &salida);

    void algoritmoNGLDM(vectorNGLDM &salida);

    void algoritmoGLRLM(vectorGLRLM &salida);

    Mat *obtenerMatrizCocurrencia();

public:
    textureAnalyzer(const string &path);

    textureAnalyzer();

    vectorNGLDM NGLDManalysis();

    vectorGLRLM GLRLManalysis();

    vectorGLCM GLCManalysis();

    virtual ~textureAnalyzer();
};


#endif //MRILIBRARY_TEXTUREANALYZER_H
