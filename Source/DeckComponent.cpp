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
    void setStatusText(const juce::String& text);
    void setTrackText(const juce::String& text);

private:
    juce::Label nameLabel;
    juce::Label statusLabel;
    juce::Label trackLabel;
    juce::Slider volumeSlider;
    juce::Slider bpmSlider;
    juce::TextButton loadButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;

    std::function<void()> onLoad = [] {};
    std::function<void()> onPlay = [] {};
    std::function<void()> onStop = [] {};

    juce::String deckName;
    juce::Path waveform;
};
