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

    void prepare (float sampleRate_, int numChannels_, float maxDelayMs)
    {
        sampleRate = sampleRate_;
        numChannels = numChannels_;

        writeIndex.resize (numChannels);
        circularBuffer.resize (numChannels);

        bufferSize = static_cast<int> (maxDelayMs * sampleRate / 1000.0f);

        reset();
    }

    void reset()
    {
        std::fill (writeIndex.begin(), writeIndex.end(), 0);
        std::fill (circularBuffer.begin(), circularBuffer.end(), std::vector<float> (bufferSize, 0.0f));
    }

    void push (int channel, float sample)
    {
        circularBuffer[channel][writeIndex[channel]] = sample;
        writeIndex[channel] = (writeIndex[channel] + 1) % bufferSize; // move the write index forward
    }

    float pop (int channel, float delayMs)
    {
        int delaySamples = static_cast<int> (std::floor (delayMs * sampleRate / 1000.0f));
        jassert (delaySamples < bufferSize); // make sure the delay is within the buffer size

        int readIndex = (writeIndex[channel] - delaySamples + bufferSize) % bufferSize;

        return circularBuffer[channel][readIndex];
    }

private:
    float sampleRate { 44100.0 };
    int numChannels { 1 };

    std::vector<std::vector<float>> circularBuffer;
    int bufferSize { 44100 };
    std::vector<int> writeIndex;
};