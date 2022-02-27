#include "util.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief 
 * 
 * @param condition 如果有错误发生，就会调用哦个perror，同时还会打印我们传出的字符串;
 * @param errmsg 让程序退出并返回一个特定的常量exit_failure;
 */
void errif(bool condition, const char * errmsg){
    if(condition){
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}