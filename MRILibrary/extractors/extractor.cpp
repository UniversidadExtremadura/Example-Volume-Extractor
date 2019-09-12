//
// Created by avatar01 on 10/09/18.
//

#include "extractor.h"

extractor::extractor() {}

extractor::extractor(const string &path) : path(path) {}

extractor::~extractor() {

}

void extractor::loadImage() {
    inputImg = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
}

void extractor::loadImage(string _path) {
    path = _path;
    inputImg = imread(_path, CV_LOAD_IMAGE_GRAYSCALE);
}

void extractor::obtenerContorno(Mat *&salida) {

    int i, j; // Variables para usar como indices en los bucles.
    int alto, ancho; // Variables para las dimensiones de la imagen.
    Mat *imagenAux; // Imagenes auxiliar.
//    IplConvKernel *element = NULL; // Elemento utilizado para realizar el dilate y el erode de la imagen.
    Mat element; //kernel para dilate/erode

    // Obtenemos las dimensiones de la imagen

    obtenerTamano(&inputImg, ancho, alto);

    // Aplicamos el filtro de canny

    Canny(inputImg, *salida, 10, 400, 3);

    // Ponemos las imagenes auxiliares enteras a negro.
    imagenAux = new Mat(alto, ancho, CV_8UC1, Scalar(0.));



    // Dilatamos el contorno obtenido del filtro de canny

    //element=cvCreateStructuringElementEx(2,2,1,1,CV_SHAPE_CROSS,0);
    element = getStructuringElement(MORPH_CROSS, Size(2, 2), Point(1, 1));
    //cvDilate(salida,salida,element,10);
    dilate(*salida, *salida, element, Point(1, 1), 10);
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

    element = getStructuringElement(MORPH_CROSS, Size(2, 2), Point(1, 1));
    dilate(*imagenAux, *imagenAux, element, Point(1, 1), 10);
    erode(*imagenAux, *imagenAux, element, Point(1, 1), 4);

    // Volcamos la imagen auxiliar sobre la imagen salida.

    *salida = imagenAux->clone();


    // Liberamos memoria auxiliar

    delete (imagenAux);

}

void extractor::obtenerPuntosContorno(Mat *imagen, Mat *&matrizPuntosContorno) {

    int i, j; // Variables para usar como indices en los bucles.
    int ancho, alto;
    int bandera; // Variable para usar como bandera de control.
    int auxilX, auxilY; // Variables auxiliares para calcular los 360 puntos del contorno.
    int PUNTA2ideX, PUNTA2ideY; // Variables para almacenar las coordenadas del PUNTA2ide.
    uchar val; // Variable para almacenar los valores de un pixel de la imagen.

    bandera = 0;
    j = 0;

    obtenerTamano(imagen, ancho, alto);

    // Calculammos el PUNTA2ide.

    obtenerPUNTA2ide(imagen, PUNTA2ideX, PUNTA2ideY);

    // Calculamos los puntos del contorno girando grado a grado a partir del PUNTA2ide.

    for (i = 0; i < 360; i++) {

        while (bandera == 0) {

            auxilX = (int) (PUNTA2ideX + (j * cos(i * CV_PI / 180)));
            auxilY = (int) (PUNTA2ideY + (j * sin(i * CV_PI / 180)));

            if ((auxilX < alto) && (auxilY < ancho) && auxilX >= 0 && auxilY >= 0) {

                val = imagen->at<uchar>(auxilX, auxilY);

                if (val == 255) {
                    bandera = 1;
                    matrizPuntosContorno->at<double>(i, 0) = auxilX;
                    matrizPuntosContorno->at<double>(i, 1) = auxilY;


                }

            } else {

                bandera = 1;

            }

            j++;

        }

        j = 0;
        bandera = 0;

    }

}

void extractor::obtenerTamano(Mat *imagen, int &ancho, int &alto) {
    ancho = imagen->cols;
    alto = imagen->rows;

}

