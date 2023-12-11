本主题接下来会贴 2 个代码  server.c 和 client.c
本作业是 unix 下的网络程序，需要使用 linux


在接下来的后续作业中，我们要依次用多进程 多线程 线程池 进程池等技术
不断提高服务器的性能

本次作业要接入性能测试工具
用于量化性能指标

在压缩包的基础上
用下面的字符串当做响应
char *msg = "HTTP/1.1 200\r\nContent-Type: text/plain\r\nConnection: close\r\nContent-Length: 2\r\n\r\nok";


这样就可以用 ab wrk 这两个测试工具来测试服务器的性能（他们需要这样格式的响应）
你以后或许会看到有人用 ab，但我们这里只用 wrk

wrk 是系统没有收录的软件
要我们手动编译
方法如下

git clone https://gitee.com/mirrors/wrk.git
cd wrk
sudo apt-get install libssl-dev git -y
make
# 复制编译好的 wrk 到一个 PATH 可以找到的地方
sudo cp wrk /usr/local/bin



# 编译好后，用如下命令测试我们的服务器
# 以下测试在 linux 下跑

# wrk
# 以 4 个线程 200 个连接 进行持续 5s 的测试
# -t 需要模拟的线程数
# -c 需要模拟的连接数
# -d 测试的持续时间
wrk -t4 -c200 -d5 http://127.0.0.1:3000