//
// Created by avatar01 on 10/09/18.
//

#include "roiExtractor.h"

roiExtractor::roiExtractor(const string &path) : extractor(path) {}

roiExtractor::~roiExtractor() {

}

roiExtractor::roiExtractor() {}

Mat roiExtractor::extractROI(std::string path_contorno) {
    Mat *imagenContorno = new Mat();
    Mat *matrizPuntosContorno = new Mat(360, 2, CV_64FC1, Scalar(0.));
    int ancho, alto, xMin, xMax, yMin, yMax;
    int distancia = 7;
    int anchoRet, altoRet, tamanoRet;
    int tamanoRetIoF = 0;
    int tamanoRetF = 0;
    int tamanoMatrizU = 0;

    cargarImagen(path_contorno, imagenContorno);

    obtenerPuntosContorno(imagenContorno, matrizPuntosContorno);
    obtenerPuntosRectanguloExterior(matrizPuntosContorno, xMin, xMax, yMin, yMax);

    altoRet = xMax - xMin;
    anchoRet = yMax - yMin;

    if (anchoRet % distancia != 0) {
        anchoRet = (anchoRet / distancia) + 2;
    } else {
        anchoRet = (anchoRet / distancia) + 1;
    }

    if (altoRet % distancia != 0) {
        altoRet = (altoRet / distancia) + 2;
    } else {
        altoRet = (altoRet / distancia) + 1;
    }

    tamanoRet = (altoRet * anchoRet);
    Mat *matrizPuntosReticula = new Mat(tamanoRet, 5, CV_64FC1);

    crearReticula(matrizPuntosReticula, distancia, xMin, xMax, yMin, yMax, tamanoRet);

    asignarIoEoF(matrizPuntosContorno, matrizPuntosReticula, tamanoRet);
    corregirErroresReticula(matrizPuntosReticula, tamanoRet, anchoRet);
    actualizarPuntosFrontera(matrizPuntosReticula, tamanoRet, anchoRet);

    for (int i = 0; i < tamanoRet; i++) {
        if (matrizPuntosReticula->at<double>(i, 4) != 0) {
            tamanoRetIoF++;
        }

        if (matrizPuntosReticula->at<double>(i, 4) == 2) {
            tamanoRetF++;
        }
    }

    Mat *matrizPuntosReticulaIoF = new Mat(tamanoRetIoF, 5, CV_64FC1);
    Mat *matrizPuntosReticulaF = new Mat(tamanoRetF, 5, CV_64FC1);

    volcarPuntosIoF(matrizPuntosReticula, matrizPuntosReticulaIoF, tamanoRet);
    volcarPuntosFrontera(matrizPuntosReticulaIoF, matrizPuntosReticulaF, tamanoRetIoF);

    tamanoMatrizU = tamanoRetIoF - tamanoRetF - 2;

    Mat *matrizU = new Mat(tamanoMatrizU, 5, CV_64FC1);
    listaRectangulo R;
    for (int m = 0; m < MAX_TAM_LIST; m++) {
        R[m].area = 0;
    }
    double area = 0;
    int ocupacion = 0;

    for (int i = 0; i < tamanoRetIoF - tamanoRetF - 2; i++) {
        area = 0;
        if (matrizPuntosReticulaIoF->at<double>(i, 4) != 2) {

            obtenerMatrizU(matrizPuntosReticulaIoF, matrizPuntosReticulaF, matrizU, tamanoMatrizU, i, tamanoRetIoF,
                           tamanoRetF);
            obtenerRectanguloMaximo(matrizPuntosReticulaIoF, matrizPuntosReticulaF, matrizU, i, tamanoRetIoF,
                                    tamanoRetF, tamanoMatrizU, ocupacion, area, R);

        }

        printf("Calculando Rectangulos........\n");
        printf("Porcentaje completado %d de %d\n", i + 1, tamanoRetIoF - tamanoRetF - 2); //TODO PRINT

    }

    xMin = 999;
    xMax = 0;
    yMin = 999;
    yMax = 0;

    if (R[0].puntoIx < xMin)

        xMin = R[0].puntoIx;

    if (R[0].puntoIx > xMax)

        xMax = R[0].puntoIx;

    if (R[0].puntoJx < xMin)

        xMin = R[0].puntoJx;

    if (R[0].puntoJx > xMax)

        xMax = R[0].puntoJx;

    if (R[0].puntoKx < xMin)

        xMin = R[0].puntoKx;

    if (R[0].puntoKx > xMax)

        xMax = R[0].puntoKx;

    if (R[0].puntoSx < xMin)

        xMin = R[0].puntoSx;

    if (R[0].puntoSx > xMax)

        xMax = R[0].puntoSx;

    if (R[0].puntoIy < yMin)

        yMin = R[0].puntoIy;

    if (R[0].puntoIy > yMax)

        yMax = R[0].puntoIy;

    if (R[0].puntoJy < yMin)

        yMin = R[0].puntoJy;

    if (R[0].puntoJy > yMax)

        yMax = R[0].puntoJy;

    if (R[0].puntoKy < yMin)

        yMin = R[0].puntoKy;

    if (R[0].puntoKy > yMax)

        yMax = R[0].puntoKy;

    if (R[0].puntoSy < yMin)

        yMin = R[0].puntoSy;

    if (R[0].puntoSy > yMax)

        yMax = R[0].puntoSy;

    alto = xMax - xMin;
    ancho = yMax - yMin;
    Mat imagensal = Mat(alto, ancho, CV_8UC1);

    int i, j, k = 0, l = 0;

    for (i = yMin; i < yMax; i++) {
        for (j = xMin; j < xMax; j++) {
            imagensal.at<uchar>(l, k) = inputImg.at<uchar>(j, i);
            l++;
        }

        l = 0;
        k++;

    }

    delete (matrizPuntosContorno);
    delete (matrizPuntosReticula);
    delete (matrizPuntosReticulaIoF);
    delete (matrizPuntosReticulaF);
    delete (imagenContorno);
    delete (matrizU);
    return imagensal;
}