void extractor::obtenerPUNTA2ide(Mat *imagen, int &PUNTA2ideX, int &PUNTA2ideY) {

    int ancho, alto; // Variables para las dimensiones de la imagen.
    int i, j; // Variables para usar como indices de los bucles.
    int cuentaX = 0; // Cuenta el numero de coordenadas X de los puntos del contorno.
    int cuentaY = 0; // Cuenta el numero de coordenadas Y de los puntos del contorno.
    int sumaX = 0; // Suma de todos las coordenadas X de los puntos del contorno.
    int sumaY = 0; // Suma de todas las coordenadas Y de los puntos del contorno.
    uchar val; // Variable para almacenar los valores de un pixel de la imagen.

    // Obtenemos las dimensiones de la imagen

    obtenerTamano(imagen, ancho, alto);

    // Calculamos el PUNTA2ide

    for (i = 0; i < alto; i++) {
        for (j = 0; j < ancho; j++) {
            val = imagen->at<uchar>(i, j);

            if (val == 255) {

                cuentaY += j;
                cuentaX += i;
                sumaX++;
                sumaY++;

            }

        }

    }

    PUNTA2ideX = cuentaX / sumaX;
    PUNTA2ideY = cuentaY / sumaY;

}

void extractor::cargarImagen(string nombre, Mat *&imagen) {

    *imagen = imread(nombre, CV_LOAD_IMAGE_GRAYSCALE); //Cargamos la imagen en la escala de grises.

}

void extractor::obtenerPuntosRectanguloExterior(Mat *matrizPuntosContorno, int &xMin, int &xMax, int &yMin, int &yMax) {

    int i; // Variable para usar como indice en el bucle.
    double valX, valY;

    xMin = 9999;
    xMax = 0;
    yMin = 9999;
    yMax = 0;


    for (i = 0; i < 360; i++) {

        valX = matrizPuntosContorno->at<double>(i, 0);
        valY = matrizPuntosContorno->at<double>(i, 1);
        if ((valX != 0) && (valY != 0)) {
            if (valX > xMax) {
                xMax = static_cast<int>(valX);

            }

            if (valX < xMin) {
                xMin = static_cast<int>(valX);

            }

            if (valY > yMax) {
                yMax = static_cast<int>(valY);

            }

            if (valY < yMin) {
                yMin = static_cast<int>(valY);

            }

        }

    }

}

void extractor::crearReticula(Mat *&matrizPuntosReticula, int distancia, int xMin, int xMax, int yMin,
                              int yMax, int tamanoRet) {

    int i, j; // Variables para usar como indices en los bucles.
    int x, y; // Variables para usar como indices en la reticula.
    int num;

    x = 0;
    y = 0;
    i = xMin;
    j = yMin;

    for (num = 0; num < tamanoRet; num++) {

        matrizPuntosReticula->at<double>(num, 0) = i;
        matrizPuntosReticula->at<double>(num, 1) = j;
        matrizPuntosReticula->at<double>(num, 2) = x;
        matrizPuntosReticula->at<double>(num, 3) = y;
        matrizPuntosReticula->at<double>(num, 4) = 0;

        if (j < (yMax)) {

            j += distancia;
            y++;

        } else {

            j = yMin;
            y = 0;
            x++;
            i += distancia;

        }

    }

}

