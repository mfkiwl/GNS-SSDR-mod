#ifndef FEATURE_SET_H   
#define FEATURE_SET_H


#include "gnss_synchro.h"
#include "rtklib_solver.h"
#include <map>
#include <ctime>

#include <iostream>

using namespace std::chrono;

struct Position {

    double latitude;
    double longitude;
    double height;
};

class FeatureSet {

public:

    // data mebers

    int nSats;
//  int nSatellitesChanged
    double signalToNoise;
    std::map<int, double> pseudoranges;
    std::map<int, double> carrierPhases;
    double dopplerMeasured;
//    double gapFromLastPos;
//  double eastingFromReal
//  double northingFromReal
//  double heightFromReal
    double velocity; // m/s
    double acceleration; // m/s^2
    std::map<int, double> amplitudes;
//  ___ timewithNoLock

    // methods

    void assembleFeatures(std::map<int, Gnss_Synchro> &synchros, 
                          std::shared_ptr<rtklib_solver> pvt_solver) {

        

        signalToNoise = 0;
        signalToNoise = 0;
        int counter = 0;
        int dt = 0;

        for (auto it = synchros.begin(); it != synchros.end(); ++it) {

            counter++;

            Gnss_Synchro syn = it->second;
            int32_t channel_id = syn.Channel_ID;

            if (counter == 1) {

                lastTime = currentTime;
                currentTime = syn.TOW_at_current_symbol_ms;
                dt = currentTime - lastTime;
            }
                                
            signalToNoise += syn.CN0_dB_hz;
            pseudoranges[channel_id] = syn.Pseudorange_m;
            carrierPhases[channel_id] = syn.Carrier_phase_rads;
            dopplerMeasured += syn.Carrier_Doppler_hz;
            amplitudes[channel_id] = syn.amplitude;
        }

        signalToNoise /= counter;
        dopplerMeasured /= counter;
        nSats = counter;

        velocity = pvt_solver->get_speed_over_ground();
        lastVelocity = currentVelocity;
        currentVelocity = velocity;

        lastPos = currentPos;
        currentPos = {
                        pvt_solver->get_latitude(),
                        pvt_solver->get_longitude(),
                        pvt_solver->get_height()
                     };

        acceleration = (velocity - lastVelocity) / (static_cast<double> (dt) / 1000.0);
    }

    void printFeatures() {

        std::cout   << "FEATURES:" << std::endl
                    << "  nSats: " << nSats << std::endl
                    << "  satsChanged: " << "NA" << std::endl
                    << "  signalToNoise: " << signalToNoise << std::endl;

        printMap(pseudoranges, "pseudoranges");
        printMap(carrierPhases,"carrier phases");

        std::cout   << "  dopplerMeasured: " << dopplerMeasured << std::endl
                    << "  gapFromLastPos: " << "NA" << std::endl
                    << "  eastingFromReal: " << "NA" << std::endl
                    << "  northingFromReal: " << "NA" << std::endl
                    << "  heightFromReal: " << "NA" << std::endl
                    << "  velocity: " << velocity << " m/s" << std::endl
                    << "  acceleration: " << acceleration << " m/s^2" << std::endl;

        printMap(amplitudes, "amplitudes");
    }

private:

    // data members

    int lastTime; // times are in ms
    int currentTime;

    Position lastPos;
    Position currentPos;

    float lastVelocity;  // velocities in m/s
    float currentVelocity;

    // methods

    void printMap(std::map<int, double> m, std::string name) {

        std::cout   << "  " << name << ": {";

        for (auto it = m.begin(); it != m.end(); ++it) {

            std::cout << " " << it->first << ": " << it->second << ",";
        }

        std::cout << " }" << std::endl;
    }
};



#endif



