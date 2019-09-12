//
// Created by avatar01 on 7/09/18.
//

#ifndef MRILIBRARY_DATATYPE_OPFTA_H
#define MRILIBRARY_DATATYPE_OPFTA_H

class vectorOFCV {
private:
    std::vector<double> myv;
public:
    vectorOFCV() : myv(7, 0) {};

    double getUniformity() { return myv[0]; };

    double getEntropy() { return myv[1]; };

    double getCorrelation() { return myv[2]; };

    double getHomogeneity() { return myv[3]; };

    double getInertia() { return myv[4]; };

    double getContrast() { return myv[5]; };

    double getEficency() { return myv[6]; };

    double &operator[](int i) { return myv[i]; };

    ~vectorOFCV() {};

};
// 0: Uniformity
// 1: Entropy
// 2: Correlation
// 3: Homogeneity
// 4: Inertia
// 5: Contrast
// 6: Eficency

#endif //MRILIBRARY_DATATYPE_OPFTA_H
