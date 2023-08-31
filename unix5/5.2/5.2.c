/*
作业目录
unix/5/2.c.c
unix/5/2.p.c



shell 在 fork 后应该暂停
等子进程执行完毕后，再继续运行

可以用下面的 wait 调用来实现
这个调用会 block （阻塞）直到子进程结束
其中 status 是子进程返回的状态码

int status
wait(&status);


实现一个 child.c
它的内容是在无限循环里 printf 和 sleep(1)
这样方便你监视 child 的情况

实现一个 parent.c
它 fork 后，在父进程 wait，在子进程 execve child

开新终端，在新终端里 kill child       kill -2 [PID]
你会发现父进程的 wait 返回了

用 kill -SIGTSTP 可以将进程挂起
但是你发现这时候父进程并未返回
说明 wait 只在程序退出的时候返回


现代的 正常的 shell 中都可以用 Ctrl-z 发送 SIGTSTP 来挂起进程
如果要实现现代 shell
wait 就没用了

所以就出了个补丁版 waitpid

然后你还能 man 2 wait 看到 wait3 wait4

这就是设计的演化
*/