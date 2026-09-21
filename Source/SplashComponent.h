#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class SplashComponent : public juce::Component
{
public:
    SplashComponent();
    ~SplashComponent() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::Label splashLabel;
};
