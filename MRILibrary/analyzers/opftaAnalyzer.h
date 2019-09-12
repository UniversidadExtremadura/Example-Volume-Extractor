//
// Created by avatar01 on 7/09/18.
//

#ifndef MRILIBRARY_OPFTAANALYZER_H
#define MRILIBRARY_OPFTAANALYZER_H

#include "analyzer.h"
#include "../datatypes/opfta.h"

class opftaAnalyzer : public analyzer {
private:
    void FTA(Mat *&N, int columnas, int filas);

    void statistics(Mat *frac, int filas, int columnas, vectorOFCV &vector);

    double calcular_FD(int i, int j);

    double calcular_casilla(int k, int l, int kini, int kfin, int lini, int lfin);

public:
    opftaAnalyzer();

    opftaAnalyzer(const string &path);

    vectorOFCV opftaAnalysis();

    virtual ~opftaAnalyzer();
};


#endif //MRILIBRARY_OPFTAANALYZER_H
