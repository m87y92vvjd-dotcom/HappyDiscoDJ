#pragma once

#include <memory>
#include <juce_gui_extra/juce_gui_extra.h>
#include "DeckComponent.h"

class MainComponent : public juce::Component,
                     private juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    juce::Label titleLabel;
    juce::Label statusLabel;
    juce::TextButton sessionButton;
    juce::TextButton libraryButton;
    juce::TextButton mixerButton;
    juce::TextButton settingsButton;

    std::unique_ptr<DeckComponent> leftDeck;
    std::unique_ptr<DeckComponent> rightDeck;

    juce::Rectangle<float> glowArea;
    int pulse = 0;
};
