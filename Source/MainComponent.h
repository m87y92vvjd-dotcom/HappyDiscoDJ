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
    void updateMasterGain(float value);

    juce::Label titleLabel;
    juce::Label statusLabel;
    juce::TextButton sessionButton;
    juce::TextButton libraryButton;
    juce::TextButton mixerButton;
    juce::TextButton settingsButton;
    std::unique_ptr<DeckComponent> leftDeck;
    std::unique_ptr<DeckComponent> rightDeck;
    std::unique_ptr<AudioEngine> leftEngine;
    std::unique_ptr<AudioEngine> rightEngine;
    std::unique_ptr<MixerComponent> mixer;
    juce::Rectangle<float> glowArea;

    float masterLevel = 0.85f;
    float crossfadeValue = 0.0f;
};
