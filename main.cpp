#include <stdlib.h>
#include <time.h>
#include <stdio.h>

template<typename T>
struct binary_tree {
    binary_tree* left;
    binary_tree* right;
    T value;
    binary_tree() : left(NULL), right(NULL) {
    }
};

template <typename T, typename F>
binary_tree<T>* bt_iterate(F func, binary_tree<T>* tree) {
    for(binary_tree<T>* result = (tree->left);
        result != NULL; result = bt_iterate(func, result)) {
    }
    func(tree->value);
    return tree->right;
}

template <typename T, typename F>
void bt_iterate_traditional(F func, binary_tree<T>* tree) {
    if(tree->left != NULL) {
        bt_iterate_traditional(func, tree->left);
    }
    func(tree->value);
    if(tree->right != NULL) {
        bt_iterate_traditional(func, tree->right);
    }
}

static void functor(long) {
}

static long try_invalid_buf(long index = 0, long max_index = 10000){
    if(index != max_index){
        try_invalid_buf(index + 1, max_index);
    }
    return index;
}

const static int test_size = 1<<16;
int real_size = test_size;

int main(int argc, char* argv[]) {

    if(argc > 1){
        real_size = atoi(argv[1]);
    }

    binary_tree<long> *tree = new binary_tree<long>[real_size];
    for (long i = 0; i < real_size/2 - 2; ++i) {
        tree[i].left = tree + 2*i + 1;
        tree[i].right = tree + 2*i + 2;
        tree[i].value = i;
    }
    struct timespec tpstart;
    struct timespec tpend;

    try_invalid_buf();
    {
        clock_gettime(CLOCK_MONOTONIC, &tpstart);
        bt_iterate(functor, tree);
        bt_iterate(functor, tree);
        bt_iterate(functor, tree);
        bt_iterate(functor, tree);
        clock_gettime(CLOCK_MONOTONIC, &tpend);
        printf("use_time: %ld \n", (tpend.tv_sec-tpstart.tv_sec)*1000000000+(tpend.tv_nsec-tpstart.tv_nsec));
    }

    try_invalid_buf();
    {
        clock_gettime(CLOCK_MONOTONIC, &tpstart);
        bt_iterate_traditional(functor, tree);
        bt_iterate_traditional(functor, tree);
        bt_iterate_traditional(functor, tree);
        bt_iterate_traditional(functor, tree);
        clock_gettime(CLOCK_MONOTONIC, &tpend);
        printf("use_time: %ld \n", (tpend.tv_sec-tpstart.tv_sec)*1000000000+(tpend.tv_nsec-tpstart.tv_nsec));
    }

    delete[] tree;
    return 0;
}
