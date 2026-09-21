#pragma once

#include <memory>
#include <juce_gui_extra/juce_gui_extra.h>
#include "AudioEngine.h"
#include "DeckComponent.h"
#include "MixerComponent.h"

class MainComponent : public juce::Component, private juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void openTrackForDeck(AudioEngine&, const juce::String&, DeckComponent&);
    void updateCrossfade(float value);

    juce::Label titleLabel, statusLabel;
    juce::TextButton sessionButton, libraryButton, mixerButton, settingsButton;
    std::unique_ptr<DeckComponent> leftDeck, rightDeck;
    std::unique_ptr<AudioEngine> leftEngine, rightEngine;
    std::unique_ptr<MixerComponent> mixer;
    juce::Rectangle<float> glowArea;
    int pulse = 0;
};
