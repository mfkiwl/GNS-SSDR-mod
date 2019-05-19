#ifndef DATASET_H
#define DATASET_H

#include "record.h"
#include <vector>
#include <map>
#include <utility>

class DataSet {

public:

    friend class C45_tree;
    friend class C45_node;

    DataSet();

    DataSet(std::string csvName, 
            std::vector<std::string> selectedFeatures,
            std::vector<std::string> classes);

    DataSet(std::vector<Record> records, 
            std::vector<std::string> classes);

    int getSize();
    std::vector<std::string> getLabels();
    std::vector<std::string> getFeatureNames();
    std::vector<Record> getRecords();

    void readCsv(std::string csvName);
    std::pair<std::vector<Record>, std::vector<Record>> splitRecords(std::string feature, double value);
    std::vector<Record> sortRecordsByFeature(std::string feat);
    void removeFeature(std::string feat);
    void clear();
    
    void printRecords();
    void printRecords(int nRecords);

    bool allSameClass();
    std::string majorityClass();
    double entropy();

private:

    std::vector<std::string> labels;
    std::vector<std::string> featureNames;
    std::vector<Record> records;

    void adjustStandardDeviations();
};



#endif
