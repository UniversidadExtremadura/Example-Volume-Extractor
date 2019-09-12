//
// Created by avatar01 on 10/09/18.
//

#include "voiExtractor.h"

voiExtractor::voiExtractor() {}

voiExtractor::voiExtractor(const string &path) : extractor(path) {}

voiExtractor::~voiExtractor() {

}

void voiExtractor::setListPaths(const vector<pair<string, string>> &listPaths) {
    path = listPaths[0].first;
    voiExtractor::listPaths = listPaths;
}

void
voiExtractor::obtenerOctaedroMaximo(listaRectangulo R, Mat *matrizPuntosContorno, double &maxVolumen, int &maxProf,
                                    int &maxInd) {

    int i = 0;
    int prof = 1;
    double volumen = 0;
    bool enc = true;
    bool salir = false;

    std::vector<pair<string, string>> deepPaths(listPaths.begin() + 1, listPaths.end());
    while ((i < MAX_TAM_LIST - 1) && !salir) {
        for (const pair<string, string> &tmppath:deepPaths) {
            if (enc) {
                enc &= BuscarPlanoEquipolente(tmppath, matrizPuntosContorno, R[i]);
                if (enc) {
                    prof++;
                }
            }
        }
        salir = enc;
        volumen = prof * R[i].area;

        if (volumen > maxVolumen) {

            maxVolumen = volumen;
            maxProf = prof;
            maxInd = i;

        }
/*
        if (salir) {

            printf("Finalizando.....\n");

        } else {

            printf("Calculando Octaedros........\n");
            printf("Porcentaje completado %d de %d\n", i + 1, MAX_TAM_LIST - 1);

        }
*/
        prof = 1;
        i++;
    }

}

bool voiExtractor::BuscarPlanoEquipolente(pair<string, string> nombre, Mat *matrizPuntosContorno, rectangulo R) {

    Mat *imagenOriginal = new Mat();
    Mat *imagenContorno = new Mat();
    bool enc;

    *imagenOriginal = imread(nombre.first, CV_LOAD_IMAGE_GRAYSCALE);
    *imagenContorno = imread(nombre.second, CV_LOAD_IMAGE_GRAYSCALE);
    //obtenerContorno(imagenOriginal, imagenContorno);
    obtenerPuntosContorno(imagenContorno, matrizPuntosContorno);
    enc = rectanguloInteriorContorno(matrizPuntosContorno, R);

    delete (imagenOriginal);
    delete (imagenContorno);

    return enc;

}

// DEPRECATED
void voiExtractor::obtenerContorno(Mat *entrada, Mat *&salida) {

    int i, j; // Variables para usar como indices en los bucles.
    int alto, ancho; // Variables para las dimensiones de la imagen.
    Mat *imagenAux; // Imagenes auxiliar.
    //IplConvKernel *element = NULL; // Elemento utilizado para realizar el dilate y el erode de la imagen.
    Mat element2; //kernel para dilate/erode

    // Obtenemos las dimensiones de la imagen

    obtenerTamano(entrada, ancho, alto);

    // Aplicamos el filtro de canny

    cvCanny(entrada, salida, 10, 400, 3);

    // Ponemos las imagenes auxiliares enteras a negro.
    imagenAux = new Mat(alto, ancho, CV_8UC1, Scalar(0.));



    // Dilatamos el contorno obtenido del filtro de canny

    //element=cvCreateStructuringElementEx(2,2,1,1,CV_SHAPE_CROSS,0);
    element2 = getStructuringElement(MORPH_CROSS, Size(2, 2), Point(1, 1));
    //cvDilate(salida,salida,element,10);
    dilate(*salida, *salida, element2, Point(1, 1), 10);
    // Filtramos parte de los puntos conseguidos.
    // Quedandonos unicamente con los puntos del contorno.


    // Puntos a la izquierda

    uchar actVal;
    for (i = 0; i < alto; i++) {
        for (j = 0; j < ancho; j++) {
            actVal = salida->at<uchar>(i, j);
            //s=cvGet2D(salida,i,j);

            if (actVal == 255) {
                imagenAux->at<uchar>(i, j) = actVal;
                j = ancho - 1;

            } else {

                actVal = 0;
                imagenAux->at<uchar>(i, j) = actVal;

            }

        }

    }

    // Puntos a la derecha

    for (i = 0; i < alto; i++) {

        for (j = ancho - 1; j >= 0; j--) {
            actVal = salida->at<uchar>(i, j);

            if (actVal == 255) {
                imagenAux->at<uchar>(i, j) = actVal;
                j = 0;


            } else {
                actVal = 0;
                imagenAux->at<uchar>(i, j) = actVal;

            }

        }

    }

    // Puntos por arriba

    for (j = 0; j < ancho; j++) {

        for (i = 0; i < alto; i++) {
            actVal = salida->at<uchar>(i, j);

            if (actVal == 255) {
                imagenAux->at<uchar>(i, j) = actVal;
                i = alto - 1;


            } else {
                actVal = 0;
                imagenAux->at<uchar>(i, j) = actVal;

            }

        }

    }

    // Puntos por abajo

    for (j = 0; j < ancho; j++) {

        for (i = alto - 1; i >= 0; i--) {

            actVal = salida->at<uchar>(i, j);
            if (actVal == 255) {
                imagenAux->at<uchar>(i, j) = actVal;
                i = 0;

            } else {
                actVal = 0;
                imagenAux->at<uchar>(i, j) = actVal;

            }

        }

    }

    // Closing sobre la imagen auxiliar

    element2 = getStructuringElement(MORPH_CROSS, Size(2, 2), Point(1, 1));
    dilate(*imagenAux, *imagenAux, element2, Point(1, 1), 10);
    erode(*imagenAux, *imagenAux, element2, Point(1, 1), 4);

    // Volcamos la imagen auxiliar sobre la imagen salida.

    *salida = imagenAux->clone();


    // Liberamos memoria auxiliar

    delete (imagenAux);

}

