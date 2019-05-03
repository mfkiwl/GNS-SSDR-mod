
#include "feature_set.h"
#include <chrono>

using namespace std::chrono;

void FeatureSet::updateFeaturesPerChannel(  std::map<int, Gnss_Synchro> &synchros, 
                                std::shared_ptr<rtklib_solver> pvt_solver) {


    assembleFeatures(synchros, pvt_solver);

    featuresPerChannel.clear();

    for (auto it = currentPRNs.begin(); it != currentPRNs.end(); ++it) {

        featuresPerChannel[*it] = featuresForChannel(*it);
    }
}

std::map<std::string, double> FeatureSet::featuresForChannel(int channel_id) {

    std::map<std::string, double> features; 

    features["channel_id"] = channel_id;
    features["minDoppler"] = 0;
    features["stddevDoppler"] = 0;
    features["minValidSats"] = 0;
    features["minPseudoRange"] = 0;
    features["minSigToNoise"] = 0;
    features["stddevValidsats"] = 0;
    features["stddevSigToNoise"] = 0;
    features["stddevPseudoranges"] = 0;
    features["maxSatsChanged"] = 0;
    features["avgValidSats"] = 0;
    features["avgPseudoRanges"] = 0;
    features["avgDoppler"] = 0;
    features["maxSigToNoise"] = 0;
    features["maxCarrierPhase"] = 0;
    features["maxValidSats"] = 0;
    features["maxCarrierPhase"] = 0;
    features["avgSigToNoise"] = 0;
    features["minHeightFromReal"] = 0;
    features["maxAmplitude"] = 0;

    return features;
}


void FeatureSet::assembleFeatures(std::map<int, Gnss_Synchro> &synchros, 
                                  std::shared_ptr<rtklib_solver> pvt_solver) {

    signalToNoise = 0;
    signalToNoise = 0;
    int counter = 0;
    int dt = 0;

    lastPRNs = currentPRNs;
    currentPRNs.clear();

    for (auto it = synchros.begin(); it != synchros.end(); ++it) {

        counter++;

        Gnss_Synchro syn = it->second;
        int32_t channel_id = syn.Channel_ID;
        currentPRNs.push_back(channel_id);

        if (counter == 1) {

            lastTime = currentTime;
            currentTime = syn.TOW_at_current_symbol_ms;
            dt = currentTime - lastTime;
        }
                                
        signalToNoise += syn.CN0_dB_hz;
        pseudorangePerChannel[channel_id] = syn.Pseudorange_m;
        carrierPhasePerChannel[channel_id] = syn.Carrier_phase_rads;
        dopplerMeasured += syn.Carrier_Doppler_hz;
        amplitudes[channel_id] = syn.amplitude;
    }

    signalToNoise /= counter;
    dopplerMeasured /= counter;
    nSats = counter;

    velocity = pvt_solver->get_speed_over_ground();
    lastVelocity = currentVelocity;
    currentVelocity = velocity;

    acceleration = (velocity - lastVelocity) / (static_cast<double> (dt) / 1000.0);

    lastPos = currentPos;

    currentPos = {
                   pvt_solver->get_latitude(),
                   pvt_solver->get_longitude(),
                   pvt_solver->get_height()
                 };

    distancesFromReal(currentPos);

    updatePosQueue(currentPos);

    gapFromLastPos = posDistance(currentPos, lastPos);

    nSatsChanged = comparePRNs();

    updateTimeWithNoLock();
}

void FeatureSet::printFeatures() {

    std::cout   << "FEATURES:" << std::endl;

    for (auto it = featuresPerChannel.begin(); it != featuresPerChannel.end(); ++it) {

        int ch_id = it->first;
        std::map <std::string, double> chData = it->second;

        std::cout << "  Ch " << ch_id << std::endl;

        for (auto it1 = chData.begin(); it1 != chData.end(); ++it1) {

            std::cout << "    " << it1->first << ": " << it1->second << std::endl;
        }
    }


    printMap(amplitudes, "amplitudes");

    std::cout << "  timeWithNoLock: " << timeWithNoLock << std::endl;
}


double FeatureSet::posDistance(Position p1, Position p2) {

    double dLon = p1.longitude - p2.longitude;
    double dLat = p1.latitude - p2.latitude;
    double dHgt = p1.height - p2.height; 

    return sqrt(dLon * dLon + dLat * dLat + dHgt * dHgt);
}


int FeatureSet::comparePRNs() {

    if (lastPRNs.empty() && currentPRNs.empty()) {
        return 0;
    }

    else if (! lastPRNs.empty() && currentPRNs.empty()) {
        return lastPRNs.size();
    }

    else if (lastPRNs.empty() && ! currentPRNs.empty()) {
        return currentPRNs.size();
    }
    else {

        int count = 0;
        size_t i = 0;

        std::vector<int> v1;
        std::vector<int> v2;

        if (lastPRNs.size() <= currentPRNs.size()) {
            v1 = lastPRNs;
            v2 = currentPRNs;
        }
        else {
            v2 = lastPRNs;
            v1 = currentPRNs;
        }

        while (i < v1.size()) {

            if (! (std::find(v2.begin(), v2.end(), v1[i]) != v2.end())) {
                ++count;
            }

            ++i;
        }

        while (i < v2.size()) {

            if (! (std::find(v1.begin(), v1.end(), v2[i]) != v1.end())) {
                ++count;
            }

            ++i;
        }

        return count;
    }
}


void FeatureSet::printMap(std::map<int, double> m, std::string name) {

    std::cout   << "  " << name << ": {";

    for (auto it = m.begin(); it != m.end(); ++it) {

        std::cout << " " << it->first << ": " << it->second << ",";
    }

    std::cout << " }" << std::endl;
}

void FeatureSet::updatePosQueue(Position pos) {

    lastPositions.push_back(pos);

    if (lastPositions.size() > queueSize) {
        lastPositions.pop_front();
    } 

    queueAvg = {0, 0, 0};

    for (auto it = lastPositions.cbegin(); it != lastPositions.cend(); ++it) {

        queueAvg.longitude += it->longitude;
        queueAvg.latitude += it->latitude;
        queueAvg.height += it->height;
    }

    queueAvg.longitude /= lastPositions.size();
    queueAvg.latitude /= lastPositions.size();
    queueAvg.height /= lastPositions.size();
}

void FeatureSet::distancesFromReal(Position pos) {

    // real pos estimated from average over last 10 positions

    eastingFromReal = pos.longitude - queueAvg.longitude;
    northingFromReal = pos.latitude - queueAvg.latitude;
    heightFromReal = pos.height - queueAvg.height;
}

void FeatureSet::updateTimeWithNoLock() {

    if (noLock && currentPRNs.size() > 0) {
        timeWithNoLock = 0;
        noLock = false;
    }

    if (noLock && currentPRNs.size() == 0) {

        tLock_2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> dt = tLock_2 - tLock_1;
        timeWithNoLock = dt.count();
    }

    if (! noLock && currentPRNs.size() == 0) {

        timeWithNoLock = 0;
        tLock_1 = std::chrono::high_resolution_clock::now();
        noLock = true;
    }
}






















