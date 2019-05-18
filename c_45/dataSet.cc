
#include "dataSet.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

DataSet::DataSet() {}

DataSet::DataSet(std::string csvName, std::vector<std::string> selectedFeatures) {
    readCsv(csvName, selectedFeatures);
}

DataSet::DataSet(std::vector<Record> records) 
    : records(records)
{}

int DataSet::getSize() {
    return records.size();
}

std::vector<Record> DataSet::getRecords() {
    return records;
}

void DataSet::readCsv(std::string csvName, std::vector<std::string> selectedFeatures) {

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
            if(std::find(selectedFeatures.begin(), 
                         selectedFeatures.end(), currentHeader) != selectedFeatures.end()) {

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

void DataSet::printRecords() {

    for (auto it = records.begin(); it != records.end(); ++it) {

        it->printRecord();
    }
}



















