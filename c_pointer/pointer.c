#include <stdio.h>

double getAverage(int *arr, int size);

double getAverage(int *arr, int size) {
    int i, sum = 0;
    double avg = 0;

    for(int i = 0; i < size; i++) {
        sum += arr[i];
    }

    avg = (double) sum / size;
    return avg;
}

int main ()
{
//     int var_runoob = 10;
//     int *p;              // 定义指针变量
//     p = &var_runoob;
 
//    printf("var_runoob 变量的地址： %p\n", p);
//    printf("p 指针所指向内存地址中存储的数据： %d\n", *p);

//    p+= 1;
//    printf("指针加法：新地址： %p\n", p);

   
//    const int MAX = 3;
//    int arr[] = {10, 20, 300};

//    int *ptr[MAX];

//    for(int i = 0; i < MAX; i++) {
//     ptr[i] = &arr[i];
//    }

//    for(int i = 0; i < MAX; i++) {
//     printf("arr[%d] = %d \n", i, *ptr[i]);
//    }


// 能接受指针作为参数的函数，也能接受数组作为参数
// 指针 == 数组

int arr1[] = {200, 300, 400};
double res1 = getAverage(arr1, 3);
printf("Average value is: %f\n", res1 );

return 0;
}