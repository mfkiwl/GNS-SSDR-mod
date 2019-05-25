
#include "record.h"
#include <iostream>
#include <string>
#include <math.h>

long recordCounter = 0;


Record::Record(std::map< std::string, double > features, std::string label)
    : features(features), label(label)
{
    id = recordCounter++;
}


long Record::getId() {
    return id;
}

void Record::removeFeature(std::string feat) {
    features.erase(feat);
}

std::map<std::string, double> Record::getFeatures() {
    return features;
}

int Record::getSize() {
    return features.size();
}

std::string Record::getLabel() {
    return label;
}

/*
void Record::adjustStandardDeviations() {

    for (auto it = features.begin(); it != features.end(); ++it) {

        std::string feature = it->first;
        double value = it->second;

        if (feature.rfind("var", 0) == 0) {
            it->second = sqrt(value);
        }
    }
}
*/

void Record::printRecord() {

    std::cout << id << "\t";

    for (auto it = features.begin(); it != features.end(); ++it) {

        std::cout << it->first << "=" << it->second << "\t";
    }

    std::cout << std::endl;
}
















