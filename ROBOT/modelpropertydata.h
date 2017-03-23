#ifndef MODELPROPERTYDATA_H
#define MODELPROPERTYDATA_H
#include <QString>
#include <QMap>
typedef struct MotorStartData MStartData;
typedef struct MotorStopData MStopData;
typedef struct RingData RingData;
typedef struct PanduanData PdData;

const float yinpin[22] = {0.0, 1047.0, 1175.0, 1319.0, 1397.0, 1568.0, 1760.0, 1975.0,
                    523.2, 587.3, 659.2, 698.4, 783.9, 880.0, 987.7,
                    261.6, 293.6, 329.6, 349.2, 391.9, 440.0, 493.8};
const float yinfu[4] = {0.5, 0.25, 0.125, 0.0625};

struct MotorStartData{
    const int num = 4;
    bool motorChecked[4];
    int motorPower[4];

    void setData(MStartData data);
};
struct MotorStopData{
    const int num = 4;
    bool motorChecked[4];

    void setData(MStopData data);
};

struct RingData{
    float yinfuTime;
    int yinfuID;
    int yinpinID;

    void setData(RingData data);
};

struct PanduanData{
    QString tj1_variableName;
    QString tj1_operator;
    QString tj1_compareContent;
    bool tj2_isEnable;
    QString tj2_variableName;
    QString tj2_operator;
    QString tj2_compareContent;
    QString tj2_logicVersusTj1;

    void setData(PanduanData data);
};
/*************用C++11的新特性--初始化列表初始化,需要Qt5.1以上版本***************/
const QMap<QString, QString> sensorIconMap({std::pair<QString, QString>("ZL_S", ":/images/dlgicon/ZL_S.PNG"),
                                            std::pair<QString, QString>("AN_S", ":/images/dlgicon/AN_S.PNG"),
                                            std::pair<QString, QString>("DH_S", ":/images/dlgicon/COUNTER_S.PNG"),
                                            std::pair<QString, QString>("CS_S", ":/images/dlgicon/CSB_S.PNG"),
                                            std::pair<QString, QString>("GZ_S", ":/images/dlgicon/GZ_S.PNG"),
                                            std::pair<QString, QString>("HY_S", ":/images/dlgicon/HY_S.PNG"),
                                            std::pair<QString, QString>("KQ_S", ":/images/dlgicon/KQ_S.PNG"),
                                            std::pair<QString, QString>("BZ_S", ":/images/dlgicon/BZ_S.PNG"),
                                            std::pair<QString, QString>("SY_S", ":/images/dlgicon/SY_S.PNG"),
                                            std::pair<QString, QString>("WD_S", ":/images/dlgicon/WSD_S.PNG"),
                                            std::pair<QString, QString>("XJ_S", ":/images/dlgicon/XJ_S.PNG")});
#endif // MODELPROPERTYDATA_H
