#include "AudioEngine.h"
#include <cmath>

AudioEngine::AudioEngine()
{
    formatManager.registerBasicFormats();
    setAudioChannels(0, 2);
}

AudioEngine::~AudioEngine()
{
    stop();
    transportSource.setSource(nullptr);
    readerSource.reset();
    shutdownAudio();
}

void AudioEngine::loadFile(const juce::File& file)
{
    stop();
    transportSource.setSource(nullptr);
    readerSource.reset();
    outputLevel.store(0.0f);

    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            currentTrackName = file.getFileName();
            const auto sampleRate = reader->sampleRate;
            readerSource.reset(new juce::AudioFormatReaderSource(reader, true));
            transportSource.setSource(readerSource.get(), 0, nullptr, sampleRate);
            transportSource.setGain(gain);
            return;
        }
    }

    currentTrackName = "Unsupported file format";
}

void AudioEngine::play()
{
    if (readerSource != nullptr && ! transportSource.isPlaying())
        transportSource.start();
}

void AudioEngine::stop()
{
    transportSource.stop();
    outputLevel.store(0.0f);
}

bool AudioEngine::isPlaying() const noexcept
{
    return transportSource.isPlaying();
}

void AudioEngine::setGain(float newGain) noexcept
{
    gain = juce::jlimit(0.0f, 1.0f, newGain);
    transportSource.setGain(gain);
}

void AudioEngine::setPosition(double seconds)
{
    if (readerSource != nullptr)
        transportSource.setPosition(juce::jlimit(0.0, getLengthInSeconds(), seconds));
}

double AudioEngine::getCurrentPosition() const
{
    return transportSource.getCurrentPosition();
}

double AudioEngine::getLengthInSeconds() const
{
    return transportSource.getLengthInSeconds();
}

float AudioEngine::getOutputLevel() const noexcept
{
    return outputLevel.load();
}

juce::String AudioEngine::getTrackName() const
{
    return currentTrackName;
}

void AudioEngine::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void AudioEngine::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        outputLevel.store(0.0f);
        return;
    }

    transportSource.getNextAudioBlock(bufferToFill);

    const auto rms = bufferToFill.buffer->getRMSLevel(
        bufferToFill.startSample, bufferToFill.numSamples);

    outputLevel.store(juce::jlimit(0.0f, 1.0f, rms * 2.5f));
}

void AudioEngine::releaseResources()
{
    transportSource.stop();
    transportSource.releaseResources();
}
