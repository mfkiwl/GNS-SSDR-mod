
#include "c45_tree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <float.h>
#include <algorithm>



C45_tree::C45_tree(std::string csvName, 
                std::vector<std::string> featureNames, 
                std::vector<std::string> classes, 
                int minSize) 

    : minSize(minSize), classes(classes)
{

    DataSet rootSet(csvName, featureNames, classes);
    root.data = rootSet;
    doSplit(&root);
}

void C45_tree::printInfo() {

    std::cout << "root id: " << root.id << std::endl;
    std::cout << "n. samples: " << root.data.getSize() << std::endl;
    std::cout << "n. nodes: " << nodes.size() << std::endl;
}




/////////////////////////
//  TREE CONSTRUCTION  //
/////////////////////////


void C45_tree::doSplit(C45_node * node) {

    // base cases

    DataSet data = node->data;

    if (data.getSize() < minSize) {
        makeLeaf(node->parent);
        return;
    }

    if (data.allSameClass()) {

        makeLeaf(node);
        return;
    }

    applyBestSplit(node);

    if (node->gain == 0) {
        makeLeaf(node->parent);
        return;
    }

    // recursive case

    nodes.push_back(*node);

    doSplit(node->left);
    doSplit(node->right);

    node->data.clear();
}

////////////////////
//  TREE TO FILE  //
////////////////////

void C45_tree::saveTree(std::string outName) {

    std::ofstream out(outName);

    if (! out.is_open()) {
        std::cout << "Error opening " << outName << std::endl;
        return;
    }

    std::cout << "Opened " << outName << std::endl;

    for (int i = 0; i < nodes.size(); ++i) {

        std::cout << "writing node " << nodes[i].id << std::endl;
        out << nodes[i].toString() << std::endl;
    } 

    out.close();
    std::cout << "Closed " << outName << std::endl;
}




///////////////////////////
//  AUXILIARY FUNCTIONS  //
///////////////////////////


double C45_tree::gain(  DataSet &top,
                        DataSet &left,
                        DataSet &right) {

    if (left.getSize() == 0 || right.getSize() == 0) {
        return 0;
    }

    double eTop = top.entropy();
    double eLeft = left.entropy() * left.getSize() / top.getSize();
    double eRight = right.entropy() * right.getSize() / top.getSize();

    return eTop - eLeft -eRight; 

}


void C45_tree::applyBestSplit(C45_node * node) {

    std::cout << std::endl << "splitting node " << node->id << std::endl;

    DataSet ds = node->data;

    double bestGain = -DBL_MAX;
    std::string bestFeature;
    double bestValue;
    DataSet bestLeft;
    DataSet bestRight;

    std::vector<std::string> features = ds.getFeatureNames();
    std::vector<Record> records = ds.getRecords();

    for (auto it1 = features.begin(); it1 != features.end(); ++it1) {

        std::string feat = *it1;

        for (auto it2 = records.begin(); it2 != records.end(); ++it2) {

            std::map<std::string, double> recFeatures = it2->getFeatures();

            double value = recFeatures[feat];

            std::pair<std::vector<Record>, std::vector<Record>> lr = ds.splitRecords(feat, value);

            DataSet left(lr.first, classes);
            DataSet right(lr.second, classes);

            double g = gain(ds, left, right);

            if (g > bestGain) {

                bestGain = g;
                bestFeature = feat;
                bestValue = value;
                bestLeft = left;
                bestRight = right;
            }
        }
    }

    std::cout << "split feature: " << bestFeature << std::endl;
    std::cout << "split value: " << bestValue << std::endl;
     

    bestLeft.removeFeature(bestFeature);
    bestRight.removeFeature(bestFeature);

    node->gain = bestGain;
    node->splitFeature = bestFeature;
    node->leftData = bestLeft;
    node->rightData = bestRight;
}


void C45_tree::makeLeaf(C45_node * node) {

    node->left = nullptr;
    node->right = nullptr;
    node->label = node->data.majorityClass();
    node->data.clear();
}




















