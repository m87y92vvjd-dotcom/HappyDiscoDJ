#include "DeckComponent.h"
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
    timeLabel.setText("00:00 / 00:00", juce::dontSendNotification);
    timeLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(timeLabel);

    volumeSlider.setRange(0.0, 100.0, 0.1);
    volumeSlider.setValue(68.0);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    volumeSlider.onValueChange = [this] { onGain(static_cast<float>(volumeSlider.getValue() / 100.0)); };
    addAndMakeVisible(volumeSlider);

    bpmSlider.setRange(70.0, 180.0, 0.1);
    bpmSlider.setValue(128.0);
    bpmSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(bpmSlider);

    positionSlider.setRange(0.0, 1.0, 0.001);
    positionSlider.setValue(0.0, juce::dontSendNotification);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    positionSlider.onValueChange = [this]
    {
        if (! positionSlider.isMouseButtonDown())
            return;
        onSeek(positionSlider.getValue());
    };
    addAndMakeVisible(positionSlider);

    loadButton.setButtonText("Load"); loadButton.onClick = [this] { onLoad(); }; addAndMakeVisible(loadButton);
    playButton.setButtonText("Play"); playButton.onClick = [this] { onPlay(); }; addAndMakeVisible(playButton);
    stopButton.setButtonText("Stop"); stopButton.onClick = [this] { onStop(); }; addAndMakeVisible(stopButton);
}

void DeckComponent::setLoadCallback(std::function<void()> callback) { onLoad = std::move(callback); }
void DeckComponent::setPlayCallback(std::function<void()> callback) { onPlay = std::move(callback); }
void DeckComponent::setStopCallback(std::function<void()> callback) { onStop = std::move(callback); }
void DeckComponent::setGainCallback(std::function<void(float)> callback) { onGain = std::move(callback); }
void DeckComponent::setSeekCallback(std::function<void(double)> callback) { onSeek = std::move(callback); }
void DeckComponent::setStatusText(const juce::String& text) { statusLabel.setText(text, juce::dontSendNotification); }
void DeckComponent::setTrackText(const juce::String& text) { trackLabel.setText(text, juce::dontSendNotification); }

void DeckComponent::setPlayback(double position, double length, float level)
{
    const auto safeLength = juce::jmax(0.0, length);
    const auto progress = safeLength > 0.0 ? juce::jlimit(0.0, 1.0, position / safeLength) : 0.0;
    positionSlider.setValue(progress, juce::dontSendNotification);
    timeLabel.setText(juce::String::formatted("%02d:%02d / %02d:%02d",
        static_cast<int>(position) / 60, static_cast<int>(position) % 60,
        static_cast<int>(safeLength) / 60, static_cast<int>(safeLength) % 60), juce::dontSendNotification);
    outputLevel = juce::jlimit(0.0f, 1.0f, level);
    repaint();
}

void DeckComponent::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    g.setColour(juce::Colour(0xff1c212d)); g.fillRoundedRectangle(area.reduced(6.0f), 20.0f);
    g.setColour(juce::Colour(0xff2d3745)); g.fillRoundedRectangle(area.reduced(16.0f), 18.0f);

    const auto meter = getLocalBounds().toFloat().withTrimmedLeft(18.0f).withTrimmedRight(18.0f);
    g.setColour(juce::Colours::black.withAlpha(0.35f)); g.fillRoundedRectangle(meter.withHeight(8.0f).withY(92.0f), 4.0f);
    g.setColour(juce::Colour(0xff58d68d)); g.fillRoundedRectangle(meter.withWidth(meter.getWidth() * outputLevel).withHeight(8.0f).withY(92.0f), 4.0f);
    g.setColour(juce::Colours::white.withAlpha(0.16f)); g.drawRoundedRectangle(area.reduced(12.0f), 18.0f, 1.0f);
}

void DeckComponent::resized()
{
    auto area = getLocalBounds().reduced(18);
    nameLabel.setBounds(area.removeFromTop(34));
    statusLabel.setBounds(area.removeFromTop(20));
    trackLabel.setBounds(area.removeFromTop(24));
    timeLabel.setBounds(area.removeFromTop(20));
    positionSlider.setBounds(area.removeFromTop(30).reduced(4, 8));
    auto controls = area.removeFromTop(110);
    volumeSlider.setBounds(controls.removeFromLeft(controls.getWidth() / 2).reduced(4));
    bpmSlider.setBounds(controls.reduced(4));
    auto buttons = area.removeFromTop(44);
    loadButton.setBounds(buttons.removeFromLeft(90).reduced(4));
    playButton.setBounds(buttons.removeFromLeft(90).reduced(4));
    stopButton.setBounds(buttons.removeFromLeft(90).reduced(4));
}