void extractor::asignarIoEoF(Mat *matrizPuntosContorno, Mat *&matrizPuntosReticula, int tamanoReticula) {

    int i, k;  // Variables para recorrer los bucles.
    int cortes = 0; // Variable que contabiliza el numero de cortes.
    double auxiliar; // Variable auxiliar que almacena valores.
    bool enc; // Variable que controla si un punto de la reticula es o no vertice del contorno.
    double actJ, actI, kJ, kI, prevkI, prevkJ;
    for (i = 0; i < tamanoReticula; i++) {

        // Extraemos un punto de la reticula y comprobamos si esta en el contorno.

        enc = esVertice(matrizPuntosContorno, matrizPuntosReticula, i);

        if (!enc) {

            // Si el punto de la reticula no esta en el contorno mediante las rectas de Benhansen comprobamos
            // si dicho punto es interior o exterior al contorno detectado.

            cortes = 0;

            for (k = 1; k < 360; k++) {

                actJ = matrizPuntosReticula->at<double>(i, 1);
                actI = matrizPuntosReticula->at<double>(i, 0);
                kI = matrizPuntosContorno->at<double>(k, 0);
                kJ = matrizPuntosContorno->at<double>(k, 1);
                prevkI = matrizPuntosContorno->at<double>(k - 1, 0);
                prevkJ = matrizPuntosContorno->at<double>(k - 1, 1);

                if (actJ > min(prevkJ, kJ)) {
                    //cvmGet(matrizPuntosReticula,i,1) > min(cvmGet(matrizPuntosContorno,k-1,1),cvmGet(matrizPuntosContorno,k,1))
                    if (actJ <= max(prevkJ, kJ)) {
                        //cvmGet(matrizPuntosReticula,i,1) <= max(cvmGet(matrizPuntosContorno,k-1,1),cvmGet(matrizPuntosContorno,k,1))
                        if (actI <= max(prevkI, kI)) {
                            //cvmGet(matrizPuntosReticula,i,0) <= max(cvmGet(matrizPuntosContorno,k-1,0),cvmGet(matrizPuntosContorno,k,0))
                            if (prevkJ != kJ) {
                                //cvmGet(matrizPuntosContorno,k-1,1)!=cvmGet(matrizPuntosContorno,k,1)

                                //auxiliar= (cvmGet(matrizPuntosReticula,i,1)-cvmGet(matrizPuntosContorno,k-1,1))*(cvmGet(matrizPuntosContorno,k,0)-cvmGet(matrizPuntosContorno,k-1,0))/(cvmGet(matrizPuntosContorno,k,1)-cvmGet(matrizPuntosContorno,k-1,1))+cvmGet(matrizPuntosContorno,k-1,0);
                                auxiliar = (actJ - prevkJ) * (kI - prevkI) / (kJ - prevkJ) + prevkI;
                                if (prevkI == kI || actI <= auxiliar) {
                                    //cvmGet(matrizPuntosContorno,k-1,0) == cvmGet(matrizPuntosContorno,k,0) || cvmGet(matrizPuntosReticula,i,0) <= auxiliar
                                    cortes++;

                                }

                            }

                        }

                    }

                }

            }

            // Si el numero de cortes es mayor que 0 y es par el punto es exterior y en caso contrario interior.

            if (cortes > 0) {

                if (cortes % 2 != 0) {
                    matrizPuntosReticula->at<double>(i, 4) = 1;

                } else {
                    matrizPuntosReticula->at<double>(i, 4) = 0;

                }

            }

        }

    }

}

bool extractor::esVertice(Mat *matrizPuntosContorno, Mat *&matrizPuntosReticula, int pos) {

    int i = 0;  // Variable para recorrer el vector de puntos del contorno.
    bool enc = false; // Variable para conocer si hemos hallado el punto en el vector o no.

    // Comprueba si un punto del vector de puntos del contorno coincide con un punto de la reticula.

    while ((!enc) && (i < 360)) {

        if (matrizPuntosContorno->at<double>(i, 0) == matrizPuntosReticula->at<double>(pos, 0)) {

            if (matrizPuntosContorno->at<double>(i, 1) == matrizPuntosReticula->at<double>(pos, 1)) {

                enc = true;
                matrizPuntosReticula->at<double>(pos, 4) = 2; // Indica que el punto esta en la frontera.

            }

        }

        i++;

    }

    return enc;

}

void extractor::corregirErroresReticula(Mat *&matrizPuntosReticula, int tamanoReticula, int anchoReticula) {

    int i; // Variables para recorrer los bucles.
    double actY, prePos, posPos;

    for (i = 0; i < tamanoReticula; i++) {
        actY = matrizPuntosReticula->at<double>(i, 3);
        prePos = matrizPuntosReticula->at<double>(i - 1, 4);
        posPos = matrizPuntosReticula->at<double>(i + 1, 4);

        if ((actY != 0) && (actY != anchoReticula - 1)) {

            if ((prePos == 1) && (posPos == 1)) {
                matrizPuntosReticula->at<double>(i, 4) = 1;

            }

            if ((prePos == 0) && (posPos == 0)) {
                matrizPuntosReticula->at<double>(i, 4) = 0;

            }

        }

    }

}

