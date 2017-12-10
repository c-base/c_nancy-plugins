#ifndef _MIDIPLAYER_H
#define _MIDIPLAYER_H

#include "uccncplugin.h"

extern "C" {
  #include "embedded-midilib/midiplayer.h"
}

constexpr const char* AUTHOR =         "coon@c-base.org";
constexpr const char* PLUGIN_NAME =    "midiplayer (c++)";
constexpr const char* PLUGIN_VERSION = "10.12.17";

class MidiPlayer : public UccncPlugin {
public:
  MidiPlayer();
  ~MidiPlayer();

  virtual void onFirstCycle() final override;
  virtual void onTick() final override;
  virtual void onShutdown() final override;
  virtual void buttonPressEvent(UccncButton button, bool onScreen) final override;
  virtual void textFieldClickEvent(UccncField label, bool isMainScreen) final override;
  virtual void textFieldTextTypedEvent(UccncField label, bool isMainScreen, const char* pText) final override;

private:
  MIDI_PLAYER mpl_{0};
};

#endif // _MIDIPLAYER_H
