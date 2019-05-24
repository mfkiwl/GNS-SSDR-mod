
#include "c45_tree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <float.h>
#include <algorithm>
#include <thread>
#include <functional>



C45_tree::C45_tree(std::string csvName, 
                std::vector<std::string> featureNames, 
                std::vector<std::string> classes, 
                int minSize,
                int maxDepth,
                int threadsPerFeature) 

    :   minSize(minSize), 
        maxDepth(maxDepth), 
        threadsPerFeature(threadsPerFeature), 
        classes(classes),
        featureNames(featureNames)
{

    DataSet rootSet(csvName, featureNames, classes);
    root.data = rootSet;
    doSplit(&root);
}

void C45_tree::printInfo() {

    std::cout << "root id: " << root.id << std::endl;
    std::cout << "n. nodes: " << nodes.size() << std::endl << std::endl;
}


/////////////////////////
//  TREE CONSTRUCTION  //
/////////////////////////


void C45_tree::doSplit(C45_node * node) {

    // base cases

    if (node == nullptr) {
        std::cout << std::endl << "base case: null pointer" << std::endl;
        return;
    }

    if (node->label == "empty") {
        makeLeaf(node);
        return;
    }

    DataSet data = node->data;

    if (data.getSize() == 0 || data.getFeatureNames().size() == 0) {

        std::cout << std::endl << "base case: empty on node " << node->id << std::endl;
        node->label = "empty";
        makeLeaf(node);
        return;
    }

    if (data.getSize() <= minSize || data.allSameClass() || node->depth > maxDepth) {

        if (data.getSize() <= minSize) {
            std::cout << std::endl << "base case: minSize on  node " << node->id << std::endl;
        }
        else if (data.allSameClass()) {
            std::cout << std::endl << "base case: allSame on  node " << node->id << std::endl;
        }
        else if (node->depth > maxDepth) {
            std::cout << std::endl << "base case: maxDepth on  node " << node->id << std::endl;
        }

        node->label = data.majorityClass();
        makeLeaf(node);
        return;
    }

    applyBestSplit(node);

/*
    if (node->gain == 0) {
        makeLeaf(node);
        return;
    }
*/
    // recursive case

    nodes.push_back(node);

    doSplit(node->left);
    doSplit(node->right);
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

    std::string header = "";

    for (auto it = featureNames.begin(); it != featureNames.end(); ++it) {

        header += *it + ",";
    }

    header.substr(0, header.size()-1);
    
    out << header << std::endl;

    for (int i = 0; i < nodes.size(); ++i) {

        std::cout << "writing node " << nodes[i]->id << std::endl;
        out << nodes[i]->toString() << std::endl;
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

    SplitInfo * splitInfo = new SplitInfo();

    std::vector<std::string> features = ds.getFeatureNames();
    std::vector<Record> records = ds.getRecords();

    std::vector<std::thread> threads;

    for (int i = 0; i < features.size(); ++i) {

        int subsetSize = ds.getSize() / threadsPerFeature;
        int start = 0;
        int stop = 0;

        for (int j = 0; j < threadsPerFeature; ++j) {

            start = j * subsetSize;
            stop = start + subsetSize;

            if (j == threadsPerFeature) {
                stop = ds.getSize();
            }

            threads.push_back(  std::thread( &C45_tree::findSplitValue, this,
                                std::ref(ds),
                                std::ref(features[i]),
                                splitInfo,
                                start,
                                stop) );    
        }
    }

    std::cout << "running " << threads.size() << " threads" << std::endl;

    for (int i = 0; i < threads.size(); ++i) {

        threads[i].join();
    }

    node->majority = node->data.majorityClass();
    node->data.clear();

    std::string bestFeature = splitInfo->getFeature();
    double bestValue = splitInfo->getValue();
    double bestGain = splitInfo->getGain();
    DataSet bestLeft = splitInfo->getLeft();
    DataSet bestRight = splitInfo->getRight();

    delete splitInfo;

    bestLeft.removeFeature(bestFeature);
    bestRight.removeFeature(bestFeature);

    std::cout << "split feature: " << bestFeature << std::endl;
    std::cout << "split value: " << bestValue << std::endl;
    std::cout << "split gain: " << bestGain << std::endl;
    std::cout << "left size: " << bestLeft.getSize() << std::endl;
    std::cout << "right size: " << bestRight.getSize() << std::endl;


    node->gain = bestGain;
    node->splitFeature = bestFeature;
    node->splitValue = bestValue;

    C45_node * left = new C45_node;
    left->data = bestLeft;
    left->parent = node;
    left->depth = node->depth + 1;

    C45_node * right = new C45_node;
    right->data = bestRight;
    right->parent = node;
    right->depth = node->depth + 1;

    if (bestLeft.getSize() > 0) {
        node->left = left;
    }
    else {
        left->label = "empty";
        node->left = left;
    }
    if (bestRight.getSize() > 0) {
        node->right = right;  
    }
    else {
        right->label = "empty";
        node->right = right;
    }
}

void C45_tree::findSplitValue(  DataSet &data,
                                std::string &feature, 
                                SplitInfo * splitInfo,
                                int start,
                                int stop) {

    std::vector<Record> records = data.sortRecordsByFeature(feature);

    for (long i = start; i < stop; ++i) {

        if (i >= records.size()) {
            break;
        }

        std::map<std::string, double> recFeatures = records[i].getFeatures();
        double value = recFeatures[feature];

        std::pair<std::vector<Record>, std::vector<Record>> lr = data.splitRecords(feature, value);

        DataSet left(lr.first, classes);
        DataSet right(lr.second, classes);

        double g = gain(data, left, right);

        if (i == 0) {
            value /= 2;
        }
        else {
            value = (value + records[i-1].getFeatures()[feature]) / 2;
        }

        splitInfo->update(g, feature, value, left, right);
    }
}


void C45_tree::makeLeaf(C45_node * node) {

    node->left = nullptr;
    node->right = nullptr;
    node->leaf = true;
    node->splitFeature = "_";
    node->data.clear();

    nodes.push_back(node);
}

















