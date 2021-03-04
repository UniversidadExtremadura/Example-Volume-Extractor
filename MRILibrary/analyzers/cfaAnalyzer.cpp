//
// Created by avatar01 on 7/09/18.
//

#include "cfaAnalyzer.h"

#define MAX_TAM_IMG 20

cfaAnalyzer::cfaAnalyzer() {}

cfaAnalyzer::cfaAnalyzer(const string &path) : analyzer(path) {}

cfaAnalyzer::~cfaAnalyzer() {

}

vectorCFA cfaAnalyzer::cfaAnalysis() {
    vectorCFA result;
    std::vector<double> fractals(MAX_TAM_IMG, 0);
    double fractalDimensions = 0;
    if (!inputImg.empty()) {
        obtenerFractales(fractals, fractalDimensions);

    }
    result.setMyv(fractals);
    result.setSize(fractalDimensions);
    return result;
}

vectorFCV cfaAnalyzer::ftaAnalysis() {
    vectorFCV result;
    vectorCFA fractal;
    std::vector<double> fractals(MAX_TAM_IMG, 0);
    double fractalDimensions = 0;
    if (!inputImg.empty()) {
        obtenerFractales(fractals, fractalDimensions);
        fractal.setMyv(fractals);
        fractal.setSize(fractalDimensions);
        statistics(fractal, result);
    }

    return result;
}

void cfaAnalyzer::statistics(vectorCFA vectorCFA1, vectorFCV &vector) {

    int i;
    double aux, res, media, varianza;
    std::vector<double> N = vectorCFA1.getMyv();
    double p = vectorCFA1.getSize();

    res = 0;

    for (i = 0; i < p; i++) {
        res += N[i];

    }

    media = res / p;
    res = 0;

    for (i = 0; i < p; i++) {

        aux = N[i];
        aux = pow(aux - media, 2);
        res += aux;

    }

    varianza = res / p;

    for (i = 0; i < p; i++) {

        aux = N[i];

        if (aux != 0) {

            vector[0] += pow(aux, 2);
            vector[1] += aux * log10(aux);
            vector[2] += (i - media) * aux;
            vector[3] += aux / (1 + pow(i, 2));
            vector[4] += aux * pow(i, 2);
            vector[5] += pow(aux, 2) * pow(i, 2);
            vector[7] += pow((i - media), 2) * aux;
            vector[8] += pow((i - media), 3) * aux;
            vector[9] += pow((i - media), 4) * aux;

            if (i != 0) {

                vector[6] += aux / pow(i, 2);

            }

        }

    }

    if (varianza != 0) {

        vector[2] = vector[2] / varianza;

    } else {

        vector[2] = 0;

    }

}

void cfaAnalyzer::obtenerFractales(std::vector<double> &N, double &p) {

    int alto = inputImg.rows;
    int ancho = inputImg.cols;
    int tamano;
    int i, j, g;
    double res, aux, auxil, maximo;
    uchar actVal, neighVal;

    maximo = std::max(alto, ancho);

    p = log10(maximo) / log10(2);
    p = ceil(p);
    tamano = pow(2, p);

    Mat *imagenAmpliada = new Mat(tamano, tamano, CV_8UC1, Scalar(0));


    for (i = 0; i < alto && i < tamano; i++) {
        for (j = 0; j < ancho && j < tamano; j++) {
            imagenAmpliada->at<uchar>(i, j) = inputImg.at<uchar>(i, j);
        }

    }


    for (g = 1; g < p; g++) {
        res = 0;
        aux = pow(2, p - g);


        for (i = 0; i < pow(2, g); i++) {
            for (j = 0; j < pow(2, g); j++) {
                if (i != 0 || j != 0) {
                    for (int k = 0; k < aux; k++) {
                        for (int l = 0; l < aux; l++) {
                            actVal = imagenAmpliada->at<uchar>(k, l);
                            neighVal = imagenAmpliada->at<uchar>(k + i * aux, l + j * aux);

                            if (actVal == neighVal) {

                                res++;

                            }
                        }
                    }
                }
            }
        }
        N[g] = res;
    }

    /*
    for (g = p - 1; g > 0; g--) {

        res = 0;
        aux = pow(2, p - g);
        auxil = ceil(aux / 2);

        for (i = 1; i < aux; i = i+1 ) {
            for (j = 1; j < aux; j = j + 1) {

                actVal = imagenAmpliada->at<uchar>(i, j);
                neighVal = imagenAmpliada->at<uchar>(i + auxil, j);

                if (actVal == neighVal) {

                    res++;

                } else {
                    neighVal = imagenAmpliada->at<uchar>(i, j + auxil);

                    if (actVal == neighVal) {

                        res++;

                    } else {
                        neighVal = imagenAmpliada->at<uchar>(i + auxil, j + auxil);

                        if (actVal == neighVal) {

                            res++;

                        }

                    }

                }

            }

        }

        N[g] = res;
    }
*/

    delete (imagenAmpliada);
}
