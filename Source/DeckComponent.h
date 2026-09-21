#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    formatManager.registerBasicFormats();
    setAudioChannels(0, 2);
}

AudioEngine::~AudioEngine()
{
    stop();
    shutdownAudio();
}

void AudioEngine::loadFile(const juce::File& file)
{
    stop();
    readerSource.reset();

    auto reader = formatManager.createReaderFor(file);
    if (reader == nullptr)
    {
        currentTrackName = "Unsupported file format";
        return;
    }

    currentTrackName = file.getFileName();
    readerSource.reset(new juce::AudioFormatReaderSource(reader, true));
    transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
    transportSource.setGain(gain);
}

void AudioEngine::play()
{
    if (readerSource != nullptr)
        transportSource.start();
}

void AudioEngine::stop()
{
    transportSource.stop();
}

bool AudioEngine::isPlaying() const noexcept
{
    return transportSource.isPlaying();
}

void AudioEngine::setGain(float newGain) noexcept
{
    gain = juce::jlimit(0.0f, 2.0f, newGain);
    transportSource.setGain(gain);
}

double AudioEngine::getCurrentPosition() const
{
    return transportSource.getCurrentPosition();
}

double AudioEngine::getLengthInSeconds() const
{
    if (readerSource == nullptr)
        return 0.0;

    return transportSource.getLengthInSeconds();
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
        bufferToFill.buffer->clear();
        return;
    }

    transportSource.getNextAudioBlock(bufferToFill);
    bufferToFill.buffer->applyGain(gain);
}

void AudioEngine::releaseResources()
{
    transportSource.stop();
    transportSource.releaseResources();
}
