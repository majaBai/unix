// enum 用于自定义类型
// ANumberValueType 是一个只有 2 个值的类型
typedef enum {
    ANumberValueType_int,
    ANumberValueType_float,
} ANumberValueType;


typedef struct _ANumber {
    ANumberValueType valueType;
    int valueInt;
    float valueFloat;
} ANumber;


ANumber *
ANumber_newInt(int value);

ANumber *
ANumber_newFloat(float value);

int
ANumber_intValue(ANumber *n);

float
ANumber_floatValue(ANumber *n);

ANumberValueType
ANumber_type(ANumber *n);

char *
ANumber_typeToString(int t);
