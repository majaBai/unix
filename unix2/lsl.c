/*
作业 2.2
 实现一个 lsl.c 程序
 给上面的 ls 程序增加一个可选参数 -l
 只能下面这样使用（-l 必须在最后）
 ls -l
 ls / -l

 -l 参数的输出格式如下，看不懂英语就群里问
 file mode, number of links, owner name, group name, number of
 bytes in the file, last modified time, the pathname

 -rw-r--r--    1 gua  groupname       360 1590569121 a.txt
 drwxr-xr-x   14 gua  groupname       448 1590508972 axe

*/
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/stat.h>
#include<pwd.h>
#include <grp.h>
#include<time.h>
#include<sys/types.h>
#include <errno.h>

#include <dirent.h>
#include <stdbool.h>

struct tm *localtime(const time_t *timer);

int
myLs(char *path){
    int numberOfFiles = 0;
    // if(strlen(path) <= 0){
    //     // 默认当前目录
    //    path = getcwd(NULL, 0);
    // } 
    DIR *dir = opendir(path);
    printf("open dir %p\n", dir);
    
    while(true) {
        struct dirent *dirData = readdir(dir);
        if(dirData != NULL) {
            if (dirData->d_type == DT_DIR) {
                printf("dir: ");
            } else {
                printf("file: ");
            }
            numberOfFiles++;
            printf("name：(%s) \n", dirData->d_name);
        } else {
            break;
        }
    }
    printf("files：%d\n", numberOfFiles);

    // 关闭目录
    closedir(dir);
    return 0;
}

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

// stat（file, buf）获取的文件信息存到 buf 中。执行成功返回0，否则-1
// 获取的字段包括：
// mode_t st_mode;   /*file tpye &mode (permissions)*/
// ino_t st_ino;     /*i=node number (serial number)*/ 
// dev_t st_rdev;   /*device number for special files*/
// nlink_t st_nlink; /*number of links*/
// uid_t    st_uid; /*user id of owner*/
// gid_t    st_gid; /*group ID of owner*/
// off_t   st_size; /*size in bytes for regular files*/
// time_t st_atime; /*time of last access*/
// time_t st_mtime; /*time of last modification*/
// time_t st_ctime; /*time of last file status change*/
// long st_blksize; /*best I/O block size */
// long st_blocks; /*number of 512-byte blocks allocated*/
int
lsl(char *path){
    // printf("path: %s \n", path);
    DIR *dir = opendir(path);
    char *d = "/";
    
    while(true) {
        struct dirent *dirData = readdir(dir);
        if(dirData != NULL) {
            char *f_name = dirData->d_name;
            char *r1 = ".";
            char *r2 = "..";
            if(strcmp(f_name, r1) == 0 | strcmp(f_name, r2) == 0){
                continue;
            }
            struct stat info;
            // 文件的完整路径 = path + '/' + 文件名
            char *p = stringCat(path, d);
            p = stringCat(p, f_name);
            int res = stat(p, &info);
            if(res !=0){
                perror("failed");
            } else {
                // file mode 权限
                char *m_str = malloc(sizeof(char) * 11);
                m_str[0] = (S_ISDIR(info.st_mode)) ? 'd' : '-';
                m_str[1] = (info.st_mode & S_IRUSR) ? 'r' : '-';
                m_str[2] = (info.st_mode & S_IWUSR) ? 'w' : '-';
                m_str[3] = (info.st_mode & S_IXUSR) ? 'x' : '-';
                m_str[4] = (info.st_mode & S_IRGRP) ? 'r' : '-';
                m_str[5] = (info.st_mode & S_IWGRP) ? 'w' : '-';
                m_str[6] = (info.st_mode & S_IXGRP) ? 'x' : '-';
                m_str[7] = (info.st_mode & S_IROTH) ? 'r' : '-';
                m_str[8] = (info.st_mode & S_IWOTH) ? 'w' : '-';
                m_str[9] = (info.st_mode & S_IXOTH) ? 'x' : '-';
                m_str[10] = '\0';
                printf("%s\t", m_str);

                printf("%ld\t", info.st_nlink); // number of links
                // owner name
                struct passwd *user;
                user = getpwuid(info.st_uid);
                printf("%s\t", user->pw_name);

                // group name
                struct group *gdata;
                gdata = getgrgid(info.st_gid);
                printf("%s\t", gdata->gr_name);


                // number of bytes in the file
                printf("%ld\t", info.st_size);

                // last modified time
                // printf("  %s", ctime(info.st_mtime));
                time_t m_time = info.st_mtime;
                // 转换为可读时间字符串
                char time_str[100];
                char d_time[20] = {""};
                strftime(time_str, 100, "%m月 %d %H:%M", localtime(&m_time));
                strncpy(d_time, time_str + 1, strlen(time_str) - 1);
                printf("%s\t", d_time);
                // struct tm *t = localtime(info.st_mtime);
                // printf("  %d月 %2d %2d:%2d", t->tm_mon + 1, t->tm_mday + 1, t->tm_hour, t->tm_min);

                //文件名
                printf("%s\t\n", dirData->d_name);
            }
        } else {
            break;
        }
    }
    closedir(dir);
    return 0;
}
int
main(int argc, char **argv) {
//    printf("有 %d 个参数 \n", argc);
//    printf("第1个参数  %s \n", argv[0]);
//    printf("第2个参数  %s \n", argv[1]);
   char *path = getcwd(NULL, 0); // 默认当前目录
    // char *path = "/home/maja/unix_w/unix2";
    lsl(path);
    free(path);
    return 0;
}