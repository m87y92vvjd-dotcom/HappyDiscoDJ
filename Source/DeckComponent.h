#pragma once

#include <functional>
#include <juce_gui_extra/juce_gui_extra.h>

class DeckComponent : public juce::Component
{
public:
    explicit DeckComponent(const juce::String& deckName);
    ~DeckComponent() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setLoadCallback(std::function<void()> callback);
    void setPlayCallback(std::function<void()> callback);
    void setStopCallback(std::function<void()> callback);
    void setGainCallback(std::function<void(float)> callback);
    void setSeekCallback(std::function<void(double)> callback);
    void setStatusText(const juce::String& text);
    void setTrackText(const juce::String& text);
    void setPlayback(double position, double length, float level);

private:
    juce::Label nameLabel, statusLabel, trackLabel, timeLabel;
    juce::Slider volumeSlider, bpmSlider, positionSlider;
    juce::TextButton loadButton, playButton, stopButton;
    std::function<void()> onLoad = [] {};
    std::function<void()> onPlay = [] {};
    std::function<void()> onStop = [] {};
    std::function<void(float)> onGain = [] (float) {};
    std::function<void(double)> onSeek = [] (double) {};
    float outputLevel = 0.0f;
};
