#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include  "anumber.h"

ANumber *
ANumber_newInt(int value){
    ANumber *res = malloc(sizeof(ANumber));

    res->valueType = ANumberValueType_int;
    res->valueInt = value;

    return res;
}

ANumber *
ANumber_newFloat(float value){
    ANumber *res = malloc(sizeof(ANumber));

    res->valueType = ANumberValueType_float;
    res->valueFloat = value;

    return res;
}

int
ANumber_intValue(ANumber *n){
    return n->valueInt;
}

float
ANumber_floatValue(ANumber *n){
    return n->valueFloat;
}

ANumberValueType
ANumber_type(ANumber *n){
    return n->valueType;
}


// static 约束变量的生命期，现在 numbertype 表示文件级作用域
static
char *numbertype[] = {
    "ANumberValueType_int",
    "ANumberValueType_float"
};

char *
ANumber_typeToString(int t) {
    printf("arg: %d, str: %s \n", t, numbertype[t]);
    return numbertype[t];
}