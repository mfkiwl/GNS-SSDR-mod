#ifndef C45_CL_TREE
#define C45_CL_TREE

#include <vector>
#include <map>
#include <string>

class C45_clNode {

public:

    C45_clNode() {}

private:

    friend class C45_clTree;

    int id;
    std::string feature;
    double value;
    std::string label;
    bool leaf;
    int left;
    int right;
};

class C45_clTree {

public:

    C45_clTree();

    // creates the tree from file
    C45_clTree(std::string fName);

    // classifies an instance encoded as feature-name to value map
    std::string classify(std::map<std::string, double> &instance);

    void printInfo();
    

private:

    C45_clNode root;
    std::map<int, C45_clNode> nodeMap;
    std::vector<std::string> features;

    void readTreeFile(std::string fName); 
    void buildTree(std::map<int, std::vector<std::string>> &treeData);   
    C45_clNode makeNode(std::vector<std::string> nodeData);
};

#endif
