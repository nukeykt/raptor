#include <windows.h>
#include <mmsystem.h>
#include "SDL.h"
#include "common.h"
#include "musapi.h"

HMIDISTRM mpu_stream;
UINT mpu_device;
int mpu_init;
int mpu_initcnt;
int mpu_timer;

int MPU_Init(int device)
{
    MIDIOUTCAPS caps;

    if (mpu_initcnt++ != 0)
        return 1;

    mpu_device = device;

    if (mpu_device > midiOutGetNumDevs() || midiOutGetDevCaps(mpu_device, &caps, sizeof(caps)))
        mpu_device = MIDI_MAPPER;

    if (midiStreamOpen(&mpu_stream, &mpu_device, 1, NULL, NULL, NULL))
    {
        return 0;
    }

    mpu_timer = SDL_GetTicks();
    mpu_init = 1;
    return 1;
}

void MPU_DeInit(void)
{
    if (--mpu_initcnt != 0)
        return;
    if (!mpu_init)
        return;

    midiStreamClose(mpu_stream);

    mpu_stream = NULL;
    mpu_init = 0;
}

static unsigned int MPU_MapChannel(unsigned chan)
{
    if (chan < 9)
        return chan;
    if (chan == 15)
        return 9;
    return chan + 1;
}

static void KeyOffEvent(unsigned int chan, unsigned int key)
{
    if (!mpu_init)
        return;
    uint32_t data = 0;

    data = 0x80 | MPU_MapChannel(chan);
    data |= key << 8;
    data |= 64 << 16;

    midiOutShortMsg((HMIDIOUT)mpu_stream, data);
}

static void KeyOnEvent(int chan, unsigned int key, unsigned int volume)
{
    if (!mpu_init)
        return;
    uint32_t data = 0;

    data = 0x90 | MPU_MapChannel(chan);
    data |= key << 8;
    data |= volume << 16;

    midiOutShortMsg((HMIDIOUT)mpu_stream, data);
}

static void ControllerEvent(unsigned int chan, unsigned int controller, unsigned int param)
{
    if (!mpu_init)
        return;
    static int event_map[] = {
        0, 0, 1, 7, 10, 11, 91, 93, 64, 67, 120, 123, -1, -1, 121, -1
    };
    uint32_t data = 0;
    switch (controller)
    {
    case 0:
        data = 0xc0 | MPU_MapChannel(chan);
        data |= param << 8;
        break;
    default:
        if (event_map[controller] == -1)
            return;
        data = 0xb0 | MPU_MapChannel(chan);
        data |= event_map[controller] << 8;
        data |= param << 16;
        break;
    }

    midiOutShortMsg((HMIDIOUT)mpu_stream, data);
}

static void PitchBendEvent(unsigned int chan, int bend)
{
    if (!mpu_init)
        return;
    uint32_t data = 0;
    bend <<= 6;

    data = 0xe0 | MPU_MapChannel(chan);
    data |= (bend & 127) << 8;
    data |= ((bend>>7)&127) << 16;

    midiOutShortMsg((HMIDIOUT)mpu_stream, data);
}

musdevice_t mus_device_mpu = {
    MPU_Init,
    MPU_DeInit,
    NULL,

    KeyOffEvent,
    KeyOnEvent,
    ControllerEvent,
    PitchBendEvent,
};

