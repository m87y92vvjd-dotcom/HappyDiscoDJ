#include "SplashComponent.h"

SplashComponent::SplashComponent()
{
    splashLabel.setText("Play. Mix. Glow.", juce::dontSendNotification);
    splashLabel.setJustificationType(juce::Justification::centred);
    splashLabel.setFont(juce::Font(28.0f, juce::Font::bold));
    addAndMakeVisible(splashLabel);
}

void SplashComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff0d1321));

    g.setColour(juce::Colour(0xffffb703));
    g.fillEllipse(getLocalBounds().toFloat().reduced(120.0f));

    g.setColour(juce::Colours::black);
    g.drawText("Happy Disco DJ", getLocalBounds(), juce::Justification::centred, true);
}

void SplashComponent::resized()
{
    splashLabel.setBounds(getLocalBounds());
}