void extractor::actualizarPuntosFrontera(Mat *&matrizPuntosReticula, int tamanoReticula, int anchoReticula) {

    int i; // Variables para recorrer los bucles
    double actState;

    for (i = anchoReticula + 1; i < tamanoReticula - anchoReticula; i++) {

        // Comprueba si los puntos vecinos a un punto interior son exteriores y en ese caso lo transforma en punto frontera.
        actState = matrizPuntosReticula->at<double>(i, 4);
        if (actState == 1) {

            if ((matrizPuntosReticula->at<double>(i - 1, 4) == 0) ||
                (matrizPuntosReticula->at<double>(i + 1, 4) == 0) ||
                (matrizPuntosReticula->at<double>(i - anchoReticula, 4) == 0) ||
                (matrizPuntosReticula->at<double>(i + anchoReticula, 4) == 0)) {

                matrizPuntosReticula->at<double>(i, 4) = 2;

            }

        }

    }

}

void extractor::volcarPuntosIoF(Mat *matrizPuntosReticula, Mat *&matrizPuntosReticulaIoF, int tamanoReticula) {

    int i, j = 0; //Variables para recorrerr los bucles.

    for (i = 0; i < tamanoReticula; i++) {

        if (matrizPuntosReticula->at<double>(i, 4) != 0) {

            matrizPuntosReticulaIoF->at<double>(j, 0) = matrizPuntosReticula->at<double>(i, 0);
            matrizPuntosReticulaIoF->at<double>(j, 1) = matrizPuntosReticula->at<double>(i, 1);
            matrizPuntosReticulaIoF->at<double>(j, 2) = matrizPuntosReticula->at<double>(i, 2);
            matrizPuntosReticulaIoF->at<double>(j, 3) = matrizPuntosReticula->at<double>(i, 3);
            matrizPuntosReticulaIoF->at<double>(j, 4) = matrizPuntosReticula->at<double>(i, 4);

            j++;

        }

    }

}

void extractor::volcarPuntosFrontera(Mat *matrizPuntosReticulaIoF, Mat *&matrizPuntosReticulaF, int tamanoReticulaIoF) {

    int i, j = 0; // Variables para recorrer los bucles.

    for (i = 0; i < tamanoReticulaIoF; i++) {

        if (matrizPuntosReticulaIoF->at<double>(i, 4) == 2) {

            matrizPuntosReticulaF->at<double>(j, 0) = matrizPuntosReticulaIoF->at<double>(i, 0);
            matrizPuntosReticulaF->at<double>(j, 1) = matrizPuntosReticulaIoF->at<double>(i, 1);
            matrizPuntosReticulaF->at<double>(j, 2) = matrizPuntosReticulaIoF->at<double>(i, 2);
            matrizPuntosReticulaF->at<double>(j, 3) = matrizPuntosReticulaIoF->at<double>(i, 3);
            matrizPuntosReticulaF->at<double>(j, 4) = matrizPuntosReticulaIoF->at<double>(i, 4);
            j++;

        }

    }

}

