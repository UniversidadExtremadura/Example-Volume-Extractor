//
// Created by avatar01 on 7/09/18.
//

#include "textureAnalyzer.h"

textureAnalyzer::textureAnalyzer(const string &path) : analyzer(path) {}

textureAnalyzer::~textureAnalyzer() {

}

textureAnalyzer::textureAnalyzer() {}


vectorNGLDM textureAnalyzer::NGLDManalysis() {
    vectorNGLDM out;
    if (!inputImg.empty()) {
        algoritmoNGLDM(out);

    }
    return out;
}

vectorGLRLM textureAnalyzer::GLRLManalysis() {
    vectorGLRLM out;
    if (!inputImg.empty()) {
        algoritmoGLRLM(out);

    }
    return out;
}


vectorGLCM textureAnalyzer::GLCManalysis() {
    vectorGLCM out;
    if (!inputImg.empty()) {
        algoritmoGLCM(out);

    }
    return out;
}

void textureAnalyzer::algoritmoGLCM(vectorGLCM &salida) {
    double suma = 0, sumaux = 0, maxVal, l;
    int i, j, niveles, indicef = 0;
    double UX = 0, UY = 0, SX = 0, SY = 0;
    double auxiliar = 0, dato;
    vector<double> temporal;

    cv::minMaxLoc(inputImg, NULL, &maxVal, NULL, NULL, noArray());
    niveles = (int) maxVal;

    Mat *coocurrencia;
    Mat *coocNorm = new Mat(niveles + 1, niveles + 1, CV_64FC1);
    Mat *coocAuxil = new Mat(niveles + 1, niveles + 1, CV_64FC1);

    coocurrencia = obtenerMatrizCocurrencia();
    suma = cv::sum(*coocurrencia)[0];

    if (suma != 0) {
        for (i = 0; i <= niveles; i++) {
            for (j = 0; j <= niveles; j++) {
                l = coocurrencia->at<double>(i, j);
                l /= suma;
                coocNorm->at<double>(i, j) = l;
                coocAuxil->at<double>(i, j) = l * l;

                if (l != 0) {
                    temporal.push_back(l);
                }
            }
        }
    }

    salida[0] = cv::sum(*coocAuxil)[0];

    salida[1] = 0;
    for (const double &val:temporal) {
        salida[1] += val * log10(val);
    }

    salida[1] = -salida[1];


    for (i = 0; i <= niveles; i++) {
        suma = 0;
        for (j = 0; j <= niveles; j++) {
            l = coocNorm->at<double>(i, j);
            suma += l;

        }
        UX += i * suma;
    }

    for (i = 0; i <= niveles; i++) {
        suma = 0;
        for (j = 0; j <= niveles; j++) {
            l = coocNorm->at<double>(i, j);
            suma += l;

        }
        SX += (pow((i - UX), 2)) * suma;
    }

    for (j = 0; j <= niveles; j++) {
        suma = 0;
        for (i = 0; i <= niveles; i++) {
            l = coocNorm->at<double>(i, j);
            suma += l;

        }
        UY += j * suma;
    }

    for (j = 0; j <= niveles; j++) {
        suma = 0;
        for (i = 0; i <= niveles; i++) {
            l = coocNorm->at<double>(i, j);
            suma += l;

        }
        SY += (pow((j - UY), 2)) * suma;
    }

    int auxiliares = (niveles + 1) * (niveles + 1);

    Mat *I_J = new Mat(niveles + 1, niveles + 1, CV_64FC1);
    Mat *IJ_SUMA = new Mat(niveles + 1, niveles + 1, CV_64FC1);
    Mat *IJ_MULT = new Mat(niveles + 1, niveles + 1, CV_64FC1);
    Mat *INV_I_J = new Mat(niveles + 1, niveles + 1, CV_64FC1);
    Mat *IJ = new Mat(niveles + 1, niveles + 1, CV_64FC1);
    Mat *f2 = new Mat(1, auxiliares, CV_64FC1);
    Mat *f22 = new Mat(1, auxiliares, CV_64FC1);
    Mat *f3 = new Mat(1, auxiliares, CV_64FC1);
    Mat *f4 = new Mat(1, auxiliares, CV_64FC1);
    Mat *f5 = new Mat(1, auxiliares, CV_64FC1);
    Mat *f6 = new Mat(1, auxiliares, CV_64FC1);
    Mat *f7 = new Mat(1, auxiliares, CV_64FC1);
    Mat *f8 = new Mat(1, auxiliares, CV_64FC1, Scalar(0.));


    for (i = 0; i <= niveles; i++) {
        for (j = 0; j <= niveles; j++) {
            auxiliar = pow((i - j), 2);
            I_J->at<double>(i, j) = auxiliar;
            INV_I_J->at<double>(i, j) = (1 / (1 + auxiliar));
            IJ_SUMA->at<double>(i, j) = ((i - UX) + (j - UY));
            IJ_MULT->at<double>(i, j) = ((i - UX) * (j - UY));
            IJ->at<double>(i, j) = i * j;
            dato = coocNorm->at<double>(i, j);
            auxiliar = i * j;
            f2->at<double>(0, indicef) = dato * auxiliar;
            auxiliar = IJ_MULT->at<double>(i, j);
            f22->at<double>(0, indicef) = dato * auxiliar;
            auxiliar = INV_I_J->at<double>(i, j);
            f3->at<double>(0, indicef) = dato * auxiliar;
            auxiliar = I_J->at<double>(i, j);
            f4->at<double>(0, indicef) = dato * auxiliar;
            f7->at<double>(0, indicef) = pow(dato, 2) * auxiliar;
            auxiliar = IJ_SUMA->at<double>(i, j);
            f5->at<double>(0, indicef) = pow(dato, 3) * auxiliar;
            f6->at<double>(0, indicef) = pow(dato, 4) * auxiliar;
            f8->at<double>(0, indicef) = dato * abs((i + 1) - (j + 1));
            indicef++;

        }
    }

    suma = 0;
    sumaux = 0;
    salida[4] = 0;
    salida[5] = 0;
    salida[6] = 0;
    salida[7] = 0;
    salida[8] = 0;
    salida[9] = 0;

    for (i = 0; i < indicef; i++) {
        suma += f22->at<double>(0, i);
        sumaux += f2->at<double>(0, i);
        salida[4] += f3->at<double>(0, i);
        salida[5] += f4->at<double>(0, i);
        salida[6] += f5->at<double>(0, i);
        salida[7] += f6->at<double>(0, i);
        salida[8] += f7->at<double>(0, i);
        salida[9] += f8->at<double>(0, i);

    }

    if ((SX == 0) || (SY == 0)) {
        salida[2] = 0;
        salida[3] = 0;
    } else {
        salida[2] = suma / SX / SY;
        salida[3] = sumaux - (UX * UY) / (SX * SY);

    }

    delete (coocurrencia);
    delete (coocNorm);
    delete (coocAuxil);
    delete (I_J);
    delete (IJ_SUMA);
    delete (IJ_MULT);
    delete (INV_I_J);
    delete (IJ);
    delete (f2);
    delete (f22);
    delete (f3);
    delete (f4);
    delete (f5);
    delete (f6);
    delete (f7);
    delete (f8);
}

