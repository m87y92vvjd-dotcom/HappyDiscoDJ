#pragma once

#include <functional>
#include <juce_gui_extra/juce_gui_extra.h>

class MixerComponent : public juce::Component
{
public:
    MixerComponent();
    ~MixerComponent() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

    void setCrossfadeCallback(std::function<void(float)> callback);
    void setMasterGainCallback(std::function<void(float)> callback);

private:
    juce::Label titleLabel;
    juce::Label crossfadeLabel;
    juce::Label masterLabel;
    juce::Slider crossfader;
    juce::Slider masterGain;
    juce::Slider channelGain;

    std::function<void(float)> onCrossfade = [](float) {};
    std::function<void(float)> onMasterGain = [](float) {};
};