void extractor::obtenerMatrizU(Mat *matrizPuntoReticulaIoF, Mat *matrizFrontera, Mat *&matrizU, int &tamanoMatU, int p,
                               int tamanoReticulaIoF, int tamanoReticulaF) {

    int i, j; // Indice para recorrer los bucles.

    j = 0;

    // Recorremos la matriz IoF para ver si el punto elegido es o no frontera y en caso de que no sea
    // frontera calculamos la distancia que hay entre el punto elegido y los puntos de la matriz IoF
    double pointState, actState;
    for (i = 0; i < tamanoReticulaIoF - 1; i++) {

        actState = matrizPuntoReticulaIoF->at<double>(i, 4);
        pointState = matrizPuntoReticulaIoF->at<double>(p, 4);
        if ((pointState != 2) && (actState != 2) && matrizPuntoReticulaIoF->at<double>(i, 1) != 0. &&
            matrizPuntoReticulaIoF->at<double>(i, 0) != 0.) {

            if (!interseccionSegmentos(matrizPuntoReticulaIoF, p, i, matrizFrontera, tamanoReticulaF) &&
                j < matrizU->rows) {

                matrizU->at<double>(j, 0) = matrizPuntoReticulaIoF->at<double>(p, 0);
                matrizU->at<double>(j, 1) = matrizPuntoReticulaIoF->at<double>(p, 1);
                matrizU->at<double>(j, 2) = matrizPuntoReticulaIoF->at<double>(i, 0);
                matrizU->at<double>(j, 3) = matrizPuntoReticulaIoF->at<double>(i, 1);
                matrizU->at<double>(j, 4) = sqrt(
                        (pow(matrizPuntoReticulaIoF->at<double>(i, 0) - matrizPuntoReticulaIoF->at<double>(p, 0), 2)) +
                        (pow(matrizPuntoReticulaIoF->at<double>(i, 1) - matrizPuntoReticulaIoF->at<double>(p, 1),
                             2))); //modulo vector
                j++;

            }

        }

    }

    tamanoMatU = j;

}

void extractor::obtenerRectanguloMaximo(Mat *matrizPuntoReticulaIoF, Mat *matrizFrontera, Mat *matrizU, int p,
                                        int tamanoReticulaIoF, int tamanoMatrizF, int tamanoMatrizU, int &ocupacion,
                                        double &minArea, listaRectangulo &lista) {

    int i, j; // Indices para recorrer los bucles.
    double area; // Variable que almacena las areas temporales que se van calculando para los distintos rectangulos generados.
    int puntoIx, puntoIy, puntoJx, puntoJy, puntoKx, puntoKy, puntoSx, puntoSy; // Variable que almacena los 4 puntos de los extremos del rectangulo.
    rectangulo R;

    for (i = 0; i < tamanoMatrizU - 2; i++) {

        for (j = i; j < tamanoMatrizU - 1; j++) {

            // Comprobamos si los segmentos seleccionados son perpendiculares.

            if (esPerpendicular(matrizU, i, j)) {

                puntoIx = matrizU->at<double>(p, 0);
                puntoIy = matrizU->at<double>(p, 1);
                puntoJx = matrizU->at<double>(i, 2);
                puntoJy = matrizU->at<double>(i, 3);
                puntoKx = matrizU->at<double>(j, 2);
                puntoKy = matrizU->at<double>(j, 3);
                puntoSx = puntoJx - puntoIx + puntoKx;
                puntoSy = puntoJy - puntoIy + puntoKy;

                // Comprobamos que los nuevos segmentos pertenezcan a la matriz U.

                if (puntoSx != puntoIx && puntoSy != puntoIy &&
                    (perteneceU(puntoKx, puntoKy, puntoSx, puntoSy, matrizPuntoReticulaIoF, matrizFrontera,
                                tamanoReticulaIoF, tamanoMatrizF)) &&
                    (perteneceU(puntoJx, puntoJy, puntoSx, puntoSy, matrizPuntoReticulaIoF, matrizFrontera,
                                tamanoReticulaIoF, tamanoMatrizF))) {

                    // Calculamos el area temporal.

                    area = matrizU->at<double>(i, 4) * matrizU->at<double>(j, 4);

                    // Si el area calculada es mayor que el area maxima, sustituimos el nuevo area maxima por la anterior y
                    // actualizamos los nuevos puntos vertices del rectangulo de area maxima.

                    if (area == (int) area) {

                        R.area = area;
                        R.puntoIx = puntoIx;
                        R.puntoJx = puntoJx;
                        R.puntoKx = puntoKx;
                        R.puntoSx = puntoSx;
                        R.puntoIy = puntoIy;
                        R.puntoJy = puntoJy;
                        R.puntoKy = puntoKy;
                        R.puntoSy = puntoSy;

                        insertarRectangulo(R, lista, ocupacion, minArea);

                    }

                }

            }

        }

    }

}

