
#include "c45_tree.h"
#include "c45_tree_builder.h"


int main(int argc, char * argv[]) {

    C45_treeBuilder treeBuilder;

    treeBuilder.buildTree("iris/iris.data", "out.tree", 3, 5);

    std::cout << "done" << std::endl;
}
