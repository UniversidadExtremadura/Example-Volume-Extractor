//
// Created by avatar01 on 10/09/18.
//

#ifndef MRILIBRARY_EXTRACTOR_H
#define MRILIBRARY_EXTRACTOR_H

#include <string>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include "../datatypes/extractor.h"

using namespace cv;
using namespace std;

class extractor {
private:

protected:
    Mat inputImg;
    string path;

    void obtenerContorno(Mat *&salida);

    void obtenerTamano(Mat *imagen, int &ancho, int &alto);

    void obtenerPuntosContorno(Mat *imagen, Mat *&matrizPuntosContorno);

    void cargarImagen(string nombre, Mat *&imagen);

    void obtenerPUNTA2ide(Mat *imagen, int &PUNTA2ideX, int &PUNTA2ideY);

    void obtenerPuntosRectanguloExterior(Mat *matrizPuntosContorno, int &xMin, int &xMax, int &yMin, int &yMax);

    void crearReticula(Mat *&matrizPuntosReticula, int distancia, int xMin, int xMax, int yMin,
                       int yMax, int tamanoRet);

    void asignarIoEoF(Mat *matrizPuntosContorno, Mat *&matrizPuntosReticula, int tamanoReticula);

    bool esVertice(Mat *matrizPuntosContorno, Mat *&matrizPuntosReticula, int pos);

    void corregirErroresReticula(Mat *&matrizPuntosReticula, int tamanoReticula, int anchoReticula);

    void actualizarPuntosFrontera(Mat *&matrizPuntosReticula, int tamanoReticula, int anchoReticula);

    void volcarPuntosIoF(Mat *matrizPuntosReticula, Mat *&matrizPuntosReticulaIoF, int tamanoReticula);

    void volcarPuntosFrontera(Mat *matrizPuntosReticulaIoF, Mat *&matrizPuntosReticulaF, int tamanoReticulaIoF);

    void obtenerMatrizU(Mat *matrizPuntoReticulaIoF, Mat *matrizFrontera, Mat *&matrizU, int &tamanoMatU, int p,
                        int tamanoReticulaIoF, int tamanoReticulaF);

    void obtenerRectanguloMaximo(Mat *matrizPuntoReticulaIoF, Mat *matrizFrontera, Mat *matrizU, int p,
                                 int tamanoReticulaIoF, int tamanoMatrizF, int tamanoMatrizU, int &ocupacion,
                                 double &minArea, listaRectangulo &lista);

    bool interseccionSegmentos(Mat *matrizPuntoReticula, int p1, int p2, Mat *matrizFrontera, int tamanoMatrizFrontera);

    bool esPerpendicular(Mat *matrizU, int p1, int p2);

    bool
    perteneceU(int p1x, int p1y, int p2x, int p2y, Mat *matrizReticulaIoF, Mat *matrizFrontera, int tamanoReticulaIoF,
               int tamanoReticulaF);

    void insertarRectangulo(rectangulo R, listaRectangulo &lista, int &ocupacion, double &area);

    double direction(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y);

    bool enSegmento(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y);

public:
    extractor();

    extractor(const string &path);

    void loadImage();

    void loadImage(string _path);

    virtual ~extractor() = 0;
};


#endif //MRILIBRARY_EXTRACTOR_H
