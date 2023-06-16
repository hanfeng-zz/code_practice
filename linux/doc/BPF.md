[toc]

# Linux Socket Filtering aka Berkeley Packet Filter (BPF)

## basic element
     A     32BIT wide accumulator
     X     32BIT wide X register
     M[]   scratch memory store

## instruction
![BPF_INSTRUCTION](./picture/BPF_INSTRUCTION)
![ADRESS_MODE](./picture/ADRESS_MODE)
+ 1 word = 2byte; 1byte = 8bit

## 32bit cpu bpf 
### head files
 + bpf_common.h
 + filter.h
    
### struct 
>struct sock_filter {	/* Filter block */      
    __u16	code;   /* Actual filter code */        
    __u8	jt;	/* Jump true */     
    __u8	jf;	/* Jump false */        
    __u32	k;      /* Generic multiuse field */        
};
> 
+ jt:指令成功后跳转到下一条指令位置
+ jf:指令失败后跳转到下一条指令位置
+ k:偏移地址、数据等等
### example 
>
> static struct sock_filter filter_virtual_nc[RAW_FILTER] = {       
    { OP_LDH, 0, 0, OFF_ETYPE },        
    { OP_JEQ, 1, 0, ETH_P_1588 },       
    { OP_JEQ, 0, 1, ETH_P_SLOW },       
    { OP_RETK, 0, 0, 0x00040000 },      
    { OP_RETK, 0, 0, 0x00000000 },      
};      
> 

+ OP_LDH 是(BPF_LD|BPF_H|BPF_ABS)三条指令集合，表示数据包起始地址偏移OFF_ETYPE(12byte)后，加载2byte数据以绝对值的方式到累加器中，无论jump true 
还是jump false都继续执行下一条指令
+ OP_JEQ是(BPF_JMP|BPF_JEQ|BPF_K)三条指令集合，比较累加器中的数据是否和ETH_P_1588数据类型匹配，
匹配成功执行jt，跳过一条命令执行指令；匹配失败执行jf,跳过零条命令执行指令
+ OP_JEQ是(BPF_JMP|BPF_JEQ|BPF_K)三条指令集合，比较累加器中的数据是否和ETH_P_SLOW数据类型匹配，
  匹配成功执行jt，跳过零条命令执行指令；匹配失败执行jf,跳过一条命令执行指令
## reference
[freeBSD_BPF](https://man.freebsd.org/cgi/man.cgi?query=bpf&sektion=4&manpath=FreeBSD+4.7-RELEASE)
[linux_bpf](https://www.kernel.org/doc/Documentation/networking/filter.txt)     
[BPF_并发控制](https://www.sigops.org/s/conferences/hotos/2021/papers/hotos21-s08-park.pdf)     
[BPF_背景](https://blog.csdn.net/weixin_55255438/article/details/127155786?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-0-127155786-blog-124357147.235%5Ev35%5Epc_relevant_default_base3&spm=1001.2101.3001.4242.1&utm_relevant_index=3)
[代码参考](https://blog.csdn.net/bie_niu1992/article/details/94007550)      
[bpf_and_tcpdump](https://andreaskaris.github.io/blog/networking/bpf-and-tcpdump/)

# tcpdump 生成bpf c code
## example 
+ 捕获eth0网卡中的二层1588 和 esmc_ssm报文
> tcpdump -i eth0 ether proto 0x88f7 or 0x8809 -dd
+ 捕获eth0网卡中的三层udp 1588报文，端口号是319-320
> tcpdump -i eth0 udp portrange 319-320 -dd
> 
## reference
[tcpdump_quick_start](https://packetlife.net/media/library/12/tcpdump.pdf)  
[tcpdump_example_description](https://blog.wains.be/2007/2007-10-01-tcpdump-advanced-filters/)  
[tcpdump](https://www.tcpdump.org)
[ssm](https://www.renrendoc.com/paper/89969119.html)    