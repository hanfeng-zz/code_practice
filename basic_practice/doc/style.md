
- const
  - 不用const int a; 使用 int const a;
    > 模板语法替换原则  
    > typedef char* CHARS;  
    > typedef CHARS const CPTR;　//指向char类型的常量指针


  -  返回值类似int数据不使用const标识
     > const int getVal();  // 返回值不可被赋值（不常见/无意义）

  - 