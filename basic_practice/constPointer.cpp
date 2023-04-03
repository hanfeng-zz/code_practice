//
// Created by zheng zhao on 2023/4/3.
//
#include <iostream>
///const pointer
int main() {
    int a = 10;
    int b = 20;
    const int *p = &a;///p是一个指向const int 的指针
    ///改变*p的值有两种方式
    ///方式1:指针p指向的对象a的地址，a的值不能被*p改变，可以通过直接修改变量a，改变*p
    a = 30;
    std::cout << "way1 " << *p << std::endl;
    ///方式2:修改指针p指向的对象
    p = &b;
    std::cout << "way2 " << *p << std::endl;

    int * const p1 = &a;///p1是指向int的cosnt 指针
    ///改变*p的值有两种方式
    a = 40;
    std::cout << "way3 " << *p1 << " a " << a << std::endl;
    *p1 = 50;
    std::cout << "way4 " << *p1 << " a " << a << std::endl;

    const int * const p2 = &a;///无论对象还是指针都无法改变
    return 1;
}