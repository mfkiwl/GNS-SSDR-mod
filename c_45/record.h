#ifndef RECORD_H
#define RECORD_H

#include <map>
#include <vector>
#include <iostream>

class Record {

public:

    Record() {}
    Record(std::map< std::string, double > features, std::string label);

    long getId();
    void removeFeature(std::string feat);
    std::map< std::string, double > getFeatures();
    int getSize();
    std::string getLabel(); 

    void printRecord();

private:

    friend class DatSet;
    friend class C45_tree;
    friend class C45_node;

    long id;
    std::map< std::string, double > features;
    std::string label;
};


#endif
