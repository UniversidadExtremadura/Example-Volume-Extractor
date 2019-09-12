//
// Created by avatar01 on 7/09/18.
//

#ifndef MRILIBRARY_CFAANALYZER_H
#define MRILIBRARY_CFAANALYZER_H

#include "analyzer.h"
#include <datatypes/cfa.h>


class cfaAnalyzer : public analyzer {
private:
    void obtenerFractales(std::vector<double> &N, double &p);

    void statistics(vectorCFA vectorCFA1, vectorFCV &vector);

public:
    cfaAnalyzer();

    cfaAnalyzer(const string &path);

    vectorCFA cfaAnalysis();

    vectorFCV ftaAnalysis();

    virtual ~cfaAnalyzer();

};


#endif //MRILIBRARY_CFAANALYZER_H
