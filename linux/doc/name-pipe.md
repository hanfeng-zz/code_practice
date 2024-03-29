# 背景
    进程间大量数据交互，保证其效率高且稳定，风险可控；
    - socket：进行大量数据交换时，跟随数据增加，性能越低
    - shard memory：内存交换虽然速度最快，但是需要额外增加校验代码，保证数据不会被冲，风险高
    - name-pipe: 即使进行大量数据交换，性能低于shard memory，高于socket，且不需要增加额外校验代码，风险低

# name-pipe 
    - 又名FIFO，特殊管道，存放在文件系统中，不会像管道存放在内核中。对命名管道使用结束后，其依旧存在于文件系统中，只有进行删除操作才会消失
    - 可以用于进程间通信，不限于同源的两个进程
  
## 创建命名管道（FIFO）

- 命令创建  
    - mkfifo [-m mode ] pathname #默认可读可写    
    - mknod pathname p
  
- 程序创建  
  <mark> mkfifoat 未研究</mark>

  ``````C   

        #include <sys/types.h>
        #include <sys/stat.h>

        int mkfifo(const char *pathname, mode_t mode);//mode 为0777
        int mknod(char *pathname, mode_t mode, dev_t dev);

        #include <fcntl.h>           /* Definition of AT_* constants */
        #include <sys/stat.h>

        int mkfifoat(int dirfd, const char *pathname, mode_t mode);

  ``````

## 能力描述
- 没有字节流概念（需要注意读取的起始位置是否符合需求）
- 设备上PIPE可使用的内存大小查询
  
  ``````C 

  # ls -l /proc/sys/fs/ | grep pipe
    -rw-r--r--    1 root     root             0 Nov 24 17:08 pipe-max-size
    -rw-r--r--    1 root     root             0 Nov 24 17:08 pipe-user-pages-hard
    -rw-r--r--    1 root     root             0 Nov 24 17:08 pipe-user-pages-soft

  ``````

- 多进程有名管道打开方式:<mark>使用非阻塞模式</mark>
  ``````C

  1.创建FIFO * 2，别名FIFO1，FIFO2，两个进程对该FIFOx仅读或者写
  //进程1
  2. 非阻塞只读打开FIFO1
  open(FIFO1, O_RDONLY | O_NONBLOCK);
  5. 收到连接标志，只写打开FIFO2;
  open(FIFO1, O_WRONLY | O_NONBLOCK);
  //进程2
  3. 非阻塞只写打开FIFO1，只读打开FIFO2
  open(FIFO1, O_WRONLY | O_NONBLOCK);
  open(FIFO2, O_RDONLY | O_NONBLOCK);
  4. 发送已连接标志，使用write写FIFO1
  6.创建FIFO3来使进程1知晓进程2存活，write方式，失败会产生EPIPE和SIGPIPE信号量  
  ``````  

- 管道长度获取和设置

  ``````C   

  //管道长度可通过fcntl来查询
  fcntl(fifo_fd, F_GETPIPE_SZ); //查询
  F_SETPIPE_SZ //设置

  ``````

- 管道相关问题及解决
  - 读取失败：EAGAIN    
    无需处理，等待下一次数据
  - 写失败：EPIPE   
    EPIPE表示读取端已经断开，写入端忽略EPIPE错误，同时需要处理SIGPIPE信号量（忽略），不然进程退出
  - 类似POLL方式，写端FD关闭，会提示POLLHUP    
    读端重新打开该FD
  - 写端断开，不需要重新打开，等待读端重新连接即可

- 其它
  - SIGPIPE 也会由socket send产生，可以设置SO_NoSIGPIPE（BSD）或者MSG_NOSIGNAL（Linux）清除
  - SIGPIPE 抑制可以将FIFO设置为O_RDWR，但是这个不能用于进程间通讯，只能人工忽略处理SIGPIPE信号量
  - EPIPE 仅由write失败产生
  - 使用pthread_sigmask 处理write失败产生的SIGPIPE
  

# 参考

[mkfifo_1](https://man7.org/linux/man-pages/man1/mkfifo.1.html)     
[mkfifo_3](https://man7.org/linux/man-pages/man3/mkfifo.3.html)     
[fctnl](https://man7.org/linux/man-pages/man2/fcntl.2.html)   
[fifo](https://man7.org/linux/man-pages/man7/fifo.7.html)   
[重要](https://riptutorial.com/posix/example/17424/handle-sigpipe-generated-by-write---in-a-thread-safe-manner)

[1](https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_sigmask.html)    
[2](https://stackoverflow.com/questions/108183/how-to-prevent-sigpipes-or-handle-them-properly)