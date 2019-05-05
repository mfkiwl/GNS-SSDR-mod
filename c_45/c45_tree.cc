
#include "c45_tree.h"
#include <string>

std::string C45_tree::typeToString(NodeType type) {

    switch (type) {

        case ROOT:
            return "ROOT";

        case INNER:
            return "INNER";

        case LEAF:
            return "LEAF";
    }

    return "";
}

std::string C45_tree::toString() {

    std::string res =   std::to_string(id) + "," +
                        typeToString(type) + ",";

    if (type != LEAF) {

       res += std::to_string(splitFeature) + "," + std::to_string(splitValue) + ",x,y,_";
    }
    else {
        res += "_,_," + std::to_string(classification);
    }

    return res;
}
