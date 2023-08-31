/*
作业目录
unix/5/3.p.c


3.p.c 可以判断子进程是【挂起】还是【退出】

下面的用法第三个参数 WUNTRACED
它表示【pid 进程被 SIGSTOP 挂起也要返回】
waitpid(pid, &status, WUNTRACED);


在 waitpid 返回后
可以通过判断 status 的值来知道进程发生了什么
现在主要是用 WIFEXITED WIFSTOPPED 这两个宏来判断
if (WIFEXITED(status)) {

} else if (WIFSTOPPED(status)) {

}


这些 宏 的名字很难看
这就是 unix
*/