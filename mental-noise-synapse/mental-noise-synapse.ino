/*
 *    Mental Noise - Synapse
 *    CV to MIDI Eurorack Module
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
 *    
 *    This program is free for personal use.
 *    No commercial use please!
 */

#include <ResponsiveAnalogRead.h>
#include <MIDI.h>

// All the midi commands are coming from Korg official documentation
// http://i.korg.com/uploads/Support/USA_volcabeats_MIDI_Chart_E.pdf

// By default, the Volca beats uses MIDI channel 10.
// This can be changed by starting the Volca Beats with the memory touch pressed.
#define VB_CHANNEL 10

// MIDI channel of the Arduino, set it to an unused channel in your setup.
#define ARDUINO_CHANNEL 16

// Velocity parameter to send with a note.
#define VELOCITY 127

// Turned on, the module will output informations in the Serial Monitor instead of sending MIDI commands
const bool debug = false;

// Declare the total of CV inputs you are using
const byte numberOfControls = 6;

// Declare the total of gate inputs you are using
const byte numberOfNotes = 10;

// Inputs pins of the CV for the instruments.
const byte gateInputs[numberOfNotes] = {
        2, // Kick
        3, // Snare
        4, // Lo Tom
        5, // Hi Tom
        6, // Cl Hat
        7, // Op Hat
        8, // Claves
        9, // Clap
        10, // Crash
        11, // Agogo
};

// MIDI notes used by the Volca Beats to trigger the instruments.
// Change the notes to fit your needs.
const byte midiNotes[numberOfNotes] = {
        36, // Kick
        38, // Snare
        43, // Lo Tom
        50, // Hi Tom
        42, // Closed Hat
        46, // Open Hat
        75, // Claves
        39, // Clap
        49, // Crash
        67, // Agogo
};

// Default control change value to send when no CV is received.
// Minimum is 0, Maximum is 127, Average is 63
const byte defaultControlValue = 63;

// CV Inputs pins.
ResponsiveAnalogRead ccInputs[numberOfControls] = {
    ResponsiveAnalogRead(A7, true), // Clap PCM Speed
    ResponsiveAnalogRead(A6, true), // Clap PCM Speed
    ResponsiveAnalogRead(A5, true), // Agogo PCM Speed
    ResponsiveAnalogRead(A4, true), // Crash PCM Speed
    ResponsiveAnalogRead(A3, true), // Stutter Time
    ResponsiveAnalogRead(A2, true), // Stutter Depth
};

/*
 * MIDI Control Change values.
 * I used those because I feel like they are the most useful for me to control by CV.
 * You can changes those to control other parameters of your choice.
 * 
 * This list of values is specific to the Korg Volca Beats, you can adapt it to your needs.
 * 40−49 = Part level of the instruments (40 = Kick, 49 = Crash, same order as on the keyboard)
 * 50-53 = PCM Speed (What I'm using here)
 * 54-55 = Stutter Time and Depth
 * 56-58 = Tom, CL Hat and OP Hat decay
 * 59 = Hat grain 
 */
const byte midiControls[numberOfControls] = {
        50, // Clap PCM Speed
        51, // Claves PCM Speed
        52, // Agogo PCM Speed
        53, // Crash PCM Speed
        54, // Stutter Time
        55, // Stutter Depth
};

boolean cvState = LOW;
boolean activeNotes[numberOfNotes];
int activeControls[numberOfControls];
int controlValue = 0;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  if (debug) {
    Serial.begin(9600);
  } else {
    MIDI.begin(ARDUINO_CHANNEL);
  }
}

void loop() {
    handleInstruments();
    handleControls();
}

void handleInstruments() {
    for (byte i = 0; i < numberOfNotes; i++) {
        cvState = digitalRead(gateInputs[i]);

        if (!activeNotes[i] && cvState == HIGH) {
          if (debug) {
            Serial.println("send note ON");
          } else {
            MIDI.sendNoteOn(midiNotes[i], VELOCITY, VB_CHANNEL);
          }

            activeNotes[i] = true;
        } else if (activeNotes[i] && cvState == LOW) {
            if (debug) {
              Serial.println("send note OFF");
            } else {
              MIDI.sendNoteOff(midiNotes[i], VELOCITY, VB_CHANNEL);
            }
            activeNotes[i] = false;
        }
    }
}

void handleControls() {
    for (byte i = 0; i < numberOfControls; i++) {
        ccInputs[i].update();

        if (!ccInputs[i].hasChanged()) {
          continue;
        }
        
        controlValue = map(ccInputs[i].getValue(), 0, 1023, 0, 127);
        
        if (controlValue < 5) {
            controlValue = defaultControlValue;
        }

        if (abs(controlValue - activeControls[i]) > 2) {
            if (debug) {
              Serial.println(controlValue);
            } else {
              MIDI.sendControlChange(midiControls[i], controlValue, VB_CHANNEL);
            }
            activeControls[i] = controlValue;
        }
    }
}
