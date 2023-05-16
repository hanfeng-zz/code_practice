//
// Created by zheng zhao on 2023/4/14.
//

#include <iostream>
#include <vector>

int main() {
    std::vector<int> arr;
    arr.resize(sizeof(int) * 20);
    std::cout <<  arr.max_size() << "  " << arr.capacity()<<  std::endl;
    int i;
    for ( i = 0; i < 15; i++) {
        arr.push_back(i);
    }


    for (int & it : arr) {
        std::cout << it ;
    }

    std::cout << std::endl;
    arr.erase(arr.begin());

    for (int & it : arr) {
        std::cout << it ;
    }

    std::cout << std::endl;
    std::cout << *arr.begin() << " "<< arr.max_size() << "  " << arr.capacity()<<  std::endl;
}