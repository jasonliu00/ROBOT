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

void PanduanData::setData(PanduanData data)
{
    tj1_variableName = data.tj1_variableName;
    tj1_operator = data.tj1_operator;
    tj1_compareContent = data.tj1_compareContent;
    tj2_isEnable = data.tj2_isEnable;
    tj2_variableName = data.tj2_variableName;
    tj2_operator = data.tj2_operator;
    tj2_compareContent = data.tj2_compareContent;
    tj2_logicVersusTj1 = data.tj2_logicVersusTj1;
}
