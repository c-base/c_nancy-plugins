#include "midiplayerplugin.h"

UccncPlugin* UccncPlugin::create() {
  return new MidiPlayer();
}

MidiPlayer::MidiPlayer() : UccncPlugin(AUTHOR, PLUGIN_NAME, PLUGIN_VERSION) {
  trace();

}

MidiPlayer::~MidiPlayer() {
  trace();
}

void MidiPlayer::onFirstCycle() {
  trace();
}

void MidiPlayer::onTick() {
  // trace();

}

void MidiPlayer::onShutdown() {
  trace();
}

void MidiPlayer::buttonPressEvent(UccncButton button, bool onScreen) {
  trace();

  if (onScreen) {
    if (button == UccncButton::Cyclestart) {
      // TODO: implement

    }
  }
}

void MidiPlayer::textFieldClickEvent(UccncField label, bool isMainScreen) {
  const char* pLabel;

  switch (label) {
    case UccncField::Mdi:
      pLabel = "Mdi";
      break;

    case UccncField::Setnextlinefield:
      pLabel = "Setnextlinefield";
      break;

    default:
      pLabel = "Unknown";
  }

  dbg("textFieldClickEvent; label=%s, isMainScreen=%d\n", pLabel, isMainScreen);
}

void MidiPlayer::textFieldTextTypedEvent(UccncField label, bool isMainScreen, const char* pText) {
  trace();

  if (isMainScreen) {
    if (label == UccncField::Mdi) {
      // TODO: implement
    }
  }
}
