#include "DeckComponent.h"
#include <cmath>
#include <utility>

DeckComponent::DeckComponent(const juce::String& deckName)
{
    nameLabel.setText(deckName, juce::dontSendNotification);
    nameLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    addAndMakeVisible(nameLabel);
    statusLabel.setText("Ready to spin", juce::dontSendNotification);
    addAndMakeVisible(statusLabel);
    trackLabel.setText("No track loaded", juce::dontSendNotification);
    addAndMakeVisible(trackLabel);

    volumeSlider.setRange(0.0, 100.0, 0.1);
    volumeSlider.setValue(68.0);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    volumeSlider.onValueChange = [this] { onGain(static_cast<float>(volumeSlider.getValue() / 100.0)); };
    addAndMakeVisible(volumeSlider);

    bpmSlider.setRange(70.0, 180.0, 0.1);
    bpmSlider.setValue(128.0);
    bpmSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(bpmSlider);

    loadButton.setButtonText("Load"); loadButton.onClick = [this] { onLoad(); }; addAndMakeVisible(loadButton);
    playButton.setButtonText("Play"); playButton.onClick = [this] { onPlay(); }; addAndMakeVisible(playButton);
    stopButton.setButtonText("Stop"); stopButton.onClick = [this] { onStop(); }; addAndMakeVisible(stopButton);

    waveform.startNewSubPath(0.0f, 12.0f);
    for (int i = 1; i <= 30; ++i)
        waveform.lineTo(static_cast<float>(i) * 8.0f, 12.0f + std::sin(static_cast<float>(i) * 0.8f) * 12.0f);
}

void DeckComponent::setLoadCallback(std::function<void()> callback) { onLoad = std::move(callback); }
void DeckComponent::setPlayCallback(std::function<void()> callback) { onPlay = std::move(callback); }
void DeckComponent::setStopCallback(std::function<void()> callback) { onStop = std::move(callback); }
void DeckComponent::setGainCallback(std::function<void(float)> callback) { onGain = std::move(callback); }
void DeckComponent::setStatusText(const juce::String& text) { statusLabel.setText(text, juce::dontSendNotification); }
void DeckComponent::setTrackText(const juce::String& text) { trackLabel.setText(text, juce::dontSendNotification); }

void DeckComponent::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    g.setColour(juce::Colour(0xff1c212d)); g.fillRoundedRectangle(area.reduced(6.0f), 20.0f);
    g.setColour(juce::Colour(0xff2d3745)); g.fillRoundedRectangle(area.reduced(16.0f), 18.0f);
    g.setColour(juce::Colour(0xff58d68d)); g.strokePath(waveform, juce::PathStrokeType(2.5f));
    g.setColour(juce::Colours::white.withAlpha(0.16f)); g.drawRoundedRectangle(area.reduced(12.0f), 18.0f, 1.0f);
}

void DeckComponent::resized()
{
    auto area = getLocalBounds().reduced(18);
    nameLabel.setBounds(area.removeFromTop(34));
    statusLabel.setBounds(area.removeFromTop(20));
    trackLabel.setBounds(area.removeFromTop(24));
    auto controls = area.removeFromTop(110);
    volumeSlider.setBounds(controls.removeFromLeft(controls.getWidth() / 2).reduced(4));
    bpmSlider.setBounds(controls.reduced(4));
    auto buttons = area.removeFromTop(44);
    loadButton.setBounds(buttons.removeFromLeft(90).reduced(4));
    playButton.setBounds(buttons.removeFromLeft(90).reduced(4));
    stopButton.setBounds(buttons.removeFromLeft(90).reduced(4));
}
