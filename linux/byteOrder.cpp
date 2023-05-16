//
// Created by zheng zhao on 2023/4/11.
//

#include <iostream>


int main() {
    ///小端字节序（Little endian），数值低位存储在内存的低地址，高位存储在内存的高地址
    ///大端字节序（Big endian），数值高位存储在内存的低地址，低位存储在内存的高地址
    ///大多计算机的处理模式是小端，网络传输是大端字节序
    uint16_t endian = 0x88F7;

    printf("address 0x%x 0x%x %x %x\n",
           (uint8_t *)&endian,
           (uint8_t *)&endian + 1,
           ((uint8_t *)&endian)[0],
           ((uint8_t *)&endian)[1]); //address 0x6f892f6e 0x6f892f6f f7 88
                                    ///所以主机字节序是小端字节序

}