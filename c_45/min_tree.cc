#include "min_tree.h"

int MinTree::predict(std::vector<double> sample) {

    return traverse(this, sample);
}

int MinTree::traverse(MinTree * node, std::vector<double> sample) {

    if (node->type == LEAF) {
        return node->classification;
    }
    
    if (sample[node->splitFeature] < node->splitValue) {
        return traverse(node->left, sample);
    }
    else {
        return traverse(node->right, sample);
    }
}
