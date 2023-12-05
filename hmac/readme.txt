HMAC 是一个利用现有的摘要算法（md5 sha1 sha512 等）来生成 MAC 的算法
MAC（Message Authentication Code 消息认证码）
In cryptography, a message authentication code, sometimes known as a tag
is a short piece of information used to authenticate a message—in other words
to confirm that the message came from the stated sender and has not been changed.
简单来说，就是用来确保信息完整性的，公式如下（不是让你看的，让你复制给别人看的）
HMAC（K，M）= H（K⊕opad∣H（K⊕ipad∣M））



那么你应该能想到除了 HMAC 还有别的 MAC 生成方式
我们这里只讨论 HMAC



现在，假设服务器要发一段数据给客户端，比如要存一个 cookie 在客户端
cookie = 'user=1'
hash = md5(cookie)  # 40eff00708678792a313160801ae9e1c
我们可以发送如下格式的数据给客户端
user=1#40eff00708678792a313160801ae9e1c

当客户端把这个数据发回给服务器的时候
我们收到了这样的字符串 user=1#40eff00708678792a313160801ae9e1c
我们计算一下 md5(cookie) 是否等于 40eff00708678792a313160801ae9e1c

但这样普通的 md5(cookie) 是不能保证完整性的
因为完全可以把 user=1 改成 user=2 然后重新计算 md5
比如发送如下数据仍然能通过我们的验证
user=2#7a60acc139b323a3c7915c4376e9bc95


---
我们现在想到了，必须在服务器有个密码是客户端看不到的
用这个密码参与计算 md5 才能保证安全
比如下面的方案，直接用一个随机字符串作为 salt
其实这个 salt 也就相当于一个密码了
salt = random_string()
cookie = 'user=1'
hash = md5(cookie + salt)  # 92a313160801ae9e1c40eff007086787


当客户端把这个数据发回给服务器的时候
我们加上存好的 salt 然后计算，就能确保数据完整性

但是 md5(cookie + salt) 这样简单的方式是有安全漏洞的
具体可见 Length Extension Attack（别看，知道有漏洞就行，装逼的时候再搜就是了）
---


我们当然可以把加盐的方式弄得复杂点解决安全漏洞
但是最终大家制作了一个标准方式，也就是接下来要实现的 HMAC 算法

在 HMAC 算法中，salt 被称之为 key

这个函数的最简单形式有 2 个参数
key 是用于签名的字符串，服务器持有
message 是要签名的数据
hmac(key, message)


hmac 利用 md5 sha1 等现成的摘要算法来实现 mac 功能
以 sha1 为例


每种摘要算法都是以定长的一段数据块作为基本的处理单元
称之为 block_size
具体怎么求，自己去翻或者躺

key 的长度如果小于 block_size 则在 key 后面补 0
key 的长度如果大于 block_size 则 key = sha1(key).digest()
以上 2 条 key 的处理规则都是规定

为了安全，hmac 规定了下面 2 个变量
outer_pad 是用 0x5c 这个数字 xor 处理 key 的每个字节
inner_pad 是用 0x36 这个数字 xor 处理 key 的每个字节
5c 36 是算法的作者规定死的数字，其实只要这两个数字不一样即可，但人家就这么规定了

hmac 的计算公式是 sha1(outer_pad + sha1(inner_pad + message))


注意：
一个字节的取值范围是 0-255
用十六进制表示是 0x00 - 0xFF
所以下面这样的 sha1 结果
f707095b6969cf1ce0bdf76d16758c27c9f3eb36
实际上是如下的数字序列
0xF7, 0x07, ...

这个表示方法叫做 hexstring（我瞎取的）


我会在群文件上传我是如何做这个 hmac 作业的演示视频
文件名是 hmac.7z
就是看了都说好的那个

解压密码如下
在原版 hmac 的计算公式上增加了一个 b'gua' 如下
sha1(b'gua' + outer_pad + sha1(inner_pad + message))
key = b'a'
msg = b'a'

这个计算的 hexstring 就是视频的解压密码