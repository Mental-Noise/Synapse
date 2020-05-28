/*
 *    Volca Beats CV2MIDI
 *    Copyright (C) 2020 Arthur LAURENT
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *    GNU General Public License <http://www.gnu.org/licenses/> for more details.
 */

#include <MIDI.h>

// All the midi commands are coming from Korg official documentation
// http://i.korg.com/uploads/Support/USA_volcabeats_MIDI_Chart_E.pdf

// By default, the Volca beats uses MIDI channel 10.
// This can be changed by starting the Volca Beats with the memory touch pressed.
#define VB_CHANNEL 10

// MIDI channel of the Arduino, set it to an unused channel in your setup.
#define ARDUINO_CHANNEL 16

// Velocity parameter to send with a note.
// I didn't notice any difference by using different values so I use the max value.
// Feel free to change it.
#define VELOCITY 127

// Clock input pin
#define CLOCK 13

const byte numberOfNotes = 10;
const byte numberOfControls = 6;

// Inputs pins of the CV for the instruments.
const byte cvInputs[numberOfNotes] = {
        3, // Kick
        4, // Snare
        5, // Lo Tom
        6, // Hi Tom
        7, // Cl Hat
        8, // Op Hat
        9, // Clap
        10, // Claves
        11, // Agogo
        12, // Crash
};

// MIDI notes used by the Volca Beats to trigger the instruments.
const byte midiNotes[numberOfNotes] = {
        36, // Kick
        38, // Snare
        43, // Lo Tom
        50, // Hi Tom
        42, // Closed Hat
        46, // Open Hat
        39, // Clap
        75, // Claves
        67, // Agogo
        49, // Crash
};

// Default control change value to send when no CV is received.
// Minimum is 0, Maximum is 127, Average is 63
const byte defaultControlValue = 63;

// Inputs pins of the CV for some of the Volca Beats parameters.
const byte ccInputs[numberOfControls] = {
        A0, // Clap PCM Speed
        A1, // Claves PCM Speed
        A2, // Agogo PCM Speed
        A3, // Crash PCM Speed
        A4, // Stutter Time
        A5, // Stutter Depth
};

// MIDI Control Change values used by the Volca Beats to set a specific setting.
// I used those because I feel like they are the most useful for me to control by CV.
// You can changes those to control other parameters of your choice.
// 40−49 = Part level of the instruments (40 = Kick, 49 = Crash, same order as on the keyboard)
// 50-53 = PCM Speed (What I'm using here)
// 54-55 = Stutter Time and Depth
// 56-58 = Tom, CL Hat and OP Hat decay
// 59 = Hat grain
const byte midiControls[numberOfControls] = {
        50, // Clap PCM Speed
        51, // Claves PCM Speed
        52, // Agogo PCM Speed
        53, // Crash PCM Speed
        54, // Stutter Time
        55, // Stutter Depth
};

boolean cvState = LOW;
boolean clockState = LOW;
boolean activeClock = false;
boolean activeNotes[numberOfNotes];
int activeControls[numberOfControls];
int controlValue = 0;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
    MIDI.begin(ARDUINO_CHANNEL);
}

void loop() {
    handleInstruments();
    handleControls();
    handleClock();
}

void handleInstruments() {
    for (byte i = 0; i < numberOfNotes; i++) {
        cvState = digitalRead(cvInputs[i]);

        if (!activeNotes[i] && cvState == HIGH) {
            MIDI.sendNoteOn(midiNotes[i], VELOCITY, VB_CHANNEL);
            activeNotes[i] = true;
        } else if (activeNotes[i] && cvState == LOW) {
            MIDI.sendNoteOff(midiNotes[i], VELOCITY, VB_CHANNEL);
            activeNotes[i] = false;
        }
    }
}

void handleControls() {
    for (byte i = 0; i < numberOfControls; i++) {
        controlValue = analogRead(ccInputs[i]);

        if (controlValue == 0) {
            controlValue = defaultControlValue;
        }

        if (controlValue != activeControls[i]) {
            MIDI.sendControlChange(midiControls[i], controlValue, VB_CHANNEL);
            activeControls[i] = controlValue;
        }
    }
}

void handleClock() {
    clockState = digitalRead(CLOCK);

    if (!activeClock && clockState == HIGH) {
        MIDI.sendClock();
        activeClock = true;
    } else if (activeClock && clockState == LOW) {
        activeClock = false;
    }
}