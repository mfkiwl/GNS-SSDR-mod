#ifndef FEATURE_SET_H   
#define FEATURE_SET_H


#include "gnss_synchro.h"
#include "rtklib_solver.h"
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <iostream>


struct Position {

    double latitude;
    double longitude;
    double height;
};

class FeatureSet {

public:

    // data members

    // In an other class specific classifiers may be instantiated for each channel,
    std::map< int, std::map <std::string, double> > featuresPerChannel;

    // methods

    void updateFeaturesPerChannel(  std::map<int, Gnss_Synchro> &synchros, 
                                    std::shared_ptr<rtklib_solver> pvt_solver);

    void printFeatures(); // temporary, to be removed

private:

    // data members

    int lastTime; // times are in ms
    int currentTime;

    Position lastPos;
    Position currentPos;

    float lastVelocity;  // velocities in m/s
    float currentVelocity;

    std::vector<int> lastPRNs;
    std::vector<int> currentPRNs;

    std::deque<Position> lastPositions;
    size_t queueSize = 10;
    Position queueAvg;

    bool noLock = true;
    std::chrono::high_resolution_clock::time_point tLock_1;
    std::chrono::high_resolution_clock::time_point tLock_2;
    std::chrono::duration<double> dTlock;

    int nSats;
    int nSatsChanged;
    double signalToNoise;
    std::map<int, double> pseudorangePerChannel;
    std::map<int, double> carrierPhasePerChannel;
    double dopplerMeasured;
    double gapFromLastPos;  
    double eastingFromReal;  // real pos estimated from average over last 10 positions
    double northingFromReal;
    double heightFromReal;
    double velocity; // m/s
    double acceleration; // m/s^2
    std::map<int, double> amplitudes;
    double timeWithNoLock;

    // methods

    void assembleFeatures(std::map<int, Gnss_Synchro> &synchros, 
                          std::shared_ptr<rtklib_solver> pvt_solver);

    double posDistance(Position p1, Position p2);

    int comparePRNs();

    void printMap(std::map<int, double> m, std::string name);

    void updatePosQueue(Position pos);

    void distancesFromReal(Position pos);

    void updateTimeWithNoLock();

    std::map<std::string, double> featuresForChannel(int channel);
};



#endif



