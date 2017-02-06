#include "modelpropertydata.h"

void MotorStartData::setData(MStartData data)
{
    for(int i = 0; i < num; i++){
        motorChecked[i] = data.motorChecked[i];
        motorPower[i] = data.motorPower[i];
    }
}

void MotorStopData::setData(MStopData data)
{
    for(int i = 0; i < num; i++){
        motorChecked[i] = data.motorChecked[i];
    }
}

void RingData::setData(RingData data)
{
    yinfuTime = data.yinfuTime;
    yinpinID = data.yinpinID;
    yinfuID = data.yinfuID;
}
