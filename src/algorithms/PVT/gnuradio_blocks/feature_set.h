#ifndef FEATURE_SET_H   
#define FEATURE_SET_H

class FeatureSet {

public:

    int nSats;
//  int nSatellitesChanged
    double signalToNoise;
    std::map<int, double> pseudoranges;
    std::map<int, double> carrierPhases;
    double dopplerMeasured;
    double gapFromLastPos;
//  double eastingFromReeal
//  double northingFromReal
//  double heightFromReal
    double velocity;
    double acceleration;
    std::map<int, double> amplitudes;
//  ___ timewithNoLock
};

#endif



