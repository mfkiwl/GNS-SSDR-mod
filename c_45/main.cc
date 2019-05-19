
//#include "c45_tree.h"
#include "dataSet.h"
#include <iostream>


int main(int argc, char * argv[]) {

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

    std::vector<std::string> testFeats {
                                        "min-dop",
                                        "av-dop",
                                        "var-dop", // sqrt
                                        "min-n.valid-sat",
                                        "av-valid-sat" 
                                    };

    DataSet ds0("csv/cs-ds1-2-3-4-7-balanced_train.csv", testFeats, classes);


    std::pair<std::vector<Record>, std::vector<Record>> lr = ds0.splitRecords("av-dop", 3000);
    std::vector<Record> left = lr.first;
    DataSet ds(left, classes);

    std::cout << "//////////////////////////////////////////////" << std::endl;
    std::cout << "DS" << std::endl;

    

    std::cout << std::endl << "features: ";
    std::vector<std::string> fts = ds.getFeatureNames();
    std::vector<std::string> cls = ds.getLabels();

    for (auto it = fts.begin(); it != fts.end(); ++it) {
        std::cout << *it << "  ";
    }

    std::cout << std::endl;

    std::cout << std::endl << "classes: ";

    for (auto it = cls.begin(); it != cls.end(); ++it) {
        std::cout << *it << "  ";
    }


    std::cout << std::endl;

    std::cout << std::endl << "done" << std::endl << std::endl;
}