bool extractor::interseccionSegmentos(Mat *matrizPuntoReticula, int p1, int p2, Mat *matrizFrontera,
                                      int tamanoMatrizFrontera) {

    int i; // Variable para recorrer el vector con los puntos frontera.
    double d1, d2, d3, d4; // Valores que almacenan la direccion de los segmentos.
    int p1x, p1y, p2x, p2y, p3x, p3y;
    bool enc = false; // Variable que nos indica si hay o no interseccion de segmentos.

    for (i = 0; i < tamanoMatrizFrontera - 1; i++) {

        if (!enc) {

            p1x = matrizFrontera->at<double>(i, 0);
            p1y = matrizFrontera->at<double>(i, 1);
            p2x = matrizFrontera->at<double>(i + 1, 0);
            p2y = matrizFrontera->at<double>(i + 1, 1);
            p3x = matrizPuntoReticula->at<double>(p1, 0);
            p3y = matrizPuntoReticula->at<double>(p1, 1);
            d1 = direction(p1x, p1y, p2x, p2y, p3x, p3y);

            p1x = matrizFrontera->at<double>(i, 0);
            p1y = matrizFrontera->at<double>(i, 1);
            p2x = matrizFrontera->at<double>(i + 1, 0);
            p2y = matrizFrontera->at<double>(i + 1, 1);
            p3x = matrizPuntoReticula->at<double>(p2, 0);
            p3y = matrizPuntoReticula->at<double>(p2, 1);
            d2 = direction(p1x, p1y, p2x, p2y, p3x, p3y);

            p1x = matrizPuntoReticula->at<double>(p1, 0);
            p1y = matrizPuntoReticula->at<double>(p1, 1);
            p2x = matrizPuntoReticula->at<double>(p2, 0);
            p2y = matrizPuntoReticula->at<double>(p2, 1);
            p3x = matrizFrontera->at<double>(i, 0);
            p3y = matrizFrontera->at<double>(i, 1);
            d3 = direction(p1x, p1y, p2x, p2y, p3x, p3y);

            p1x = matrizPuntoReticula->at<double>(p1, 0);
            p1y = matrizPuntoReticula->at<double>(p1, 1);
            p2x = matrizPuntoReticula->at<double>(p2, 0);
            p2y = matrizPuntoReticula->at<double>(p2, 1);
            p3x = matrizFrontera->at<double>(i + 1, 0);
            p3y = matrizFrontera->at<double>(i + 1, 1);
            d4 = direction(p1x, p1y, p2x, p2y, p3x, p3y);

            if ((((d1 > 0) && (d2 < 0)) || ((d1 < 0) && (d2 > 0))) &&
                (((d3 > 0) && (d4 < 0)) || ((d3 < 0) && (d4 > 0)))) {

                enc = true;

            } else {

                p1x = matrizFrontera->at<double>(i, 0);
                p1y = matrizFrontera->at<double>(i, 1);
                p2x = matrizFrontera->at<double>(i + 1, 0);
                p2y = matrizFrontera->at<double>(i + 1, 1);
                p3x = matrizPuntoReticula->at<double>(p1, 0);
                p3y = matrizPuntoReticula->at<double>(p1, 1);

                if ((d1 == 0) && (enSegmento(p1x, p1y, p2x, p2y, p3x, p3y))) {

                    enc = true;

                } else {

                    p1x = matrizFrontera->at<double>(i, 0);
                    p1y = matrizFrontera->at<double>(i, 1);
                    p2x = matrizFrontera->at<double>(i + 1, 0);
                    p2y = matrizFrontera->at<double>(i + 1, 1);
                    p3x = matrizPuntoReticula->at<double>(p2, 0);
                    p3y = matrizPuntoReticula->at<double>(p2, 1);

                    if ((d2 == 0) && (enSegmento(p1x, p1y, p2x, p2y, p3x, p3y))) {

                        enc = true;

                    } else {

                        p1x = matrizPuntoReticula->at<double>(p1, 0);
                        p1y = matrizPuntoReticula->at<double>(p1, 1);
                        p2x = matrizPuntoReticula->at<double>(p2, 0);
                        p2y = matrizPuntoReticula->at<double>(p2, 1);
                        p3x = matrizFrontera->at<double>(i, 0);
                        p3y = matrizFrontera->at<double>(i, 1);

                        if ((d3 == 0) && (enSegmento(p1x, p1y, p2x, p2y, p3x, p3y))) {

                            enc = true;

                        } else {

                            p1x = matrizPuntoReticula->at<double>(p1, 0);
                            p1y = matrizPuntoReticula->at<double>(p1, 1);
                            p2x = matrizPuntoReticula->at<double>(p2, 0);
                            p2y = matrizPuntoReticula->at<double>(p2, 1);
                            p3x = matrizFrontera->at<double>(i + 1, 0);
                            p3y = matrizFrontera->at<double>(i + 1, 1);

                            if ((d4 == 0) && (enSegmento(p1x, p1y, p2x, p2y, p3x, p3y))) {

                                enc = true;

                            }

                        }

                    }

                }

            }

        }

    }
    return enc;
}

