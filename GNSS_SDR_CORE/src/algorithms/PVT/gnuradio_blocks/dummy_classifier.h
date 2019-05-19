#ifndef DUMMY_CLASSIFIER_H
#define DUMMY_CLASSIFIER_H

#include "classification.h"
#include "feature_set.h"
#include <map>


class DummyClassifier {

public:

    Classification classify(FeatureSet features);

    void printClass(Classification cl);
};

#endif
