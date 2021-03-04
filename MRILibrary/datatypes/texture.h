//
// Created by avatar01 on 6/09/18.
//

#ifndef MRILIBRARY_DATATYPES_TEXTURE_H
#define MRILIBRARY_DATATYPES_TEXTURE_H

class vectorGLCM {
private:
    std::vector<double> myv;
public:
    vectorGLCM() : myv(10, 0) {};

    double getEnergy() { return myv[0]; };

    double getEntropy() { return myv[1]; };

    double getCorrelation() { return myv[2]; };

    double getHaralick() { return myv[3]; };

    double getIDM() { return myv[4]; };

    double getInertia() { return myv[5]; };

    double getCS() { return myv[6]; };

    double getCP() { return myv[7]; };

    double getContrast() { return myv[8]; };

    double getDissimilarity() { return myv[9]; };

    double &operator[](int i) { return myv[i]; };

    uint getSize() { return myv.size(); };

    ~vectorGLCM() {};

};

// 0: Energy
// 1: Entropy
// 2: Correlation
// 3: HC Haralick's Correlation
// 4: IDM Inverse Different Moment
// 5: Inertia
// 6: CS Claster Shade
// 7: CP Cluster Prominence
// 8: Contrast
// 9: Dissimilarity


class vectorNGLDM{
private:
    std::vector<double> myv;
public:
    vectorNGLDM() : myv(5, 0) {};

    double getSNE() { return myv[0]; };

    double getLNE() { return myv[1]; };

    double getNNU() { return myv[2]; };

    double getSM() { return myv[3]; };

    double getENT() { return myv[4]; };

    double &operator[](int i) { return myv[i]; };

    uint getSize() { return myv.size(); };

    ~vectorNGLDM() {};
};

// 0: SNE Small Number Emphasis
// 1: LNE Large Number Emphasis
// 2: NNU Number Non Unifomity
// 3: SM Second Moment
// 4: ENT Entropy

class vectorGLRLM{
private:
    std::vector<double> myv;
public:
    vectorGLRLM() : myv(11, 0) {};

    double getLRE() { return myv[0]; };

    double getSRE() { return myv[1]; };

    double getGLNU() { return myv[2]; };

    double getRLNU() { return myv[3]; };

    double getRPC() { return myv[4]; };

    double getLGRE() { return myv[5]; };

    double getHGRE() { return myv[6]; };

    double getSRLGE() { return myv[7]; };

    double getSRHGE() { return myv[8]; };

    double getLRLGE() { return myv[9]; };

    double getLRHGE() { return myv[10]; };

    double &operator[](int i) { return myv[i]; };

    uint getSize() { return myv.size(); };

    ~vectorGLRLM() {};
};

// 0: LRE: Long Run Emphasis
// 1: SRE: Short Run Emphasis
// 2: GLNU: Grey Level Non Uniformity
// 3: RLNU: Run Length Non Uniformity
// 4: RPC: Run PerCentage
// 5: LGRE: Peso de las secuencias de nivel de gris bajo
// 6: HGRE: Peso de las secuencias de nivel de gris alto
// 7: SRLGE: Peso de las secuencias cortas de nivel de gris bajo
// 8: SRHGE: Peso de las secuencias cortas de nivel de gris alto
// 9: LRLGE: Peso de las secuencias largas de nivel de gris bajo
// 10: LRHGE: Peso de las secuencias largas de nivel de gris alto


/*
 *  Estructura creada para poder recorrer ventanas alrededor de un pixel de forma iterativa
 *
 *  Almacenar desplazamientos en i,j. Para luego sumarselos directamente a las coordenadas del pixel tratado
 *
 */
typedef struct kernelDis {
    int i;
    int j;

    kernelDis(int i_, int j_) {
        i = i_;
        j = j_;
    };
} kernelDis;

#endif
