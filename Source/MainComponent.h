#include "DeckComponent.h"

DeckComponent::DeckComponent(const juce::String& deckName)
    : deckName(deckName)
{
    nameLabel.setText(deckName, juce::dontSendNotification);
    nameLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    nameLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(nameLabel);

    statusLabel.setText("Ready to spin", juce::dontSendNotification);
    statusLabel.setFont(juce::Font(13.0f));
    statusLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(statusLabel);

    trackLabel.setText("No track loaded", juce::dontSendNotification);
    trackLabel.setFont(juce::Font(12.0f, juce::Font::italic));
    trackLabel.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.9f));
    addAndMakeVisible(trackLabel);

    volumeSlider.setRange(0.0, 100.0);
    volumeSlider.setValue(68.0);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(volumeSlider);

    bpmSlider.setRange(70.0, 180.0);
    bpmSlider.setValue(128.0);
    bpmSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(bpmSlider);

    loadButton.setButtonText("Load");
    loadButton.onClick = [this] { onLoad(); };
    addAndMakeVisible(loadButton);

    playButton.setButtonText("Play");
    playButton.onClick = [this] { onPlay(); };
    addAndMakeVisible(playButton);

    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { onStop(); };
    addAndMakeVisible(stopButton);

    waveform.clear();
    waveform.startNewSubPath(0.0f, 12.0f);
    for (int i = 1; i <= 30; ++i)
    {
        const auto x = static_cast<float>(i) * 8.0f;
        const auto y = 12.0f + std::sin(static_cast<float>(i) * 0.8f) * 12.0f;
        waveform.lineTo(x, y);
    }
}

void DeckComponent::setLoadCallback(std::function<void()> callback)
{
    onLoad = std::move(callback);
}

void DeckComponent::setPlayCallback(std::function<void()> callback)
{
    onPlay = std::move(callback);
}

void DeckComponent::setStopCallback(std::function<void()> callback)
{
    onStop = std::move(callback);
}

void DeckComponent::setStatusText(const juce::String& text)
{
    statusLabel.setText(text, juce::dontSendNotification);
}

void DeckComponent::setTrackText(const juce::String& text)
{
    trackLabel.setText(text, juce::dontSendNotification);
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

    g.setColour(juce::Colours::white.withAlpha(0.16f));
    g.drawRoundedRectangle(area.reduced(12.0f), 18.0f, 1.0f);

    g.setColour(juce::Colours::orange.withAlpha(0.8f));
    g.fillRect(14.0f, 86.0f, static_cast<float>(getWidth() - 28), 6.0f);
}

void DeckComponent::resized()
{
    auto area = getLocalBounds().reduced(18);

    nameLabel.setBounds(area.removeFromTop(34));
    statusLabel.setBounds(area.removeFromTop(20).reduced(0, 6));
    trackLabel.setBounds(area.removeFromTop(18).reduced(0, 6));

    auto controls = area.removeFromTop(110);
    volumeSlider.setBounds(controls.removeFromLeft(controls.getWidth() * 0.48f).reduced(0, 10));
    bpmSlider.setBounds(controls.removeFromLeft(controls.getWidth() * 0.48f).reduced(0, 10));

    auto buttons = area.removeFromTop(44);
    loadButton.setBounds(buttons.removeFromLeft(90).reduced(4, 4));
    playButton.setBounds(buttons.removeFromLeft(90).reduced(4, 4));
    stopButton.setBounds(buttons.removeFromLeft(90).reduced(4, 4));
}
