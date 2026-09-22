#include "SettingsComponent.h"

SettingsComponent::SettingsComponent()
{
    titleLabel.setText("Settings", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(titleLabel);

    outputLabel.setText("Output gain", juce::dontSendNotification);
    latencyLabel.setText("Latency", juce::dontSendNotification);
    crossfadeLabel.setText("Crossfade boost", juce::dontSendNotification);

    addAndMakeVisible(outputLabel);
    addAndMakeVisible(latencyLabel);
    addAndMakeVisible(crossfadeLabel);

    outputSlider.setRange(0.0, 1.0, 0.01);
    outputSlider.setValue(0.85);
    outputSlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, 60, 20);
    addAndMakeVisible(outputSlider);

    latencySlider.setRange(0.0, 300.0, 1.0);
    latencySlider.setValue(48.0);
    latencySlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, 60, 20);
    addAndMakeVisible(latencySlider);

    crossfadeSlider.setRange(0.0, 2.0, 0.01);
    crossfadeSlider.setValue(1.0);
    crossfadeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, 60, 20);
    addAndMakeVisible(crossfadeSlider);
}

void SettingsComponent::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    g.setColour(juce::Colour(0xff151d2c));
    g.fillRoundedRectangle(area.reduced(6.0f), 18.0f);
    g.setColour(juce::Colours::white.withAlpha(0.12f));
    g.drawRoundedRectangle(area.reduced(6.0f), 18.0f, 1.0f);
}

void SettingsComponent::resized()
{
    auto area = getLocalBounds().reduced(18);
    titleLabel.setBounds(area.removeFromTop(30));

    auto rows = area.reduced(8);
    outputLabel.setBounds(rows.removeFromTop(28));
    outputSlider.setBounds(rows.removeFromTop(28).reduced(0, 4));
    latencyLabel.setBounds(rows.removeFromTop(28));
    latencySlider.setBounds(rows.removeFromTop(28).reduced(0, 4));
    crossfadeLabel.setBounds(rows.removeFromTop(28));
    crossfadeSlider.setBounds(rows.removeFromTop(28).reduced(0, 4));
}
