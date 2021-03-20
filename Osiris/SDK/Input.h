#pragma once

#include "Pad.h"
#include "Vector.h"

template<typename FuncType>
__forceinline static FuncType qCallVFunction(void* ppClass, int index)
{
    int* pVTable = *(int**)ppClass;
    int dwAddress = pVTable[index];
    return (FuncType)(dwAddress);
}

class Input {
public:
#ifdef _WIN32
    PAD(12)
#else
    PAD(16)
#endif
    bool isTrackIRAvailable;
    bool isMouseInitialized;
    bool isMouseActive;
#ifdef _WIN32
    PAD(158)
#else
    PAD(162)
#endif
    bool isCameraInThirdPerson;
    bool cameraMovingWithMouse;
    Vector cameraOffset;

    UserCmd* GetUserCmd(int nSlot, int sequence_number)
    {
        using OriginalFn = UserCmd * (__thiscall*)(void*, int, int);
        return qCallVFunction<OriginalFn>(this, 8)(this, nSlot, sequence_number);
    }
};