void textureAnalyzer::algoritmoNGLDM(vectorNGLDM &salida) {
    int ancho = inputImg.cols;
    int alto = inputImg.rows;
    int i, j, filas, iguales, distancia, columnas;
    double maxVal, suma, dato;
    uchar actualVal, neighVal;
    std::vector<kernelDis> kernel = {kernelDis(-1, -1), kernelDis(-1, 0), kernelDis(-1, 1), kernelDis(0, -1),
                                     kernelDis(0, 1), kernelDis(1, -1), kernelDis(1, 0), kernelDis(1, 1)};

    cv::minMaxLoc(inputImg, NULL, &maxVal, NULL, NULL, noArray());
    filas = (int) maxVal;
    distancia = 1; // DISTANCIA DEL KERNEL A TRATAR
    filas++;
    columnas = (int) pow((2 * distancia + 1), 2);


    Mat *NGLDS = new Mat(filas, columnas, CV_64FC1, Scalar(0.));

    for (i = 1; i < alto - 1; i++) {
        for (j = 1; j < ancho - 1; j++) {

            iguales = 0;
            actualVal = inputImg.at<uchar>(i, j);
            for (auto &desplazamiento:kernel) {
                neighVal = inputImg.at<uchar>(i + desplazamiento.i, j + desplazamiento.j);
                if (actualVal == neighVal)
                    iguales++;
            }

            if (iguales != 0) {
                suma = NGLDS->at<double>(actualVal, iguales);
                NGLDS->at<double>((int) actualVal, iguales) = suma + 1;
            }

        }

    }

    for (i = 0; i < 5; i++) {
        salida[i] = 0;
    }

    for (i = 0; i < filas; i++) {
        for (j = 0; j < columnas; j++) {
            dato = NGLDS->at<double>(i, j);

            if (j != 0) {

                salida[0] += (dato / pow(j, 2));

            }
            salida[1] += (dato * pow(j, 2));
            salida[3] += (pow(dato, 2));
            if (dato != 0) {
                salida[4] += dato * log10(dato);
            }

        }
    }

    for (j = 0; j < columnas; j++) {
        suma = 0;
        for (i = 0; i < filas; i++) {
            dato = NGLDS->at<double>(i, j);
            suma += dato;

        }
        salida[2] += pow(suma, 2);

    }

    salida[4] = -salida[4];
    delete (NGLDS);
}

