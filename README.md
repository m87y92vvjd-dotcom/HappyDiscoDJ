#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize(1280, 760);
    setOpaque(true);

    titleLabel.setText("Happy Disco DJ", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(32.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(titleLabel);

    statusLabel.setText("Live session / 2 decks ready", juce::dontSendNotification);
    statusLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(statusLabel);

    sessionButton.setButtonText("Session");
    sessionButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff1f7a8c));
    addAndMakeVisible(sessionButton);

    libraryButton.setButtonText("Library");
    addAndMakeVisible(libraryButton);

    mixerButton.setButtonText("Mixer");
    mixerButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xfff58e09));
    addAndMakeVisible(mixerButton);

    settingsButton.setButtonText("Settings");
    addAndMakeVisible(settingsButton);

    leftEngine = std::make_unique<AudioEngine>();
    rightEngine = std::make_unique<AudioEngine>();

    leftDeck = std::make_unique<DeckComponent>("Deck A");
    rightDeck = std::make_unique<DeckComponent>("Deck B");

    leftDeck->setLoadCallback([this] { openTrackForDeck(*leftEngine, "Deck A", *leftDeck); });
    rightDeck->setLoadCallback([this] { openTrackForDeck(*rightEngine, "Deck B", *rightDeck); });

    leftDeck->setPlayCallback([this] { leftEngine->play(); leftDeck->setStatusText("Playing"); });
    rightDeck->setPlayCallback([this] { rightEngine->play(); rightDeck->setStatusText("Playing"); });

    leftDeck->setStopCallback([this] { leftEngine->stop(); leftDeck->setStatusText("Stopped"); });
    rightDeck->setStopCallback([this] { rightEngine->stop(); rightDeck->setStatusText("Stopped"); });

    addAndMakeVisible(leftDeck.get());
    addAndMakeVisible(rightDeck.get());

    startTimerHz(18);
}

MainComponent::~MainComponent() = default;

void MainComponent::openTrackForDeck(AudioEngine& engine, const juce::String& deckName, DeckComponent& deck)
{
    juce::FileChooser chooser("Select a track for " + deckName,
                              juce::File::getSpecialLocation(juce::File::userHomeDirectory),
                              "*.wav;*.aiff;*.mp3;*.flac;*.ogg");

    if (chooser.browseForFileToOpen())
    {
        const auto file = chooser.getResult();
        engine.loadFile(file);
        deck.setTrackText(file.getFileName());
        deck.setStatusText("Ready");
        statusLabel.setText("Loaded track: " + file.getFileName(), juce::dontSendNotification);
    }
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff0d1220));

    g.setGradientFill(juce::ColourGradient(
        juce::Colour(0xff101a2e),
        0.0f, 0.0f,
        juce::Colour(0xff1d2538),
        static_cast<float>(getWidth()), static_cast<float>(getHeight()),
        false));
    g.fillRect(getLocalBounds().toFloat());

    g.setColour(juce::Colour(0xffff7b7b).withAlpha(0.20f));
    g.fillEllipse(glowArea);

    g.setColour(juce::Colours::white.withAlpha(0.94f));
    g.drawText("Night set / Main booth", getLocalBounds().reduced(60, 60), juce::Justification::centredRight, true);
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds().reduced(30);

    auto topBar = bounds.removeFromTop(90);
    titleLabel.setBounds(topBar.removeFromLeft(340).reduced(12, 0));
    statusLabel.setBounds(topBar.removeFromRight(360).reduced(12, 0));

    auto actions = topBar.withTrimmedRight(20);
    sessionButton.setBounds(actions.removeFromRight(120).reduced(10, 16));
    libraryButton.setBounds(actions.removeFromRight(120).reduced(10, 16));
    mixerButton.setBounds(actions.removeFromRight(120).reduced(10, 16));
    settingsButton.setBounds(actions.removeFromRight(120).reduced(10, 16));

    auto deckArea = bounds.reduced(10);
    auto left = deckArea.removeFromLeft(deckArea.getWidth() * 0.48f);
    auto right = deckArea;

    leftDeck->setBounds(left.reduced(10));
    rightDeck->setBounds(right.reduced(10));

    glowArea = juce::Rectangle<float>(
        static_cast<float>(getWidth()) * 0.35f,
        static_cast<float>(getHeight()) * 0.18f,
        static_cast<float>(getWidth()) * 0.28f,
        static_cast<float>(getHeight()) * 0.42f
    );
    glowArea.setCentre(glowArea.getCentreX() + static_cast<float>(pulse) * 0.5f,
                       glowArea.getCentreY());
}

void MainComponent::timerCallback()
{
    pulse = (pulse + 1) % 360;
    repaint();
}
