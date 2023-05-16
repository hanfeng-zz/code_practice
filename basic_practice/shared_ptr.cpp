//
// Created by zheng zhao on 2023/4/12.
//
#include <iostream>

class animal {
public:
    animal() {
        printf("construct function animal\n");
    }
    virtual int getLeg() = 0;
    virtual void setLeg(int num) = 0;
private:
};
class dog;
static dog *p;
class dog : public animal {
public:
    dog(const char string[4]) {
        printf("construct function dog\n");
    }
    ~dog() {
        delete p;
        printf("delete p \n");
    }
    void setLeg(int num) {
        leg = num;
    }
    int getLeg() {
        return leg;
    }
    inline static dog &shared() {
        static auto instance = dog("dog");
        return instance;
    }

    inline static dog *po() {
        if(NULL == p) {
            p = new dog("dog");
        }
        return p;
    }
private:
    int leg = 4;
};
static dog * test;
int main() {

    std::shared_ptr<animal> ptr = std::static_pointer_cast<animal>(std::make_shared<dog>("dog"));
    ptr->setLeg(9527);

    printf(" leg 1 = %d \n",ptr->getLeg());
    std::shared_ptr<dog> dogptr = std::static_pointer_cast<dog>(ptr);

    dog &instance = dog::shared();
    printf(" leg 2 = %d \n",instance.getLeg());
    instance.setLeg(20);
    printf(" leg 3 = %d \n",instance.getLeg());

    dog &instance2 = dog::shared();
    printf(" leg 4 = %d \n",instance2.getLeg());
#if 0
    dog *instance3 = dog::po();
    std::shared_ptr<dog> ptr3 (instance3);
    printf(" leg 5 = %d \n",instance3->getLeg());
    printf(" leg 6 = %d \n",ptr3->getLeg());
    printf(" %ld\n",ptr3.use_count());
    std::shared_ptr<dog> ptr4 = ptr3;
    printf(" %ld\n",ptr3.use_count());
#endif

    std::shared_ptr<animal> ptr6 = std::static_pointer_cast<animal>(std::make_shared<dog>("dog"));

    printf("----%lx,%d\n",ptr6.use_count(),ptr6->getLeg());
    return 1;
}