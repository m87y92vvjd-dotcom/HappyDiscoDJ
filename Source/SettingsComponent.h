#include "LibraryComponent.h"

LibraryComponent::LibraryComponent()
{
    titleLabel.setText("Library", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(titleLabel);

    trackListLabel.setText(
        "1. Neon Rush\n"
        "2. Summer Heat\n"
        "3. Midnight Echo\n"
        "4. Groove Factory\n"
        "5. Electric Avenue",
        juce::dontSendNotification);
    trackListLabel.setJustificationType(juce::Justification::topLeft);
    addAndMakeVisible(trackListLabel);
}

void LibraryComponent::setTrackList(const juce::StringArray& tracks)
{
    trackListLabel.setText(tracks.joinIntoString("\n"), juce::dontSendNotification);
}

void LibraryComponent::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    g.setColour(juce::Colour(0xff151d2c));
    g.fillRoundedRectangle(area.reduced(6.0f), 18.0f);
    g.setColour(juce::Colours::white.withAlpha(0.12f));
    g.drawRoundedRectangle(area.reduced(6.0f), 18.0f, 1.0f);
}

void LibraryComponent::resized()
{
    auto area = getLocalBounds().reduced(18);
    titleLabel.setBounds(area.removeFromTop(30));
    trackListLabel.setBounds(area.reduced(6));
}
