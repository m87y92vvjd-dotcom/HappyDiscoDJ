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
    leftDeck->setPlayCallback([this] { leftEngine->play(); leftDeck->setStatusText("Playing"); });
    rightDeck->setPlayCallback([this] { rightEngine->play(); rightDeck->setStatusText("Playing"); });
    leftDeck->setStopCallback([this] { leftEngine->stop(); leftDeck->setStatusText("Stopped"); });
    rightDeck->setStopCallback([this] { rightEngine->stop(); rightDeck->setStatusText("Stopped"); });
    leftDeck->setGainCallback([this] (float gain) { leftEngine->setGain(gain); });
    rightDeck->setGainCallback([this] (float gain) { rightEngine->setGain(gain); });

    mixer->setCrossfadeCallback([this] (float value) { updateCrossfade(value); });
    mixer->setMasterGainCallback([this] (float value)
    {
        leftEngine->setGain(value);
        rightEngine->setGain(value);
    });

    startTimerHz(18);
}

MainComponent::~MainComponent() = default;

void MainComponent::openTrackForDeck(AudioEngine& engine, const juce::String& deckName, DeckComponent& deck)
{
    juce::FileChooser chooser("Select a track for " + deckName, {}, "*.wav;*.aiff;*.mp3;*.flac;*.ogg");
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        engine.loadFile(file);
        deck.setTrackText(engine.getTrackName());
        deck.setStatusText("Ready");
        statusLabel.setText("Loaded: " + engine.getTrackName(), juce::dontSendNotification);
    }
}

void MainComponent::updateCrossfade(float value)
{
    const auto left = juce::jlimit(0.0f, 1.0f, 0.5f - value * 0.5f);
    const auto right = juce::jlimit(0.0f, 1.0f, 0.5f + value * 0.5f);
    leftEngine->setGain(left);
    rightEngine->setGain(right);
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff0d1220));
    g.setGradientFill(juce::ColourGradient(juce::Colour(0xff101a2e), 0, 0,
                                             juce::Colour(0xff1d2538),
                                             static_cast<float>(getWidth()),
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
    auto rightArea = content;

    leftDeck->setBounds(leftArea.reduced(8));
    mixer->setBounds(mixerArea.reduced(8));
    rightDeck->setBounds(rightArea.reduced(8));

    glowArea = juce::Rectangle<float>(getWidth() * 0.38f, getHeight() * 0.22f,
                                      getWidth() * 0.24f, getHeight() * 0.35f);
}

void MainComponent::timerCallback()
{
    pulse = (pulse + 1) % 360;
    repaint();
}
