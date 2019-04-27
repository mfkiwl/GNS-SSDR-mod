#ifndef FEATURE_SET_H   
#define FEATURE_SET_H

#include <iostream>
#include <string>
#include <map>
#include "gnss_synchro.h"
#include "rtklib_solver.h"

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


    void assembleFeatures(std::map<int, Gnss_Synchro> &synchros, std::shared_ptr<rtklib_solver> pvt_solver) {

        signalToNoise = 0;
        signalToNoise = 0;
        int counter = 0;

        for (auto it = synchros.begin(); it != synchros.end(); ++it) {

            counter++;

                Gnss_Synchro syn = it->second;
                int32_t channel_id = syn.Channel_ID;
                            
                signalToNoise += syn.CN0_dB_hz;
                pseudoranges[channel_id] = syn.Pseudorange_m;
                carrierPhases[channel_id] = syn.Carrier_phase_rads;
                dopplerMeasured += syn.Carrier_Doppler_hz;
                amplitudes[channel_id] = syn.amplitude;
        }

        signalToNoise /= counter;
        dopplerMeasured /= counter;
        nSats = counter;
    }


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



