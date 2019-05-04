
#include "c45_tree.h"
#include "c45_tree_builder.h"


int main(int argc, char * argv[]) {

    C45_treeBuilder treeBuilder;

    C45_tree tree = treeBuilder.buildTree("iris/iris.data", 3, 0, 0);

    std::cout << "done" << std::endl;
}