void textureAnalyzer::algoritmoGLRLM(vectorGLRLM &salida) {

    int ancho = inputImg.cols;
    int alto = inputImg.rows;
    int i, j, k, runLength, nivelGris, maxRunLen, cont;
    double auxiliar, maxVal, dato, suma = 0, sumaux;
    uchar actualVal, neighVal;

    cv::minMaxLoc(inputImg, NULL, &maxVal, NULL, NULL, noArray());
    nivelGris = (int) maxVal;
    maxRunLen = ancho;

    // Crear e Inicializar matrices RL

    Mat *RL = new Mat(nivelGris + 1, maxRunLen + 1, CV_64FC1, Scalar(0.));
    Mat *RL0 = new Mat(nivelGris + 1, maxRunLen + 1, CV_64FC1, Scalar(0.));
    Mat *RL45 = new Mat(nivelGris + 1, maxRunLen + 1, CV_64FC1, Scalar(0.));
    Mat *RL90 = new Mat(nivelGris + 1, maxRunLen + 1, CV_64FC1, Scalar(0.));
    Mat *RL135 = new Mat(nivelGris + 1, maxRunLen + 1, CV_64FC1, Scalar(0.));


    //Calculo de la matriz RL0

    for (j = 0; j < ancho; j++) { // ancho de la matriz.

        actualVal = inputImg.at<uchar>(0, j);
        cont = 1;

        for (i = 1; i < alto; i++) { // alto de la matriz.

            neighVal = inputImg.at<uchar>(i, j);
            if (actualVal == neighVal) {

                cont++;

            } else {

                runLength = cont - 1;
                auxiliar = RL0->at<double>(actualVal, runLength);
                RL0->at<double>(actualVal, runLength) = auxiliar + 1;
                actualVal = neighVal;
                cont = 1;

            }

        }

        runLength = cont - 1;
        auxiliar = RL0->at<double>(actualVal, runLength);
        RL0->at<double>(actualVal, runLength) = auxiliar + 1;

    }

    //Calculo de la matriz RL90 JP

    for (i = 0; i < alto; i++) { // alto de la matriz.

        actualVal = inputImg.at<uchar>(i, 0);
        cont = 1;

        for (j = 1; j < ancho; j++) { // ancho de la matriz.

            neighVal = inputImg.at<uchar>(i, j);
            if (actualVal == neighVal) {

                cont++;

            } else {

                runLength = cont - 1;
                auxiliar = RL90->at<double>(actualVal, runLength);
                RL90->at<double>(actualVal, runLength) = auxiliar + 1;
                actualVal = neighVal;
                cont = 1;

            }

        }

        runLength = cont - 1;
        auxiliar = RL90->at<double>(actualVal, runLength);
        RL90->at<double>(actualVal, runLength) = auxiliar + 1;

    }

    //Calculo de la matriz RL45 JP

    for (i = 0; i < alto; i++) { // filas de la matriz

        //FIRST TRIANGLE
        actualVal = inputImg.at<uchar>(i, 0);
        cont = 1;
        j = 1;
        k = i - 1;

        while (k >= 0 && j < ancho) {
            neighVal = inputImg.at<uchar>(k, j);
            if (actualVal == neighVal) {
                cont++;

            } else {

                runLength = cont - 1;
                auxiliar = RL45->at<double>(actualVal, runLength);
                RL45->at<double>(actualVal, runLength) = auxiliar + 1;
                actualVal = neighVal;
                cont = 1;
            }
            j++;
            k--;
        }

        runLength = cont - 1;
        auxiliar = RL45->at<double>(actualVal, runLength);
        RL45->at<double>(actualVal, runLength) = auxiliar + 1;

        //SECOND TRIANGLE
        cont = 1;
        j = ancho - 2;
        k = alto - i;
        actualVal = inputImg.at<uchar>(alto - 1 - i, ancho - 1);
        if (k > 1) {
            while (k < alto && j >= 0) {
                neighVal = inputImg.at<uchar>(k, j);
                if (actualVal == neighVal) {
                    cont++;

                } else {

                    runLength = cont - 1;
                    auxiliar = RL45->at<double>(actualVal, runLength);
                    RL45->at<double>(actualVal, runLength) = auxiliar + 1;
                    actualVal = neighVal;
                    cont = 1;
                }
                j--;
                k++;
            }

            runLength = cont - 1;
            auxiliar = RL45->at<double>(actualVal, runLength);
            RL45->at<double>(actualVal, runLength) = auxiliar + 1;
        }
    }

    //Calculo de la matriz RL135 JP

    for (i = 0; i < alto; i++) { // filas de la matriz

        //FIRST TRIANGLE
        actualVal = inputImg.at<uchar>(i, 0);
        cont = 1;
        j = 1;
        k = i + 1;

        while (k < alto && j < ancho) {
            neighVal = inputImg.at<uchar>(k, j);
            if (actualVal == neighVal) {
                cont++;

            } else {

                runLength = cont - 1;
                auxiliar = RL135->at<double>(actualVal, runLength);
                RL135->at<double>(actualVal, runLength) = auxiliar + 1;
                actualVal = neighVal;
                cont = 1;
            }
            j++;
            k++;
        }

        runLength = cont - 1;
        auxiliar = RL135->at<double>(actualVal, runLength);
        RL135->at<double>(actualVal, runLength) = auxiliar + 1;

        //SECOND TRIANGLE
        cont = 1;
        j = ancho - 2;
        k = alto - 2 - i;
        actualVal = inputImg.at<uchar>(alto - 1 - i, ancho - 1);
        if (k < alto - 2) {
            while (k >= 0 && j >= 0) {
                neighVal = inputImg.at<uchar>(k, j);
                if (actualVal == neighVal) {
                    cont++;

                } else {

                    runLength = cont - 1;
                    auxiliar = RL135->at<double>(actualVal, runLength);
                    RL135->at<double>(actualVal, runLength) = auxiliar + 1;
                    actualVal = neighVal;
                    cont = 1;
                }
                j--;
                k--;

            }
            runLength = cont - 1;
            auxiliar = RL135->at<double>(actualVal, runLength);
            RL135->at<double>(actualVal, runLength) = auxiliar + 1;
        }
    }

    //Calculo de la matriz Promedio.
    *RL = (2 * *RL0 + 2 * *RL45 + 2 * *RL90 + 2 * *RL135) / 8;


    for (i = 0; i <= nivelGris; i++) {
        sumaux = 0;
        for (j = 0; j <= maxRunLen; j++) {

            dato = RL->at<double>(i, j);

            if (dato != 0) {

                salida[0] += dato * pow(j + 1, 2);
                salida[1] += dato / pow(j + 1, 2);
                suma += dato;
                sumaux += dato;

                if (i != 0) {

                    salida[5] += dato / pow(i, 2);
                    salida[7] += dato / (pow(i, 2) * pow(j + 1, 2));
                    salida[9] += (dato * pow(j + 1, 2)) / pow(i, 2);

                }

                salida[6] += dato * pow(i, 2);
                salida[8] += (dato * pow(i, 2)) / pow(j + 1, 2);
                salida[10] += dato * (pow(i, 2) * pow(j + 1, 2));

            }

        }

        salida[2] += pow(sumaux, 2);

    }

    salida[0] = salida[0] / suma;
    salida[1] = salida[1] / suma;
    salida[2] = salida[2] / suma;
    salida[5] = salida[5] / suma;
    salida[6] = salida[6] / suma;
    salida[7] = salida[7] / suma;
    salida[8] = salida[8] / suma;
    salida[9] = salida[9] / suma;
    salida[10] = salida[10] / suma;

    for (j = 0; j < maxRunLen; j++) {
        sumaux = 0;
        for (i = 0; i < nivelGris; i++) {
            dato = RL->at<double>(i, j);
            if (dato != 0) {
                sumaux += dato;
                salida[4] += dato;
            }
        }
        salida[3] += pow(sumaux, 2);
    }

    salida[3] = salida[3] / suma;
    salida[4] = salida[4] / nivelGris + 1;

    delete (RL);
    delete (RL0);
    delete (RL45);
    delete (RL90);
    delete (RL135);
}

