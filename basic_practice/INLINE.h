//
// Created by zheng zhao on 2023/4/3.
//

#ifndef BASIC_PRACTICE_INLINE_H
#define BASIC_PRACTICE_INLINE_H


class INLINE {
    int size = 100;
public:
    [[nodiscard]] int getSize() const {
        return size;
    };

    static INLINE& shared() {
        static auto instance = INLINE();
        return instance;
    }
};


#endif //BASIC_PRACTICE_INLINE_H
