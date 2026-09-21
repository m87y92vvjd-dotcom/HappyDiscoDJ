#include "DeckComponent.h"

DeckComponent::DeckComponent(const juce::String& deckName)
    : deckName(deckName)
{
    nameLabel.setText(deckName, juce::dontSendNotification);
    nameLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    nameLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(nameLabel);

    statusLabel.setText("Ready to spin", juce::dontSendNotification);
    statusLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(statusLabel);

    bpmLabel.setText("128 BPM", juce::dontSendNotification);
    bpmLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    bpmLabel.setJustificationType(juce::Justification::centredRight);
    bpmLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    addAndMakeVisible(bpmLabel);

    volumeSlider.setRange(0.0, 100.0);
    volumeSlider.setValue(68.0);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(volumeSlider);

    bpmSlider.setRange(70.0, 180.0);
    bpmSlider.setValue(128.0);
    bpmSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(bpmSlider);

    loadButton.setButtonText("Load Track");
    addAndMakeVisible(loadButton);

    waveform.clear();
    waveform.startNewSubPath(0.0f, 12.0f);
    for (int i = 1; i <= 24; ++i)
    {
        const auto x = static_cast<float>(i) * 8.0f;
        const auto y = 14.0f + std::sin(static_cast<float>(i) * 0.9f) * 12.0f;
        waveform.lineTo(x, y);
    }
}

void DeckComponent::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    g.setColour(juce::Colour(0xff1c212d));
    g.fillRoundedRectangle(area.reduced(6.0f), 20.0f);

    g.setColour(juce::Colour(0xff2d3745));
    g.fillRoundedRectangle(area.reduced(16.0f), 18.0f);

    g.setColour(juce::Colour(0xff58d68d));
    g.strokePath(waveform, juce::PathStrokeType(2.5f));

    g.setColour(juce::Colours::white.withAlpha(0.14f));
    g.drawRoundedRectangle(area.reduced(12.0f), 18.0f, 1.0f);

    g.setColour(juce::Colours::orange.withAlpha(0.8f));
    g.fillRect(14.0f, 90.0f, static_cast<float>(getWidth() - 28), 6.0f);
}

void DeckComponent::resized()
{
    auto area = getLocalBounds().reduced(16);

    nameLabel.setBounds(area.removeFromTop(36));
    bpmLabel.setBounds(area.removeFromTop(22).reduced(0, 0));

    statusLabel.setBounds(area.removeFromTop(24).reduced(0, 8));

    auto controls = area.removeFromTop(120);
    volumeSlider.setBounds(controls.removeFromLeft(controls.getWidth() * 0.45f).reduced(0, 10));
    bpmSlider.setBounds(controls.removeFromLeft(controls.getWidth() * 0.45f).reduced(0, 10));

    loadButton.setBounds(area.removeFromTop(36).reduced(0, 8));
}
