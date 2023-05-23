// Copyright (c) 2023 Shifei Technologies. All rights reserved.
//
// This file is part of internal projects, unauthorized copying of this file
// via any medium is strictly prohibited.

//
// Created by zheng zhao on 2023/5/11.
//
#include <iostream>

enum struct filterType : uint8_t {
    PTP_AND_ESMC,
    VSC_LONG_PREFIX,
};

class raw  {

public:
    explicit raw(std::string name)  {};

    ~raw() = default;

    int32_t Open(std::string, int32_t *fd, filterType type) ;

    int32_t Close(int fd) ;

    int32_t Send(int fd, uint8_t *buf, uint32_t len) ;

    int32_t Receive(int fd, uint8_t *buf, uint32_t buflen) ;
private:
    int open_socket(const std::string name, filterType type);

    int32_t raw_config(int fd, filterType type, int index);
private:

};

