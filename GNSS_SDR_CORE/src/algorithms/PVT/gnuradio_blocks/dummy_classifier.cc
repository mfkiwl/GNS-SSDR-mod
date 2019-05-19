
#include "dummy_classifier.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>


Classification DummyClassifier::classify(FeatureSet features) {

    // the actual classifier would run on the given features

    // As dummy return a random classification

    srand (time(NULL));
    int n = rand() % 30;
   
    Classification cl;

    switch (n) {

        case 1:
            cl = STATIC_SWITCH;
            break;

        case 2:
            cl = STATIC_OVERPOWERED_TIME_PUSH;
            break;

        case 3:
            cl = STATIC_MATCHED_POWER_TIME_PUSH;
            break;

        case 4:
            cl = STATIC_MATCHED_POWER_POSITION_PUSH;
            break;

        case 5:
            cl = POWER_MATCHED_TIME_PUSH;
            break;

        case 6:
            cl = SCER;
            break;

        default:
            cl = OK;
    }

    return cl;
}

void DummyClassifier::printClass(Classification cl) {

    if (cl != 0) {
        std::cout << "\033[1;31mWARNING:\033[0m ";
    }

    switch (cl) {

        case OK:
            std::cout << "No Spoofing detected" << std::endl;
            break;

        case STATIC_SWITCH:
            std::cout << "STATIC_SWITCH detected" << std::endl;
            break;

        case STATIC_OVERPOWERED_TIME_PUSH:
            std::cout << "STATIC_OVERPOWERED_TIME_PUSH detected" << std::endl;
            break;

        case STATIC_MATCHED_POWER_TIME_PUSH:
            std::cout << "STATIC_MATCHED_POWER_TIME_PUSH detected" << std::endl;
            break;

        case STATIC_MATCHED_POWER_POSITION_PUSH:
            std::cout << "STATIC_MATCHED_POWER_POSITION_PUSH detected" << std::endl;
            break;

        case POWER_MATCHED_TIME_PUSH:
            std::cout << "POWER_MATCHED_TIME_PUSH detected" << std::endl;
            break;

        case SCER:
            std::cout << "SCER detected" << std::endl;
            break;

        default:
            std::cout << "Classification Error" << std::endl;
    }     
}






























