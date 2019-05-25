
#include "dataSet.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>

DataSet::DataSet() {}

DataSet::DataSet(   std::string csvName, 
                    std::vector<std::string> selectedFeatures,  
                    std::vector<std::string> classes) {

    labels = classes,
    featureNames = selectedFeatures;
    readCsv(csvName);
//    adjustStandardDeviations();
}

DataSet::DataSet(   std::vector<Record> records, 
                    std::vector<std::string> classes) 
    :   records(records), 
        labels(classes)
{
    if (records.size() > 0) { 

        std::map<std::string, double> feats = records[0].getFeatures();

        for (auto it = feats.begin(); it != feats.end(); ++it) {

            featureNames.push_back(it->first);
        }
    }
}

std::vector<std::string> DataSet::getLabels() {
    return labels;
}
std::vector<std::string> DataSet::getFeatureNames() {
    return featureNames;
}


int DataSet::getSize() {
    return records.size();
}

std::vector<Record> DataSet::getRecords() {
    return records;
}

void DataSet::readCsv(std::string csvName) {

    std::ifstream csv(csvName);

    if (! csv.is_open()) {
        std::cout << "Error opening " << csvName << std::endl;
        return;
    }
    else {
        std::cout << "Opened " << csvName << std::endl;
    }

    // store headers in vector
    std::string line;
    std::getline(csv, line);
    std::istringstream s(line);
    std::string field;
    std::vector<std::string> headers;

    while (std::getline(s, field, ',')) {
        headers.push_back(field);        
    }

    while (std::getline(csv, line)) {

        std::istringstream s(line);
        
        std::vector<std::string> sample;

        while (std::getline(s, field, ',')) {
            sample.push_back(field);        
        }

        std::map< std::string, double > features;

        for (int i = 0; i < sample.size() -1; ++i) {

            std::string currentHeader = headers[i];
            std::string currentFeat = sample[i];

            // check if current features is among the selected list
            if(std::find(featureNames.begin(), 
                         featureNames.end(), currentHeader) != featureNames.end()) {

                features[currentHeader] = std::stod(currentFeat);
            }
        }

        Record rec(features, sample[sample.size() -1]);
        records.push_back(rec);
    }

    csv.close();
    std::cout << "Closed " << csvName << std::endl;    
}

std::pair<std::vector<Record>, std::vector<Record>> DataSet::splitRecords(std::string feature,
                                                                         double value) {

    std::vector<Record> low;
    std::vector<Record> high;    

    for (auto it = records.begin(); it != records.end(); ++it) {

        Record rec = *it;

        if (rec.getFeatures()[feature] < value) {
            low.push_back(rec);
        }
        
        else {
            high.push_back(rec);
        }
    }

    std::pair<std::vector<Record>, std::vector<Record>> subsets;
    subsets = std::make_pair(low, high);

    return subsets;
}

std::vector<Record> DataSet::sortRecordsByFeature(std::string feat) {

    std::map<double, Record> tmpMap;
    std::vector<Record> sorted;

    // map sorts elements by key as default
    for (auto it = records.begin(); it != records.end(); ++it) {
        tmpMap[(*it).getFeatures()[feat]] = *it;
    }


    for (auto it = tmpMap.begin(); it != tmpMap.end(); ++it) {
        sorted.push_back(it->second);
    }

    return sorted;
}

void DataSet::removeFeature(std::string feat) {

    for (auto it = records.begin(); it != records.end(); ++it) {
        it->removeFeature(feat);
    }

    for (int i = 0; i < featureNames.size(); ++i) {
        if (featureNames[i] == feat) {
            featureNames.erase(featureNames.begin() + i);
        }
    }
}

void DataSet::clear() {

    records.clear();
}

/*
void DataSet::adjustStandardDeviations() {

    for (auto it = records.begin(); it != records.end(); ++it) {

        it->adjustStandardDeviations();
    }
}
*/

double DataSet::entropy() {

    long nSamples = records.size();

    if (nSamples == 0) {
        return 0;
    }

    std::map<std::string, double> labelCounts;

    for (auto it = records.begin(); it != records.end(); ++it) {
        labelCounts[it->getLabel()] += 1;
    }  

    double e = 0;

    for (auto it = labelCounts.begin(); it != labelCounts.end(); ++it) {

        double p = it->second / nSamples;
        e += p * log2(p);
    } 

    return -e;
}


void DataSet::printRecords() {

    for (auto it = records.begin(); it != records.end(); ++it) {

        it->printRecord();
    }
}

void DataSet::printRecords(int nRecords) {

    for (int i = 0; i < nRecords; ++i) {
        records[i].printRecord();
    }
}

bool DataSet::allSameClass() {

    if (records.size() > 9) {

        std::string currentLabel = records[0].getLabel();

        for (int i = 1; i < records.size(); ++i) {

            if (records[i].getLabel() != currentLabel) {
                return false;
            }
        }
    }

    return true;
}

std::string DataSet::majorityClass() {

    std::map<std::string, long> recordsPerClass;

    for (auto it = records.begin(); it != records.end(); ++it) {

        ++recordsPerClass[it->getLabel()];
    }

    long maxCount = -1;
    std::string majority;

    for (auto it = recordsPerClass.begin(); it != recordsPerClass.end(); ++it) {

        long count = it->second;

        if (count > maxCount) {

            maxCount = count;
            majority = it->first;
        }
    }

    return majority;
}

















