//
// Created by zheng zhao on 2023/4/12.
//
#include <iostream>

class testA {
public:
    virtual int open(int &df) = 0;

};

class testB : public testA {
public:
    int raw_open(int &df) {
        df = 9527;
        return 0;
    }
};

int main() {
    std::shared_ptr<testA> pt = std::make_shared<testB>();
    int a =0;
    pt->open(a);
    printf("a = %d \n",a);
}