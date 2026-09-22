#include "MixerComponent.h"
#include <utility>

MixerComponent::MixerComponent()
{
    titleLabel.setText("MIXER", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);

    crossfadeLabel.setText("Crossfader", juce::dontSendNotification);
    crossfadeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(crossfadeLabel);

    crossfader.setRange(-1.0, 1.0, 0.01);
    crossfader.setValue(0.0);
    crossfader.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    crossfader.onValueChange = [this]
    {
        onCrossfade(static_cast<float>(crossfader.getValue()));
    };
    addAndMakeVisible(crossfader);

    masterLabel.setText("Master", juce::dontSendNotification);
    masterLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(masterLabel);

    masterGain.setRange(0.0, 1.0, 0.01);
    masterGain.setValue(0.85);
    masterGain.setSliderStyle(juce::Slider::LinearVertical);
    masterGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    masterGain.onValueChange = [this]
    {
        onMasterGain(static_cast<float>(masterGain.getValue()));
    };
    addAndMakeVisible(masterGain);

    channelGain.setRange(0.0, 1.0, 0.01);
    channelGain.setValue(0.75);
    channelGain.setSliderStyle(juce::Slider::LinearVertical);
    channelGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    addAndMakeVisible(channelGain);
}

void MixerComponent::setCrossfadeCallback(std::function<void(float)> callback)
{
    onCrossfade = std::move(callback);
}

void MixerComponent::setMasterGainCallback(std::function<void(float)> callback)
{
    onMasterGain = std::move(callback);
}

void MixerComponent::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    g.setColour(juce::Colour(0xff151c2b));
    g.fillRoundedRectangle(area.reduced(5.0f), 18.0f);
    g.setColour(juce::Colours::white.withAlpha(0.15f));
    g.drawRoundedRectangle(area.reduced(5.0f), 18.0f, 1.0f);

    g.setColour(juce::Colour(0xffff9f1c).withAlpha(0.8f));
    g.fillRoundedRectangle(getLocalBounds().toFloat().withTrimmedTop(6.0f).withTrimmedBottom(6.0f)
                            .withTrimmedLeft(getWidth() * 0.42f).withTrimmedRight(getWidth() * 0.42f), 3.0f);
}

void MixerComponent::resized()
{
    auto area = getLocalBounds().reduced(14);
    titleLabel.setBounds(area.removeFromTop(28));
    crossfadeLabel.setBounds(area.removeFromTop(22));
    crossfader.setBounds(area.removeFromTop(42).reduced(4, 8));
    masterLabel.setBounds(area.removeFromTop(22));

    auto faders = area;
    masterGain.setBounds(faders.removeFromLeft(faders.getWidth() / 2).reduced(10, 4));
    channelGain.setBounds(faders.reduced(10, 4));
}
