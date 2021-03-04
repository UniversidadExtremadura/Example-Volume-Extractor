//
// Created by avatar01 on 7/09/18.
//

#include "opftaAnalyzer.h"


opftaAnalyzer::opftaAnalyzer() {}

opftaAnalyzer::opftaAnalyzer(const string &path) : analyzer(path) {}

opftaAnalyzer::~opftaAnalyzer() {

}

vectorOFCV opftaAnalyzer::opftaAnalysis() {
    vectorOFCV result;
    if (!inputImg.empty() && inputImg.cols > 16 && inputImg.rows > 16) {
        int alto = inputImg.rows;
        int ancho = inputImg.cols;
        int columnas = ancho / 16;
        int filas = alto / 16;
        if (inputImg.cols > 32 && inputImg.rows > 32) {
            columnas--;
            filas--;
        }
        Mat *frac = new Mat(filas, columnas, CV_64FC1, Scalar(0.));

        FTA(frac, columnas, filas);

        statistics(frac, filas, columnas, result);

    }
    return result;
}

void opftaAnalyzer::FTA(Mat *&N, int columnas, int filas) {

    int i, j;
    double FD;

    for (i = 0; i <= filas - 1; i++) {
        for (j = 0; j <= columnas - 1; j++) {

            FD = calcular_FD(i, j) / 16;
            N->at<double>(i, j) = FD;

        }

    }

}

void opftaAnalyzer::statistics(Mat *frac, int filas, int columnas, vectorOFCV &vector) {
    double aux, suma, UX = 0, UY = 0, SX = 0, SY = 0;
    int i, j;

    vector[0] = 0;

    for (i = 0; i < filas; i++) {
        for (j = 0; j < columnas; j++) {
            vector[0] += pow(frac->at<double>(i, j), 2);
        }

    }

    vector[1] = 0;

    for (i = 0; i < filas; i++) {
        for (j = 0; j < columnas; j++) {
            vector[1] += log10(frac->at<double>(i, j)) * frac->at<double>(i, j);

        }

    }

    vector[1] = -vector[1];

    vector[3] = 0;

    for (i = 0; i < filas; i++) {
        for (j = 0; j < columnas; j++) {
            vector[3] += frac->at<double>(i, j) / (1 + pow(i - j, 2));

        }

    }

    vector[4] = 0;

    for (i = 0; i < filas; i++) {
        for (j = 0; j < columnas; j++) {
            vector[4] += frac->at<double>(i, j) * pow(i - j, 2);

        }

    }

    vector[5] = 0;

    for (i = 0; i < filas; i++) {
        for (j = 0; j < columnas; j++) {
            vector[5] += pow(frac->at<double>(i, j), 2) * pow(i - j, 2);

        }

    }

    for (i = 0; i < filas; i++) {
        suma = 0;
        for (j = 0; j < columnas; j++) {
            suma += frac->at<double>(i, j);

        }

        UX += suma / filas;
    }

    for (i = 0; i < filas; i++) {
        suma = 0;
        for (j = 0; j < columnas; j++) {
            suma += frac->at<double>(i, j);

        }

        SX += (pow((suma - UX), 2)) / filas;

    }

    for (j = 0; j < columnas; j++) {
        suma = 0;
        for (i = 0; i < filas; i++) {
            suma += frac->at<double>(i, j);

        }

        UY += suma / columnas;

    }

    for (j = 0; j < columnas; j++) {
        suma = 0;
        for (i = 0; i < filas; i++) {
            suma += frac->at<double>(i, j);

        }

        SY += (pow((suma - UY), 2)) / columnas;

    }

    vector[2] = 0;
    vector[6] = 0;

    if ((SX == 0) || (SY == 0)) {

        vector[2] = 0;
        vector[6] = 0;

    } else {

        vector[6] = (SX / UX) + (SY / UY);

        for (i = 0; i < filas; i++) {
            for (j = 0; j < columnas; j++) {

                aux = (UX * UY) * frac->at<double>(i, j);;
                vector[2] += aux;

            }

        }

        vector[2] = vector[2] / SX / SY;

    }
}

double opftaAnalyzer::calcular_FD(int i, int j) {

    int k, l;
    int kini = i * 16;
    int kfin = (i + 1) * 16;
    int lini = j * 16;
    int lfin = (j + 1) * 16;
    double resultado = 0;

    for (k = kini; k < kfin; k++) {

        for (l = lini; l < lfin; l++) {

            resultado = resultado + calcular_casilla(k, l, kini, kfin, lini, lfin);

        }

    }

    return resultado;

}

double opftaAnalyzer::calcular_casilla(int k, int l, int kini, int kfin, int lini, int lfin) {

    double resultado = 0;
    int n, m;
    uchar actVal, nVal;

    actVal = inputImg.at<uchar>(k, l);

    for (n = kini; n < kfin; n++) {
        for (m = lini; m < lfin; m++) {
            if ((n != k) && (m != l)) {

                nVal = inputImg.at<uchar>(n, m);

                if (nVal == actVal) {

                    resultado++;

                }

            }
        }
    }

    resultado = resultado / 256;

    return resultado;

}
