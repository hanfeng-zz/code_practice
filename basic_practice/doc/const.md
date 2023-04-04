
# const 

+ const 参数 : const 限定参数优先于在参数表里用const限定参数,如
>void f2 (int ic) { 
>   const int &i = ic;
>   ....
> }
> ***
> 在函数里， c o n s t有 这 样 的 意 义 : 参 数 不 能 被 改 变 。 所 以 它 其 实 是 函 数 创 建 者 的 工 具 ， 而 不 是函数调用者的工具
+ const 返回值 
  + 常量 ： 无意义;返回变量a, 变量a会被制成副本，处之不会被修改. 
  + 对象 ： 有意义；不让接收者修改对象的内容.
  + 传递或者返回地址 ： 有意义；阻止用户通过指针修改指针内容；传递值优先使用const &方式


+ 类里的const
> class integer {
>   const int size; 
> explicit integer(int x):size(x) {};}
>***
>注意每个integer对象其size都是不一样的
+ 类里的const 和enum
> class bob {  
>  const int size = 100;  
>  int array[size];  
> }  
> &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;
> size存在于bob类中，只在类对象中进行存储空间分配，所以编译器不知道const内容是什么，即不能作为编译期间的常量;
> ***
> 如何定义常数表达式里的类常量?  
> class bob {  
> enum { size = 100 };  
> int array[size]; }
>
> &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;
> 在类里的const意思是:在这个特定对象的寿命期内，而不是对于整个类来说,这个值是不变的
***
+ const对象和成员函数 ： const 对象只能调用const成员函数