bool extractor::esPerpendicular(Mat *matrizU, int p1, int p2) {

    bool enc = false; // Variable que nos devuelve si el angulo formado por las rectas de estos dos puntos es o no perpendicular.
    int producto; // Variable que almacena el valor del producto vectorial de dos rectas.

    producto = (
            ((matrizU->at<double>(p1, 2) - matrizU->at<double>(p1, 0)) *
             (matrizU->at<double>(p2, 2) - matrizU->at<double>(p2, 0))) +
            ((matrizU->at<double>(p1, 3) - matrizU->at<double>(p1, 1)) *
             (matrizU->at<double>(p2, 3) - matrizU->at<double>(p2, 1))));

    if (producto == 0) {

        enc = true;

    }

    return enc;

}

bool extractor::perteneceU(int p1x, int p1y, int p2x, int p2y, Mat *matrizReticulaIoF, Mat *matrizFrontera,
                           int tamanoReticulaIoF,
                           int tamanoReticulaF) {

    int i; // Variable para recorrer los bucles.
    int res = 0; // Variable para controlar cuantos puntos hemos encontrado en la matriz IoF.
    int p, q; //Puntos auxiliares de la reticula para comprobar si pertenecen o no a la matriz U.

    // Buscamos los dos puntos p1 y p2 en la matriz IoF.

    for (i = 0; ((i < tamanoReticulaIoF) && (res != 2)); i++) {

        if (matrizReticulaIoF->at<double>(i, 0) == p1x) {

            if (matrizReticulaIoF->at<double>(i, 1) == p1y) {

                if (matrizReticulaIoF->at<double>(i, 4) != 2) {

                    p = i;

                    if (res == 0) {

                        res = 1;

                    } else {

                        res = 2;

                    }

                }

            }

        }

        if (matrizReticulaIoF->at<double>(i, 0) == p2x) {

            if (matrizReticulaIoF->at<double>(i, 1) == p2y) {

                if (matrizReticulaIoF->at<double>(i, 4) != 2) {

                    q = i;

                    if (res == 0) {

                        res = 1;

                    } else {

                        res = 2;

                    }

                }

            }

        }

    }

    // Si hemos encontrado los dos puntos, comprobamos que son interiores a la matriz y que no interseccionan
    // con las rectas formadas por los puntos frontera.

    if (res == 2) {

        if (!(interseccionSegmentos(matrizReticulaIoF, p, q, matrizFrontera, tamanoReticulaF))) {

            return true;

        }

    }

    return false;

}

