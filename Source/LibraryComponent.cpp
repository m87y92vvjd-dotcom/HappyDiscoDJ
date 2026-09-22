#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class LibraryComponent : public juce::Component
{
public:
    LibraryComponent();
    ~LibraryComponent() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTrackList(const juce::StringArray& tracks);

private:
    juce::Label titleLabel;
    juce::Label trackListLabel;
};
