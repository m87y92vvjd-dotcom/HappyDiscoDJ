#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize(1100, 720);

    titleLabel.setText("Happy Disco DJ", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(36.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(titleLabel);

    actionButton.setButtonText("Start Session");
    addAndMakeVisible(actionButton);

    secondButton.setButtonText("Open Mixer");
    addAndMakeVisible(secondButton);

    startTimerHz(20);
}

MainComponent::~MainComponent() = default;

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff101820));

    auto gradient = juce::ColourGradient(
        juce::Colour(0xffff5f6d),
        getWidth() * 0.15f, getHeight() * 0.15f,
        juce::Colour(0xffffc371),
        getWidth() * 0.85f, getHeight() * 0.85f,
        false
    );

    g.setGradientFill(gradient);
    g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(24.0f), 28.0f);

    g.setColour(juce::Colours::white.withAlpha(0.90f));
    g.drawText("Welcome to your DJ workspace", getLocalBounds().reduced(80, 80), juce::Justification::centredTop, true);
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds().reduced(60);

    titleLabel.setBounds(bounds.removeFromTop(120).reduced(20, 0));

    auto controls = bounds.removeFromTop(120);
    actionButton.setBounds(controls.removeFromLeft(200).reduced(10, 15));
    secondButton.setBounds(controls.removeFromLeft(200).reduced(10, 15));

    accentGlow = juce::Rectangle<float>(
        static_cast<float>(getWidth()) * 0.12f,
        static_cast<float>(getHeight()) * 0.15f,
        static_cast<float>(getWidth()) * 0.5f,
        static_cast<float>(getHeight()) * 0.4f
    );
}

void MainComponent::timerCallback()
{
    pulse = (pulse + 1) % 360;
    repaint();
}