bool voiExtractor::rectanguloInteriorContorno(Mat *matrizPuntosContorno, rectangulo R) {

    bool enc = true;
    int cortes = 0;
    int i;
    double auxiliar, actY, actX, preX, preY;

    for (i = 1; i < 360; i++) {
        preX = matrizPuntosContorno->at<double>(i - 1, 0);
        preY = matrizPuntosContorno->at<double>(i - 1, 1);
        actX = matrizPuntosContorno->at<double>(i, 0);
        actY = matrizPuntosContorno->at<double>(i, 1);

        if (R.puntoIy > min(preY, actY)) {

            if (R.puntoIy <= max(preY, actY)) {

                if (R.puntoIx <= max(preX, actX)) {

                    if (preY != actY) {

                        auxiliar = (R.puntoIy - preY) * (actX - preX) / (actY - preY) + preX;

                        if (preX == actX ||
                            R.puntoIx <= auxiliar) {

                            cortes++;

                        }

                    }

                }

            }

        }

    }

    // Si el numero de cortes es mayor que 0 y es par el punto es exterior y en caso contrario interior.

    if (cortes > 0) {

        if (cortes % 2 == 0) {

            enc = false;

        }

    }

    if (enc) {

        cortes = 0;

        for (i = 1; i < 360; i++) {

            preX = matrizPuntosContorno->at<double>(i - 1, 0);
            preY = matrizPuntosContorno->at<double>(i - 1, 1);
            actX = matrizPuntosContorno->at<double>(i, 0);
            actY = matrizPuntosContorno->at<double>(i, 1);


            if (R.puntoJy > min(preY, actY)) {

                if (R.puntoJy <= max(preY, actY)) {

                    if (R.puntoJx <= max(preX, actX)) {

                        if (preY != actY) {

                            auxiliar = (R.puntoJy - preY) * (actX - preX) / (actY - preY) + preX;

                            if (preX == actX || R.puntoJx <= auxiliar) {

                                cortes++;

                            }

                        }

                    }

                }

            }

        }

        // Si el numero de cortes es mayor que 0 y es par el punto es exterior y en caso contrario interior.

        if (cortes > 0) {

            if (cortes % 2 == 0) {

                enc = false;

            }

        }

        if (enc) {

            cortes = 0;

            for (i = 1; i < 360; i++) {
                preX = matrizPuntosContorno->at<double>(i - 1, 0);
                preY = matrizPuntosContorno->at<double>(i - 1, 1);
                actX = matrizPuntosContorno->at<double>(i, 0);
                actY = matrizPuntosContorno->at<double>(i, 1);

                if (R.puntoKy > min(preY, actY)) {

                    if (R.puntoKy <= max(preY, actY)) {

                        if (R.puntoKx <= max(preX, actX)) {

                            if (preY != actY) {

                                auxiliar = (R.puntoKy - preY) * (actX - preX) / (actY - preY) + preX;

                                if (preX == actX || R.puntoKx <= auxiliar) {

                                    cortes++;

                                }

                            }

                        }

                    }

                }

            }

            // Si el numero de cortes es mayor que 0 y es par el punto es exterior y en caso contrario interior.

            if (cortes > 0) {

                if (cortes % 2 == 0) {

                    enc = false;

                }

            }

            if (enc) {

                cortes = 0;

                for (i = 1; i < 360; i++) {

                    preX = matrizPuntosContorno->at<double>(i - 1, 0);
                    preY = matrizPuntosContorno->at<double>(i - 1, 1);
                    actX = matrizPuntosContorno->at<double>(i, 0);
                    actY = matrizPuntosContorno->at<double>(i, 1);

                    if (R.puntoSy > min(preY, actY)) {

                        if (R.puntoSy <= max(preY, actY)) {

                            if (R.puntoSx <= max(preX, actX)) {

                                if (preY != actY) {

                                    auxiliar = (R.puntoSy - preY) * (actX - preX) / (actY - preY) + preX;

                                    if (preX == actX || R.puntoSx <= auxiliar) {

                                        cortes++;

                                    }

                                }

                            }

                        }

                    }

                }

                // Si el numero de cortes es mayor que 0 y es par el punto es exterior y en caso contrario interior.

                if (cortes > 0) {

                    if (cortes % 2 == 0) {

                        enc = false;

                    }

                }

            }

        }

    }

    return enc;

}

