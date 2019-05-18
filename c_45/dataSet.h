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
    DataSet(std::string csvName, std::vector<std::string> selectedFeatures);
    DataSet(std::vector<Record> records);

    int getSize();
    std::vector<Record> getRecords();

    void readCsv(std::string csvName, std::vector<std::string> selectedFeatures);
    std::pair<std::vector<Record>, std::vector<Record>> splitRecords(std::string feature, double value);
    std::vector<Record> sortRecordsByFeature(std::string feat);
    
    void printRecords();

private:

    std::vector<Record> records;
};



#endif
