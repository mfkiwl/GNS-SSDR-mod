#ifndef C_45_NODE_H
#define C_45_NODE_H

#include <vector>
#include <map>

class C45_node {

public:
    
    C45_node();
    friend class C45_tree;

private:

    long id;
    int splitFeature = -1;
    double splitValue = -1;
    double gain = 0;
    C45_node * parent = nullptr;
    C45_node * left = nullptr;
    C45_node * right = nullptr;
    int depth = 0;
    int classification = -1;
    std::map<long, std::vector<double>> data;
    std::map<long, std::vector<double>> leftData;
    std::map<long, std::vector<double>> rightData;

    bool isRoot();
    bool isLeaf();
    std::string toString();
};

#endif
