#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class Delay
{
public:
    Delay()
    {
    }

    ~Delay()
    {
    }

    void prepare (float sampleRate_, float maxDelayMs)
    {
        sampleRate = sampleRate_;

        bufferSize = static_cast<int> (maxDelayMs * sampleRate / 1000.0f);
        circularBuffer.resize (bufferSize);

        reset();
    }

    void reset()
    {
        writeIndex = 0;

        std::fill (circularBuffer.begin(), circularBuffer.end(), 0.0f);
    }

    void push (float sample)
    {
        circularBuffer[writeIndex] = sample;
        writeIndex = (writeIndex + 1) % bufferSize; // move the write index forward
    }

    float pop (float delayMs)
    {
        int delaySamples = static_cast<int> (std::floor (delayMs * sampleRate / 1000.0f));
        jassert (delaySamples < bufferSize); // make sure the delay is within the buffer size

        int readIndex = (writeIndex - delaySamples + bufferSize) % bufferSize;

        return circularBuffer[readIndex];
    }

private:
    float sampleRate { 44100.0 };

    std::vector<float> circularBuffer;
    int bufferSize { 44100 };
    int writeIndex { 0 };
};