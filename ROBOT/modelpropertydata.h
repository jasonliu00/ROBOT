#ifndef MODELPROPERTYDATA_H
#define MODELPROPERTYDATA_H

typedef struct MotorStartData{
    bool motorChecked[4];
    int motorPower[4];
    const int num = 4;
}MSData;

#endif // MODELPROPERTYDATA_H
