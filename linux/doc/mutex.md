# mutex
在C++中，upgradable mutex（可升级互斥锁）是一种特殊类型的互斥锁，允许在读取和写入操作之间进行更细粒度的控制。这种互斥锁通常用于多线程编程中，需要在读操作和写操作之间实现高效的同步。
Upgradable Mutex 的概念

Upgradable mutex 提供了一种机制，允许一个线程以读模式持有锁，并在需要写访问时升级到写模式。这种设计的目的是提高读操作的并发性，同时保证写操作的排他性。典型的互斥锁要么是读写锁（如shared_mutex），要么是独占锁（如mutex）。而upgradable mutex允许以下几种锁状态：

    独占锁（Exclusive Lock）：只有一个线程可以持有，其他线程被阻塞。
    共享锁（Shared Lock）：多个线程可以同时持有，用于读操作。
    可升级锁（Upgradable Lock）：允许一个线程以读模式持有，并在需要时升级为独占锁。

C++ 标准库中的 Upgradable Mutex

C++标准库中并未直接提供upgradable mutex，但是可以通过Boost库来实现。Boost库中的boost::shared_mutex提供了一种类似于upgradable mutex的机制。

# c++
- std::mutex：基本互斥锁，适用于独占访问。
- std::recursive_mutex：递归互斥锁，适用于递归函数。
- std::timed_mutex：带超时功能的互斥锁，适用于需要超时的场景。
- std::shared_mutex：共享互斥锁，适用于高读低写的场景。
- std::shared_timed_mutex：带超时功能的共享互斥锁。
- std::lock_guard 和 std::unique_lock：自动管理锁的类模板。
- std::call_once 和 std::once_flag：确保代码段只执行一次。

# boost
- boost::mutex：基本互斥锁，适用于独占访问。
- boost::recursive_mutex：递归互斥锁，适用于递归函数。
- boost::timed_mutex：带超时功能的互斥锁，适用于需要超时的场景。
- boost::recursive_timed_mutex：结合递归和超时功能的互斥锁。
- boost::shared_mutex：共享互斥锁，适用于高读低写的场景。
- boost::upgrade_mutex：支持升级锁的互斥锁，适用于需要从读锁升级到写锁的场景。
- boost::lock_guard 和 boost::unique_lock：自动管理锁的类模板。
- boost::call_once 和 boost::once_flag：确保代码段只执行一次。



# boost upgradable mutex 
upgradable mutex（可升级互斥锁）是一种高级锁机制，允许线程在持有读锁的情况下，在需要时升级到写锁。它的设计旨在提高多线程编程中的并发性和灵活性，特别是在读操作频繁、写操作较少的场景中。Boost库提供了这种类型的锁，称为boost::upgrade_mutex。

1. boost::upgrade_mutex 的概念

boost::upgrade_mutex 提供了三种锁定模式：

    独占锁（Exclusive Lock）：只有一个线程可以持有，其他线程被阻塞。
    共享锁（Shared Lock）：多个线程可以同时持有，用于读操作。
    可升级锁（Upgradable Lock）：允许一个线程持有共享锁，并在需要时升级为独占锁。

2. 锁定模式详解

    共享锁（Shared Lock）：多个线程可以同时读取共享资源，但不能修改。
    独占锁（Exclusive Lock）：只有一个线程可以访问共享资源，其他所有访问（读和写）都被阻塞。
    可升级锁（Upgradable Lock）：允许一个线程以共享模式访问资源，但可以在需要时升级为独占锁。

3. 使用场景

    高读低写：适用于读操作非常频繁，写操作较少的场景。
    临时写需求：线程通常执行读操作，但有时需要进行写操作，并希望在执行写操作时最小化锁的开销。
    防止写-读死锁：可升级锁可以防止在升级过程中发生写-读死锁