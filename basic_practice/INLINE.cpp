//
// Created by zheng zhao on 2023/4/3.
//

#include "INLINE.h"
#include <iostream>
int main() {
    INLINE &instance = INLINE::shared();

    std::cout << instance.getSize() << std::endl;

    return 1;
}