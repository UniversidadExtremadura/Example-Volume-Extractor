//
// Created by avatar01 on 10/09/18.
//

#ifndef MRILIBRARY_DATATYPES_EXTRACTOR_H
#define MRILIBRARY_DATATYPES_EXTRACTOR_H

#include <vector>

#define MAX_TAM_LIST 100

typedef struct {

    int puntoIx;
    int puntoIy;
    int puntoJx;
    int puntoJy;
    int puntoKx;
    int puntoKy;
    int puntoSx;
    int puntoSy;
    double area;

} rectangulo;

class listaRectangulo {
private:
    std::vector<rectangulo> myv;
public:
    listaRectangulo() : myv(MAX_TAM_LIST) {};

    virtual ~listaRectangulo() {};

    rectangulo &operator[](int i) { return myv[i]; };
};


#endif //MRILIBRARY_DATATYPES_EXTRACTOR_H
