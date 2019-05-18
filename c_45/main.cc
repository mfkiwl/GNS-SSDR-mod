
#include "c45_tree.h"
#include "dataSet.h"
#include <iostream>


int main(int argc, char * argv[]) {

    std::vector<std::string> feats  {
                                        "min-dop",
                                        "av-dop",
                                        "var-dop", // sqrt?
                                        "min-n.valid-sat",
                                        "av-valid-sat",
                                        "var-valid-sat", // sqrt?
                                        "max-n.valid-sat-changed",
                                        "var-n.valid-sat-changed", // sqrt?
                                        "min-snr",
                                        "max-snr",
                                        "snr-av",
                                        "var-snr", // sqrt?
                                        "min-ps",
                                        "av-ps", // sqrt?
                                        "var-ps", // sqrt?
                                        "max-cp",
                                    };

    DataSet ds("csv/cs-ds1-2-3-4-7-balanced_train.csv", feats);

    ds.printRecords();

    std::cout << std::endl << "done" << std::endl << std::endl;
}
