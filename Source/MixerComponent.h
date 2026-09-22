#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize(1280, 820);
    setOpaque(true);

    titleLabel.setText("Happy Disco DJ", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(32.0f, juce::Font::bold));
    addAndMakeVisible(titleLabel);

    statusLabel.setText("Live session / 2 decks ready", juce::dontSendNotification);
    addAndMakeVisible(statusLabel);

    sessionButton.setButtonText("Session");
    libraryButton.setButtonText("Library");
    mixerButton.setButtonText("Mixer");
    settingsButton.setButtonText("Settings");
    addAndMakeVisible(sessionButton);
    addAndMakeVisible(libraryButton);
    addAndMakeVisible(mixerButton);
    addAndMakeVisible(settingsButton);

    leftEngine = std::make_unique<AudioEngine>();
    rightEngine = std::make_unique<AudioEngine>();
    leftDeck = std::make_unique<DeckComponent>("Deck A");
    rightDeck = std::make_unique<DeckComponent>("Deck B");
    mixer = std::make_unique<MixerComponent>();

    addAndMakeVisible(leftDeck.get());
    addAndMakeVisible(rightDeck.get());
    addAndMakeVisible(mixer.get());

    leftDeck->setLoadCallback([this] { openTrackForDeck(*leftEngine, "Deck A", *leftDeck); });
    rightDeck->setLoadCallback([this] { openTrackForDeck(*rightEngine, "Deck B", *rightDeck); });

    leftDeck->setPlayCallback([this]
    {
        if (leftEngine->isPlaying())
            return;

        leftEngine->play();
        leftDeck->setStatusText("Playing");
        statusLabel.setText("Deck A playing", juce::dontSendNotification);
    });

    rightDeck->setPlayCallback([this]
    {
        if (rightEngine->isPlaying())
            return;

        rightEngine->play();
        rightDeck->setStatusText("Playing");
        statusLabel.setText("Deck B playing", juce::dontSendNotification);
    });

    leftDeck->setStopCallback([this]
    {
        leftEngine->stop();
        leftDeck->setStatusText("Stopped");
        statusLabel.setText("Deck A stopped", juce::dontSendNotification);
    });

    rightDeck->setStopCallback([this]
    {
        rightEngine->stop();
        rightDeck->setStatusText("Stopped");
        statusLabel.setText("Deck B stopped", juce::dontSendNotification);
    });

    leftDeck->setGainCallback([this](float gain)
    {
        const auto deckGain = juce::jlimit(0.0f, 1.0f, gain);
        leftEngine->setGain(deckGain);
    });

    rightDeck->setGainCallback([this](float gain)
    {
        const auto deckGain = juce::jlimit(0.0f, 1.0f, gain);
        rightEngine->setGain(deckGain);
    });

    leftDeck->setSeekCallback([this](double value)
    {
        const auto length = leftEngine->getLengthInSeconds();
        if (length > 0.0)
            leftEngine->setPosition(value * length);
    });

    rightDeck->setSeekCallback([this](double value)
    {
        const auto length = rightEngine->getLengthInSeconds();
        if (length > 0.0)
            rightEngine->setPosition(value * length);
    });

    mixer->setCrossfadeCallback([this](float value)
    {
        crossfadeValue = value;
        updateCrossfade(value);
    });

    mixer->setMasterGainCallback([this](float value)
    {
        masterLevel = juce::jlimit(0.0f, 1.0f, value);
        updateMasterGain(masterLevel);
    });

    startTimerHz(30);
}

MainComponent::~MainComponent() = default;

void MainComponent::openTrackForDeck(AudioEngine& engine,
                                     const juce::String& deckName,
                                     DeckComponent& deck)
{
    juce::FileChooser chooser(
        "Select a track for " + deckName,
        juce::File::getSpecialLocation(juce::File::userHomeDirectory),
        "*.wav;*.aiff;*.mp3;*.flac;*.ogg;*.m4a;*.aac");

    if (chooser.browseForFileToOpen())
    {
        const auto file = chooser.getResult();

        if (! file.existsAsFile())
        {
            statusLabel.setText("Invalid file selected", juce::dontSendNotification);
            return;
        }

        engine.loadFile(file);

        if (engine.getTrackName().isEmpty() || engine.getTrackName() == "Unsupported file format")
        {
            deck.setStatusText("Unsupported format");
            statusLabel.setText("Track could not be loaded", juce::dontSendNotification);
            return;
        }

        deck.setTrackText(engine.getTrackName());
        deck.setStatusText("Ready");
        statusLabel.setText("Loaded: " + engine.getTrackName(), juce::dontSendNotification);
    }
}

void MainComponent::updateCrossfade(float value)
{
    const auto clamped = juce::jlimit(-1.0f, 1.0f, value);

    const auto leftGain = juce::jlimit(0.0f, 1.0f,
        (0.5f - (clamped * 0.5f)) * masterLevel);

    const auto rightGain = juce::jlimit(0.0f, 1.0f,
        (0.5f + (clamped * 0.5f)) * masterLevel);

    leftEngine->setGain(leftGain);
    rightEngine->setGain(rightGain);
}

void MainComponent::updateMasterGain(float value)
{
    updateCrossfade(crossfadeValue);
    leftEngine->setGain(juce::jlimit(0.0f, 1.0f, value));
    rightEngine->setGain(juce::jlimit(0.0f, 1.0f, value));
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff0d1220));
    g.setGradientFill(juce::ColourGradient(
        juce::Colour(0xff101a2e), 0.0f, 0.0f,
        juce::Colour(0xff1d2538), static_cast<float>(getWidth()),
        static_cast<float>(getHeight()), false));
    g.fillRect(getLocalBounds().toFloat());

    g.setColour(juce::Colour(0xffff7b7b).withAlpha(0.16f));
    g.fillEllipse(glowArea);
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds().reduced(24);
    auto top = bounds.removeFromTop(76);

    titleLabel.setBounds(top.removeFromLeft(300).reduced(8));
    statusLabel.setBounds(top.removeFromRight(300).reduced(8));

    auto buttons = top;
    settingsButton.setBounds(buttons.removeFromRight(100).reduced(5, 12));
    mixerButton.setBounds(buttons.removeFromRight(100).reduced(5, 12));
    libraryButton.setBounds(buttons.removeFromRight(100).reduced(5, 12));
    sessionButton.setBounds(buttons.removeFromRight(100).reduced(5, 12));

    auto content = bounds.reduced(8);
    const auto mixerWidth = juce::jlimit(150, 230, content.getWidth() / 6);
    auto leftArea = content.removeFromLeft((content.getWidth() - mixerWidth) / 2);
    auto mixerArea = content.removeFromLeft(mixerWidth);

    leftDeck->setBounds(leftArea.reduced(8));
    mixer->setBounds(mixerArea.reduced(8));
    rightDeck->setBounds(content.reduced(8));

    glowArea = juce::Rectangle<float>(
        getWidth() * 0.38f, getHeight() * 0.22f,
        getWidth() * 0.24f, getHeight() * 0.35f);
}

void MainComponent::timerCallback()
{
    leftDeck->setPlayback(leftEngine->getCurrentPosition(),
                          leftEngine->getLengthInSeconds(),
                          leftEngine->getOutputLevel());

    rightDeck->setPlayback(rightEngine->getCurrentPosition(),
                           rightEngine->getLengthInSeconds(),
                           rightEngine->getOutputLevel());

    repaint();
}
