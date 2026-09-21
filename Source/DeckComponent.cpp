#include "DeckComponent.h"

DeckComponent::DeckComponent(const juce::String& deckName)
    : deckName(deckName)
{
    nameLabel.setText(deckName, juce::dontSendNotification);
    nameLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    addAndMakeVisible(nameLabel);

    statusLabel.setText("Ready", juce::dontSendNotification);
    statusLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(statusLabel);

    volumeSlider.setRange(0.0, 100.0);
    volumeSlider.setValue(65.0);
    addAndMakeVisible(volumeSlider);

    loadButton.setButtonText("Load Track");
    addAndMakeVisible(loadButton);
}

void DeckComponent::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    g.setColour(juce::Colour(0xff1d2733));
    g.fillRoundedRectangle(area.reduced(6.0f), 16.0f);

    g.setColour(juce::Colours::white.withAlpha(0.9f));
    g.drawRoundedRectangle(area.reduced(12.0f), 16.0f, 1.0f);
}

void DeckComponent::resized()
{
    auto area = getLocalBounds().reduced(20);
    nameLabel.setBounds(area.removeFromTop(40));
    statusLabel.setBounds(area.removeFromTop(24).reduced(0, 8));
    volumeSlider.setBounds(area.removeFromTop(45).reduced(0, 10));
    loadButton.setBounds(area.removeFromTop(40).reduced(0, 10));
}
