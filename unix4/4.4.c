/*
我们平常在 shell 里面输入命令，是不需要输入【绝对路径】的

比如我们可以 date 来调用 /bin/date

这是怎么实现的呢？

这是通过环境变量的 PATH 实现的
通常格式如下
PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

这个 PATH 表示了用冒号分隔的几个路径
当用户在 shell 里面执行 date 的时候
会一个个去测试下面的文件是否存在，存在就执行
/usr/local/bin/date
/usr/bin/date
/bin/date
/usr/sbin/date
/sbin/date


实现一个函数，可以根据环境变量中的 PATH 来执行命令

int exe1(const char *cmd, char *const envp[]);
cmd 是一个 "date" 这样的命令

man 2 access 可以看到检查文件是否存在的文档
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/stat.h>
#include<pwd.h>
#include <grp.h>
#include<sys/types.h>

#include <dirent.h>
#include <stdbool.h>

char *
stringCat(char *s1, char *s2) {
// 求出 s1 s2 的长度
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    // 拼接后的新字符串 s3 的长度要多 1 个用于放末尾的 '\0'
    int len3 = len1 + len2 + 1;
    char *s3 = malloc(sizeof(char) * len3);

    // 循环把 s1 里的字符都复制到 s3 中
    for (int i = 0; i < len1; i++) {
        *(s3 + i) = *(s1 + i);
        // 也可以写为 s3[i] = s1[i]
    }

    // 循环把 s1 里的字符都复制到 s3 后面
    for (int i = 0; i < len2; i++) {
        *(s3 + i + len1) = *(s2 + i);
        // 也可以写为 s3[i+len1] = s2[i]
    }

    // 最后一位要设置为 '\0' 才是一个合格的字符串
    *(s3 + len3 - 1) = '\0';
    // 也可以写为 s3[len3-1] = '\0'

    return s3;
}
/*
access(const char *path, int mode)
mode 可用以下代替：
R_OK：检查读权限
W_OK：检查写权限
X_OK：检查执行权限
F_OK：检查文件或目录是否存在

如果访问权限检查成功，则返回0
如果指定的文件或目录不存在或没有足够的权限，则返回-1，并设置全局变量errno表示具体的错误原因
*/
int
exe1(const char *cmd, char *const envp[]){
    char *path_env = getenv("PATH");
    char path[1024];
    char *dir;
    if (path_env == NULL) {
        printf("Environment variable PATH not found.\n");
        return -1;
    }
    // strtok 会改变原path, 必须将 path_env 复制到缓冲区，
    // 即strtok会改变path,而不是path_env
    strncpy(path, path_env, sizeof(path));
    dir = strtok(path, ":");
    while (dir != NULL) {
        char full_path[1024];
        // snprintf(buf, size, format, arg1, arg2,....)
        // 将可变参数 agr1, arg2... 按照format格式写入 buf, 限制了长度为 size, 避免buf溢出
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);

        printf("尝试路径：%s\n", full_path);
        if(access(full_path, X_OK) == 0) {
            // 找到了文件且有执行权限
            printf("****找到指令 %s***：%s\n",cmd,full_path);
            execve(full_path, (char *const[]){ (char *)cmd, NULL}, envp);
            perror("execve");
            break;
        }
        dir = strtok(NULL, ":");
    }
    printf("****未找到指令**: %s\n", cmd);
    return -1;
}

int
main(int argc, char **argv, const char **envp){
    const char *cmd = argv[1];
    // 强制更换类型
    char *const *env = (char *const *)envp;
    exe1(cmd, env);
}