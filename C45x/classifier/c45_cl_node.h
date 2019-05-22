#ifndef C45_CL_NODE
#define C45_CL_NODE

#include<string>

class C45_clNode {

public:

    C45_clNode();
    C45_clNode(std::string feature, double value);
    C45_clNode(std::string label);
    bool isLeaf();

private:

    friend class C45_clTree;

    std::string feature;
    double value;
    std::string label;
    bool leaf;
    C45_clNode * left = nullptr;
    C45_clNode * right = nullptr;
};

#endif
