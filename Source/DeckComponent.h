#pragma once

#include <memory>
#include <juce_gui_extra/juce_gui_extra.h>

class DeckComponent : public juce::Component
{
public:
    explicit DeckComponent(const juce::String& deckName);
    ~DeckComponent() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::Label nameLabel;
    juce::Label statusLabel;
    juce::Label bpmLabel;
    juce::Slider volumeSlider;
    juce::Slider bpmSlider;
    juce::TextButton loadButton;
    juce::String deckName;
    juce::Path waveform;
};
