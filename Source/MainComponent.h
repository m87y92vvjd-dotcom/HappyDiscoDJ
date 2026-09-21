#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class MainComponent : public juce::Component,
                     private juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    juce::Label titleLabel;
    juce::TextButton actionButton;
    juce::TextButton secondButton;
    juce::Rectangle<float> accentGlow;
    int pulse = 0;
};