void extractor::insertarRectangulo(rectangulo R, listaRectangulo &lista, int &ocupacion, double &area) {

    listaRectangulo aux;
    int i = 0;
    int bandera = 0;

    while ((i < ocupacion) && (bandera == 0)) {

        if (lista[i].area > R.area) {

            aux[i].area = lista[i].area;
            aux[i].puntoIx = lista[i].puntoIx;
            aux[i].puntoJx = lista[i].puntoJx;
            aux[i].puntoKx = lista[i].puntoKx;
            aux[i].puntoSx = lista[i].puntoSx;
            aux[i].puntoIy = lista[i].puntoIy;
            aux[i].puntoJy = lista[i].puntoJy;
            aux[i].puntoKy = lista[i].puntoKy;
            aux[i].puntoSy = lista[i].puntoSy;
            i++;

        } else {


            aux[i].area = R.area;
            aux[i].puntoIx = R.puntoIx;
            aux[i].puntoJx = R.puntoJx;
            aux[i].puntoKx = R.puntoKx;
            aux[i].puntoSx = R.puntoSx;
            aux[i].puntoIy = R.puntoIy;
            aux[i].puntoJy = R.puntoJy;
            aux[i].puntoKy = R.puntoKy;
            aux[i].puntoSy = R.puntoSy;
            bandera = 1;

        }

    }
    if (i == ocupacion && bandera == 0 && ocupacion < MAX_TAM_LIST) {

        aux[i].area = R.area;
        aux[i].puntoIx = R.puntoIx;
        aux[i].puntoJx = R.puntoJx;
        aux[i].puntoKx = R.puntoKx;
        aux[i].puntoSx = R.puntoSx;
        aux[i].puntoIy = R.puntoIy;
        aux[i].puntoJy = R.puntoJy;
        aux[i].puntoKy = R.puntoKy;
        aux[i].puntoSy = R.puntoSy;
    }

    if (bandera == 1) {
        for (int j = i; j < ocupacion - 1; j++) {

            aux[j + 1].area = lista[j].area;
            aux[j + 1].puntoIx = lista[j].puntoIx;
            aux[j + 1].puntoJx = lista[j].puntoJx;
            aux[j + 1].puntoKx = lista[j].puntoKx;
            aux[j + 1].puntoSx = lista[j].puntoSx;
            aux[j + 1].puntoIy = lista[j].puntoIy;
            aux[j + 1].puntoJy = lista[j].puntoJy;
            aux[j + 1].puntoKy = lista[j].puntoKy;
            aux[j + 1].puntoSy = lista[j].puntoSy;

        }
    }

    if (ocupacion < MAX_TAM_LIST) {

        ocupacion++;

    }

    for (i = 0; i < ocupacion; i++) {

        lista[i].area = aux[i].area;
        lista[i].puntoIx = aux[i].puntoIx;
        lista[i].puntoJx = aux[i].puntoJx;
        lista[i].puntoKx = aux[i].puntoKx;
        lista[i].puntoSx = aux[i].puntoSx;
        lista[i].puntoIy = aux[i].puntoIy;
        lista[i].puntoJy = aux[i].puntoJy;
        lista[i].puntoKy = aux[i].puntoKy;
        lista[i].puntoSy = aux[i].puntoSy;

    }

}

double extractor::direction(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y) {

    int aX = p3x - p1x;
    int aY = p3y - p1y;
    int bX = p2x - p1x;
    int bY = p2y - p1y;
    double moduloU = sqrt(pow(aX, 2) + pow(aY, 2));
    double moduloV = sqrt(pow(bX, 2) + pow(bY, 2));
    double angulo = ((aX * bX) + (aY * bY)) / (moduloU * moduloV);

    return (moduloU * moduloV * angulo);

}

bool extractor::enSegmento(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y) {

    if (min(p1x, p2x) <= p3x) {

        if (max(p1x, p2x) >= p3x) {

            if (min(p1y, p2y) <= p3y) {

                if (max(p1y, p2y) >= p3y) {

                    return true;

                }

            }

        }

    }

    return false;

}