/*Metodo: obtenerMatrizCocurrencia

Descripcion: Este metodo nos permite calcular la matriz de coocurrencia de niveles de grises
             de la imagen que le pasemos.

Devuelve: Mat*: Devuelve una matriz con los niveles de coocurrencia de grises.
*/

Mat *textureAnalyzer::obtenerMatrizCocurrencia() {

    int ancho = inputImg.cols;
    int alto = inputImg.rows;
    int i, j, niveles;
    double maxVal = 0, daux, dataTri1, dataTri2;
    uchar actualVal, neighVal;

    cv::minMaxLoc(inputImg, NULL, &maxVal, NULL, NULL, noArray());
    niveles = (int) maxVal;

    Mat *salida = new Mat(niveles + 1, niveles + 1, CV_64FC1, Scalar(0.));
    Mat *M_0 = new Mat(niveles + 1, niveles + 1, CV_64FC1, Scalar(0.));
    Mat *M_45 = new Mat(niveles + 1, niveles + 1, CV_64FC1, Scalar(0.));
    Mat *M_90 = new Mat(niveles + 1, niveles + 1, CV_64FC1, Scalar(0.));
    Mat *M_135 = new Mat(niveles + 1, niveles + 1, CV_64FC1, Scalar(0.));

    for (i = 0; i < alto; i++) {
        for (j = 0; j < ancho; j++) {

            actualVal = (uchar) inputImg.at<uchar>(i, j);
            if (i < alto - 1) {
                neighVal = (uchar) inputImg.at<uchar>(i + 1, j);
                daux = M_90->at<double>(actualVal, neighVal);
                M_90->at<double>(actualVal, neighVal) = daux + 1;
            }

            if ((i < alto - 1) && (j < ancho - 1)) {
                neighVal = (uchar) inputImg.at<uchar>(i + 1, j + 1);
                daux = M_45->at<double>(actualVal, neighVal);
                M_45->at<double>((int) actualVal, (int) neighVal) = daux + 1;
            }

            if (j < ancho - 1) {
                neighVal = (uchar) inputImg.at<uchar>(i, j + 1);
                daux = M_0->at<double>(actualVal, neighVal);
                M_0->at<double>(actualVal, neighVal) = daux + 1;
            }

            if ((i > 0) && (j < ancho - 1)) {
                neighVal = (uchar) inputImg.at<uchar>(i - 1, j + 1);
                daux = M_135->at<double>(actualVal, neighVal);
                M_135->at<double>(actualVal, neighVal) = daux + 1;
            }
        }
    }

    Mat *MR_0 = new Mat(niveles + 1, niveles + 1, CV_64FC1);
    Mat *MR_45 = new Mat(niveles + 1, niveles + 1, CV_64FC1);
    Mat *MR_90 = new Mat(niveles + 1, niveles + 1, CV_64FC1);
    Mat *MR_135 = new Mat(niveles + 1, niveles + 1, CV_64FC1);

    for (i = 0; i <= niveles; i++) { /* i<256;*/
        for (j = 0; j <= niveles; j++) {  /*j<256;*/

            dataTri1 = M_0->at<double>(i, j);
            dataTri2 = M_0->at<double>(j, i);
            MR_0->at<double>(i, j) = dataTri1 + dataTri2;

            dataTri1 = M_45->at<double>(i, j);
            dataTri2 = M_45->at<double>(j, i);
            MR_45->at<double>(i, j) = dataTri1 + dataTri2;

            dataTri1 = M_90->at<double>(i, j);
            dataTri2 = M_90->at<double>(j, i);
            MR_90->at<double>(i, j) = dataTri1 + dataTri2;

            dataTri1 = M_135->at<double>(i, j);
            dataTri2 = M_135->at<double>(j, i);
            MR_135->at<double>(i, j) = dataTri1 + dataTri2;

            daux = MR_0->at<double>(i, j);
            daux += MR_45->at<double>(i, j);
            daux += MR_90->at<double>(i, j);
            daux += MR_135->at<double>(i, j);

            salida->at<double>(i, j) = daux;

        }

    }

    delete (M_0);
    delete (M_45);
    delete (M_90);
    delete (M_135);
    delete (MR_0);
    delete (MR_45);
    delete (MR_90);
    delete (MR_135);

    return salida;
}