std::vector<Mat> voiExtractor::extractVOI() {

    //Mat *imagenOriginal;
    Mat *imagenContorno = new Mat();
    Mat *matrizPuntosContorno = new Mat(360, 2, CV_64FC1, Scalar(0.));
    int ancho, alto, xMin, xMax, yMin, yMax;
    int distancia = 9;
    int anchoRet, altoRet, tamanoRet;
    int tamanoRetIoF = 0;
    int tamanoRetF = 0;
    int tamanoMatrizU = 0;

    //cargarImagen(listPaths[0].first,inputImg)
    inputImg = imread(listPaths[0].first, CV_LOAD_IMAGE_GRAYSCALE);
    cargarImagen(listPaths[0].second, imagenContorno);
    //extractor::obtenerContorno(imagenContorno);
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
        printf("Porcentaje completado %d de %d\n", i + 1, tamanoRetIoF - tamanoRetF - 2);

    }

    double maxVolumen = 0;
    int maxProf = 1;
    int maxInd = 0;

    obtenerOctaedroMaximo(R, matrizPuntosContorno, maxVolumen, maxProf, maxInd);

    printf("El volumen maximo encontrado es del rectangulo %d, profundidad %d y un volumen de %f", maxInd, maxProf,
           maxVolumen);

    printf("-----------------------\n");
    printf("------COORDENADAS------\n");
    printf("-----------------------\n");
    printf("PUNTO I. COORDENADA X: %d\n", R[maxInd].puntoIx);
    printf("PUNTO I. COORDENADA Y: %d\n", R[maxInd].puntoIy);
    printf("PUNTO J. COORDENADA X: %d\n", R[maxInd].puntoJx);
    printf("PUNTO J. COORDENADA Y: %d\n", R[maxInd].puntoJy);
    printf("PUNTO K. COORDENADA X: %d\n", R[maxInd].puntoKx);
    printf("PUNTO K. COORDENADA Y: %d\n", R[maxInd].puntoKy);
    printf("PUNTO S. COORDENADA X: %d\n", R[maxInd].puntoSx);
    printf("PUNTO S. COORDENADA Y: %d\n", R[maxInd].puntoSy);
    printf("-----------------------\n");
    printf("-----------------------\n");
    printf("-----------------------\n");


    xMin = 999;
    xMax = 0;
    yMin = 999;
    yMax = 0;

    if (R[maxInd].puntoIx < xMin)

        xMin = R[maxInd].puntoIx;

    if (R[maxInd].puntoIx > xMax)

        xMax = R[maxInd].puntoIx;

    if (R[maxInd].puntoJx < xMin)

        xMin = R[maxInd].puntoJx;

    if (R[maxInd].puntoJx > xMax)

        xMax = R[maxInd].puntoJx;

    if (R[maxInd].puntoKx < xMin)

        xMin = R[maxInd].puntoKx;

    if (R[maxInd].puntoKx > xMax)

        xMax = R[maxInd].puntoKx;

    if (R[maxInd].puntoSx < xMin)

        xMin = R[maxInd].puntoSx;

    if (R[maxInd].puntoSx > xMax)

        xMax = R[maxInd].puntoSx;

    if (R[maxInd].puntoIy < yMin)

        yMin = R[maxInd].puntoIy;

    if (R[maxInd].puntoIy > yMax)

        yMax = R[maxInd].puntoIy;

    if (R[maxInd].puntoJy < yMin)

        yMin = R[maxInd].puntoJy;

    if (R[maxInd].puntoJy > yMax)

        yMax = R[maxInd].puntoJy;

    if (R[maxInd].puntoKy < yMin)

        yMin = R[maxInd].puntoKy;

    if (R[maxInd].puntoKy > yMax)

        yMax = R[maxInd].puntoKy;

    if (R[maxInd].puntoSy < yMin)

        yMin = R[maxInd].puntoSy;

    if (R[maxInd].puntoSy > yMax)

        yMax = R[maxInd].puntoSy;

    alto = xMax - xMin;
    ancho = yMax - yMin;

    vector<Mat> results;

    for (int deep = 0; deep < maxProf; deep++) {
        Mat imagensal = Mat(alto, ancho, CV_8UC1);
        Mat orig = imread(listPaths[deep].first, CV_LOAD_IMAGE_GRAYSCALE);

        int i, j, k = 0, l = 0;
        for (i = yMin; i < yMax; i++) {
            for (j = xMin; j < xMax; j++) {
                imagensal.at<uchar>(l, k) = orig.at<uchar>(j, i);
                l++;
            }

            l = 0;
            k++;

        }
        results.push_back(imagensal);
    }

    delete (matrizU);
    delete (matrizPuntosReticulaIoF);
    delete (matrizPuntosReticulaF);
    delete (matrizPuntosReticula);
    delete (matrizPuntosContorno);
    delete (imagenContorno);
    return results;
}


