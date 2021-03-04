//
// Created by avatar01 on 7/09/18.
//

#ifndef MRILIBRARY_CFA_H
#define MRILIBRARY_CFA_H


class vectorFCV {
private:
    std::vector<double> myv;
public:
    vectorFCV() : myv(10, 0) {};

    double getUniformity() { return myv[0]; };

    double getEntropy() { return myv[1]; };

    double getCorrelation() { return myv[2]; };

    double getIDM() { return myv[3]; };

    double getInertia() { return myv[4]; };

    double getContrast() { return myv[5]; };

    double getEmphasis() { return myv[6]; };

    double getJC() { return myv[7]; };

    double getCS() { return myv[8]; };

    double getCP() { return myv[9]; };

    double &operator[](int i) { return myv[i]; };

    uint getSize() { return myv.size(); };

    ~vectorFCV() {};

};

// 0: Uniformity
// 1: Entropy
// 2: Correlation
// 3: IDM Inverse Different Moment
// 4: Inertia
// 5: Contrast
// 6: Emphasis
// 7: JC Jorna's Correlation
// 8: CS Cluster Shade
// 9: CP Cluster Prominence


class vectorCFA {
private:
    std::vector<double> myv;
    double size;
public:
    void setSize(double size) {
        vectorCFA::size = size;
    }

    void setMyv(const std::vector<double> &myv) {
        vectorCFA::myv = myv;
    }

    const std::vector<double> &getMyv() const {
        return myv;
    }

    double getSize() const {
        return size;
    }

    double &operator[](int i) { return myv[i]; };
};


#endif //MRILIBRARY_CFA_H
