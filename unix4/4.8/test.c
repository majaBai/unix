#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>

int
str_find(char* str, char *c){
    int i = 0;
    int c_len = strlen(c);
    while(i < strlen(str)) {
        int end = (i + c_len - 1) > (strlen(str) - 1) ? strlen(str) - 1 : i + c_len - 1;
        char* sb_s = str_sub(str, i, end );
        if(strcmp(sb_s, c) == 0){
            return i;
        }
        i += 1;
    }
    return -1;
}

char*
str_sub(char* str, int start, int end){
    int len = end - start + 1;
    char *res = malloc(sizeof(char) * (len + 1));
    for(int i = 0; i < len; i++) {
        res[i] = str[start +  i];
    }
    res[len] = '\0';
    return res;
}

char**
str_split(char* str, char* del) {
    char** res = malloc(sizeof(char *) * 64); // 假设结果最多分割64个字符串
    int num = 0;
    int d_len = strlen(del);
    while(strlen(str) > 0) {
        int idx = str_find(str, del);
        if(idx == -1) {
            res[num] = str;
            return res;
        } else if (idx == 0) {
            // 开头
            char *sb_s = str_sub(str, d_len, strlen(str) - 1);
            res[num] = str;
            return res;
        } else if (idx == strlen(str) - 1) {
            // 结尾
            char *sb_s = str_sub(str, 0, strlen(str) - d_len - 1);
            res[num] = str;
            return res;
        } else {
            // 中间
            char *sb_s = str_sub(str, 0, idx -1);
            res[num] = sb_s;
            num +=1;
            str = str_sub(str, idx + d_len, strlen(str) - 1);
        }
    }
    return res;
}
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

        // printf("尝试路径：%s\n", full_path);
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

char*
str_trim(char *str) {
    int len = strlen(str);
    char *res = malloc(sizeof(char) * (len + 1));
    // 找到开头的空白停止位置
    int i = 0;
    while( i < len) {
        if(str[i] != ' ') {
            break;
        }
        i++;
    }
    if(i == len) {
        // 说明全是空格
        char *emp = "";
        res = emp;
        return res;
    }
    // 找到结尾的空白停止位置
    int j = len - 1;
    while(j >= 0) {
        if(str[j] != ' '){
            break;
        }
        j--;
    }
   
    char *sub = str_sub(str, i, j);
    strcpy(res, sub);
    return res;
}

int
str_find(char* str, char *c){
    int i = 0;
    int c_len = strlen(c);
    while(i < strlen(str)) {
        int end = (i + c_len - 1) > (strlen(str) - 1) ? strlen(str) - 1 : i + c_len - 1;
        char* sb_s = str_sub(str, i, end );
        if(strcmp(sb_s, c) == 0){
            return i;
        }
        i += 1;
    }
    return -1;
}



int main(int argc, const char **argv,const char **envp){
    char *str = "abcd>123>maja";
    char *del = ">";
    char** res = str_split(str, del);
    int i = 0;
    while(res[i] != NULL){
        printf(" %s \n", res[i]);
        i++;
    }
    return -1;
}