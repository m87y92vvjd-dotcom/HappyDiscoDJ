#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize(1280, 760); setOpaque(true);
    titleLabel.setText("Happy Disco DJ", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(32.0f, juce::Font::bold)); addAndMakeVisible(titleLabel);
    statusLabel.setText("Live session / 2 decks ready", juce::dontSendNotification); addAndMakeVisible(statusLabel);

    sessionButton.setButtonText("Session"); libraryButton.setButtonText("Library");
    mixerButton.setButtonText("Mixer"); settingsButton.setButtonText("Settings");
    addAndMakeVisible(sessionButton); addAndMakeVisible(libraryButton);
    addAndMakeVisible(mixerButton); addAndMakeVisible(settingsButton);

    leftEngine = std::make_unique<AudioEngine>(); rightEngine = std::make_unique<AudioEngine>();
    leftDeck = std::make_unique<DeckComponent>("Deck A"); rightDeck = std::make_unique<DeckComponent>("Deck B");
    addAndMakeVisible(leftDeck.get()); addAndMakeVisible(rightDeck.get());

    leftDeck->setLoadCallback([this] { openTrackForDeck(*leftEngine, "Deck A", *leftDeck); });
    rightDeck->setLoadCallback([this] { openTrackForDeck(*rightEngine, "Deck B", *rightDeck); });
    leftDeck->setPlayCallback([this] { leftEngine->play(); leftDeck->setStatusText("Playing"); });
    rightDeck->setPlayCallback([this] { rightEngine->play(); rightDeck->setStatusText("Playing"); });
    leftDeck->setStopCallback([this] { leftEngine->stop(); leftDeck->setStatusText("Stopped"); });
    rightDeck->setStopCallback([this] { rightEngine->stop(); rightDeck->setStatusText("Stopped"); });
    leftDeck->setGainCallback([this] (float gain) { leftEngine->setGain(gain); });
    rightDeck->setGainCallback([this] (float gain) { rightEngine->setGain(gain); });
    startTimerHz(18);
}

MainComponent::~MainComponent() = default;

void MainComponent::openTrackForDeck(AudioEngine& engine, const juce::String& deckName, DeckComponent& deck)
{
    juce::FileChooser chooser("Select a track for " + deckName, {}, "*.wav;*.aiff;*.mp3;*.flac;*.ogg");
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult(); engine.loadFile(file);
        deck.setTrackText(engine.getTrackName()); deck.setStatusText("Ready");
        statusLabel.setText("Loaded: " + engine.getTrackName(), juce::dontSendNotification);
    }
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff0d1220));
    g.setGradientFill(juce::ColourGradient(juce::Colour(0xff101a2e), 0, 0, juce::Colour(0xff1d2538), static_cast<float>(getWidth()), static_cast<float>(getHeight()), false));
    g.fillRect(getLocalBounds().toFloat());
    g.setColour(juce::Colour(0xffff7b7b).withAlpha(0.20f)); g.fillEllipse(glowArea);
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds().reduced(30); auto top = bounds.removeFromTop(90);
    titleLabel.setBounds(top.removeFromLeft(340)); statusLabel.setBounds(top.removeFromRight(360));
    auto deckArea = bounds.reduced(10); auto left = deckArea.removeFromLeft(deckArea.getWidth() * 0.48f);
    leftDeck->setBounds(left.reduced(10)); rightDeck->setBounds(deckArea.reduced(10));
    glowArea = juce::Rectangle<float>(getWidth() * 0.35f, getHeight() * 0.18f, getWidth() * 0.28f, getHeight() * 0.42f);
}

void MainComponent::timerCallback()
{
    pulse = (pulse + 1) % 360;
    repaint();
}
