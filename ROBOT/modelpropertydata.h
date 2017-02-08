#ifndef MODELPROPERTYDATA_H
#define MODELPROPERTYDATA_H
typedef struct MotorStartData MStartData;
typedef struct MotorStopData MStopData;
typedef struct RingData RingData;

const float yinpin[22] = {0.0, 1047.0, 1175.0, 1319.0, 1397.0, 1568.0, 1760.0, 1975.0,
                    523.2, 587.3, 659.2, 698.4, 783.9, 880.0, 987.7,
                    261.6, 293.6, 329.6, 349.2, 391.9, 440.0, 493.8};
const float yinfu[4] = {0.5, 0.25, 0.125, 0.0625};

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

struct RingData{
    float yinfuTime;
    int yinfuID;
    int yinpinID;

    void setData(RingData data);
};


#endif // MODELPROPERTYDATA_H
