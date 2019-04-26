#ifndef FEATURE_SET_H   
#define FEATURE_SET_H

#include <iostream>
#include <string>
#include <map>

class FeatureSet {

public:

    int nSats;
//  int nSatellitesChanged
    double signalToNoise;
    std::map<int, double> pseudoranges;
    std::map<int, double> carrierPhases;
    double dopplerMeasured;
    double gapFromLastPos;
//  double eastingFromReal
//  double northingFromReal
//  double heightFromReal
    double velocity;
    double acceleration;
    std::map<int, double> amplitudes;
//  ___ timewithNoLock


    void printMap(std::map<int, double> m, std::string name) {

        std::cout   << "  " << name << ": {";

        for (auto it = m.begin(); it != m.end(); ++it) {

            std::cout << " " << it->first << ": " << it->second << ",";
        }

        std::cout << " }" << std::endl;
    }

    void printFeatureSet() {

        std::cout   << "FEATURES:" << std::endl
                    << "  nSats: " << nSats << std::endl
                    << "  signalToNoise: " << signalToNoise << std::endl;

        printMap(pseudoranges, "pseudoranges");
        printMap(carrierPhases,"carrier phases");

        std::cout   << "  dopplerMeasured: " << dopplerMeasured << std::endl
                    << "  gapFromLastPos: " << "NA" << std::endl
                    << "  eastingFromReal: " << "NA" << std::endl
                    << "  northingFromReal: " << "NA" << std::endl
                    << "  heightFromReal: " << "NA" << std::endl
                    << "  velocity: " << "NA" << std::endl
                    << "  acceleration: " << "NA" << std::endl;

        printMap(amplitudes, "amplitudes");
    }
};



#endif



