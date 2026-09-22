#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class SettingsComponent : public juce::Component
{
public:
    SettingsComponent();
    ~SettingsComponent() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::Label titleLabel;
    juce::Label outputLabel;
    juce::Label latencyLabel;
    juce::Label crossfadeLabel;
    juce::Slider outputSlider;
    juce::Slider latencySlider;
    juce::Slider crossfadeSlider;
};
