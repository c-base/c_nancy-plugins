#include "midiplayerplugin.h"

bool UccncPlugin::create() {
  return UccncPlugin::_create<MidiPlayer>();
}

MidiPlayer::MidiPlayer() : UccncPlugin(AUTHOR, PLUGIN_NAME, PLUGIN_VERSION) {
  trace();
}

MidiPlayer::~MidiPlayer() {
  trace();
}

void MidiPlayer::onFirstCycle() {
  trace();

  auto onNoteOn = [](int32_t track, int32_t tick, int32_t channel, int32_t note, int32_t velocity) -> void {
    dbg("NoteOn: [%i] %d\n", channel, note);
  };

  auto onNoteOff = [](int32_t track, int32_t tick, int32_t channel, int32_t note) -> void {
    dbg("NoteOff: [%i] %d\n", channel, note);
  };

  auto onMetaTrackNameEvent = [](int32_t track, int32_t tick, char *pText) -> void {
    dbg("onMetaTrackNameEvent: %s\n", pText);
  };

  auto onMetaTextEvent = [](int32_t track, int32_t tick, char* pText) -> void {
    dbg("onMetaTextEvent: %s\n", pText);
  };

  auto onMetaCopyrightEvent = [](int32_t track, int32_t tick, char* pText) -> void {
    dbg("onMetaTextEvent: %s\n", pText);
  };

  auto onEndOfSequenceEvent = [](int32_t track, int32_t tick) -> void {
    dbg("onEndOfSequenceEvent: [%i]\n", track);
  };

  MidiPlayerCallbacks_t callbacks = {0};
  callbacks.pOnNoteOnCb          = onNoteOn;
  callbacks.pOnNoteOffCb         = onNoteOff;
  callbacks.pOnMetaTextEventCb   = onMetaTextEvent;
  callbacks.pOnMetaCopyrightCb   = onMetaCopyrightEvent;
  callbacks.pOnMetaTrackNameCb   = onMetaTrackNameEvent;
  callbacks.pOnMetaEndSequenceCb = onEndOfSequenceEvent;

  midiplayer_init(&mpl_, callbacks);

  char pExePath[256];

  GetModuleFileName(NULL, pExePath, sizeof(pExePath));
  string exePath = pExePath;
  string midiPath = exePath.substr(0, exePath.find_last_of("\\") + 1);
  midiPath += "Plugins\\cpp\\";
  midiPath += "midi.mid";

  if (!playMidiFile(&mpl_, midiPath.c_str()))
    dbg("Failed opening midi file!\n");
  else
    dbg("Midi file opened successfully!\n");
}

void MidiPlayer::onTick() {
  // trace();

  midiPlayerTick(&mpl_);
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
