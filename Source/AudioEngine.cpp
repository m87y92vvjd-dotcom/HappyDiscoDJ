#pragma once

#include <atomic>
#include <memory>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_basics/juce_audio_basics.h>

class AudioEngine : public juce::AudioAppComponent
{
public:
    AudioEngine();
    ~AudioEngine() override;

    void loadFile(const juce::File& file);
    void play();
    void stop();
    bool isPlaying() const noexcept;
    void setGain(float newGain) noexcept;
    void setPosition(double seconds);

    double getCurrentPosition() const;
    double getLengthInSeconds() const;
    float getOutputLevel() const noexcept;
    juce::String getTrackName() const;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::String currentTrackName { "No track loaded" };
    std::atomic<float> outputLevel { 0.0f };
    float gain = 0.75f;
};
