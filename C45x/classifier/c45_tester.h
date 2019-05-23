#ifndef C45_TESTER
#define C45_TESTER

#include "c45_cl_tree.h"
#include <string>
#include <vector>
#include <map>

class C45_tester {

public:

    C45_tester(std::string testFileName);
    void runTest(C45_clTree classifier);
    void printResult();

private:

    long correctClass;
    long falsePositives;
    long falseNegatives;

    std::vector<std::map<std::string, double>> instances;
    std::vector<std::string> labels;

    void readTestFile(std::string fName);
};

#endif
