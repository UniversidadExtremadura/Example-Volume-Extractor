//
// Created by avatar01 on 10/09/18.
//

#ifndef MRILIBRARY_VOIEXTRACTOR_H
#define MRILIBRARY_VOIEXTRACTOR_H

#include "extractor.h"

class voiExtractor : protected extractor {
private:
    std::vector<pair<string, string>> listPaths;

    void
    obtenerOctaedroMaximo(listaRectangulo R, Mat *matrizPuntosContorno, double &maxVolumen, int &maxProf, int &maxInd);

    bool BuscarPlanoEquipolente(pair<string, string> nombre, Mat *matrizPuntosContorno, rectangulo R);

    void obtenerContorno(Mat *entrada, Mat *&salida);

    bool rectanguloInteriorContorno(Mat *matrizPuntosContorno, rectangulo R);

public:
    voiExtractor();

    void setListPaths(const vector<pair<string, string>> &listPaths);

    std::vector<Mat> extractVOI();

    voiExtractor(const string &path);

    virtual ~voiExtractor();
};


#endif //MRILIBRARY_VOIEXTRACTOR_H
