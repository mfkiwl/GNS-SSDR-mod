
#include "record.h"
#include <iostream>

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

void Record::printRecord() {

    std::cout << id;

    for (auto it = features.begin(); it != features.end(); ++it) {

        std::cout << "  " << it->second;
    }

    std::cout << std::endl;
}
















