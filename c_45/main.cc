
#include "c45_tree.h"
#include "dataSet.h"
#include <iostream>


int main(int argc, char * argv[]) {

    std::string csvName = "csv/cs-ds1-2-3-4-7-balanced_test.csv";

    std::vector<std::string> classes {"clean", "spoofed"};

    std::vector<std::string> feats  {
                                        "min-dop",
                                        "av-dop",
                                        "var-dop", // sqrt
                                        "min-n.valid-sat",
                                        "av-valid-sat",
                                        "var-valid-sat", // sqrt
                                        "max-n.valid-sat-changed",
                                        "var-n.valid-sat-changed", // sqrt
                                        "min-snr",
                                        "max-snr",
                                        "snr-av",
                                        "var-snr", // sqrt
                                        "min-ps",
                                        "av-ps", // sqrt
                                        "var-ps", // sqrt
                                        "max-cp",
                                    };


    C45_tree tree(csvName, feats, classes, 5);


    std::cout << std::endl;
    tree.printInfo();
    tree.saveTree("out.tree");

    std::cout << std::endl << "done" << std::endl << std::endl;
}
