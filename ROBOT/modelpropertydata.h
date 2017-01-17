#ifndef MODELPROPERTYDATA_H
#define MODELPROPERTYDATA_H
typedef struct MotorStartData MStartData;
typedef struct MotorStopData MStopData;

struct MotorStartData{
    bool motorChecked[4];
    int motorPower[4];
    const int num = 4;

    void setData(MStartData data);
};
struct MotorStopData{
    bool motorChecked[4];
    const int num = 4;

    void setData(MStopData data);
};


#endif // MODELPROPERTYDATA_H
