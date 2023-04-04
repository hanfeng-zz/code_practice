+ copy constructor
    + 默认是按位拷贝，有static变量的时候，尽量不要用default copy constructor ，由于对象多个的原因，存在static 变量和预期不一致的原因，尽量使用自定义的拷贝构造函数，且对static变量进行处理
    + 值传递类对象时使用拷贝构造