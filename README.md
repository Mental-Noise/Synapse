# WORK IN PROGRESS

# Korg Volca Beats CV2MIDI

The repository contains the code and schematics for a DIY module converting CV signals to MIDI commands the Volca Beats understands.

## Controls
The CV to MIDI converter gives the following controls over the Volca Beats :
* Trigger of each instrument
* Change of the PCM speed of each PCM instrument independently
* Change of the Stutter Time and Depth
* Clock trigger

## Parts
* 1x Arduino Nano
* 1x 5 pin MIDI connector 
* 24x 1N4001 or equivalent rectifier diodes
* 11x BC547B or equivalent transistors
* 33x 10k resistors
* 11x 1N914 or equivalent diodes
* 17x LED of your preference
* 17x Mono jack sockets

## General informations

The Arduino code uses the standard MIDI library, which can be found in the Arduino Library Manager.

The different controls can easily be changed to some other of your choice. All parameters are described in comments in the code.

The code is written to allow easy customization even for a someone who doesn't know anything about writing code for the Arduino. 