#ifndef FEATURE_SET_H   
#define FEATURE_SET_H


#include "gnss_synchro.h"
#include "rtklib_solver.h"
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <float.h>
#include <iostream>


struct Position {

    double latitude;
    double longitude;
    double height;
};

class FeatureSet {

public:

    FeatureSet();
    ~FeatureSet();

    // data members

    std::map< int, std::map <std::string, double> > featuresPerChannel;

    // methods

    void updateFeaturesPerChannel(  std::map<int, Gnss_Synchro> &synchros, 
                                    std::shared_ptr<rtklib_solver> pvt_solver);

    void printFeatures(); // temporary, to be removed

    void writeCsvHeader();
    void writeCsvLine(std::map <std::string, double> instance);

    bool csvEnabled;

    void setLabel(std::string l);

private:

    long mainCounter = 0;

    std::string outName;
    std::string label;
    std::ofstream csvOut;

    // minima, maxima, averages and standard deviations
    // all data as double, for processing by classifiers

    double minDoppler = DBL_MAX;
    double avgDopplerCurrent = 0;
    double avgDopplerPrev = 0;
    double varDoppler = 0;

    std::map<int, double> minPseudoranges;
    std::map<int, double> avgPseudorangesPrev;
    std::map<int, double> avgPseudorangesCurrent;
    std::map<int, double> varPseudoranges;

    double minSignalToNoise = DBL_MAX;
    double maxSignalToNoise = - DBL_MAX;
    double avgSignalToNoisePrev = 0;
    double avgSignalToNoiseCurrent = 0;
    double varSignalToNoise = 0;
    
    double minValidSats = DBL_MAX;
    double maxValidSats = - DBL_MAX;
    double avgValidSatsPrev = 0;
    double avgValidSatsCurrent = 0;
    double varValidSats = 0;

    double maxSatsChanged = - DBL_MAX;

    std::map<int, double> maxCarrierPhases;

    double minHeightFromReal = DBL_MAX;

    std::map<int, double> maxAmplitudes;


    // instantaneous data

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



