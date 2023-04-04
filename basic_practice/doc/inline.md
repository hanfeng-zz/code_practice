# inline

+ c++ 中用inline函数替代macro
+ 一般放在头文件中，函数体尽量简短；在头文件中，内联函数默认未内部链接，即static，只能被include的编译单元看见
+ 内联函数体不要有循环