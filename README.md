# Logue Effects 
This repository contains custom synthesizer effects for Korg's logue SDK. 
I only own a Nu:Tekt NTS-1 so that is the only synth that is guaranteed to work, but in theory they should work for the Minilogue XD, Prologue and the NTS-1.

## Effect List
1. *Crusher*: Bitcrush effect for the modfx section
2. *Ring-Sine* Sine wave ring mod effect for the modfx section
2. *Vinyl* Vinyl crackle overlay effect for the modfx section

## Building The Effects (linux/NTS-1)
1. Clone this repository:
    ```bash
    git clone  https://github.com/RobbieFernandez/logue-effects.git
    ```
2. Clone the logue-sdk submodule:
    ```bash
    git submodule update --init
    ```
3. Setup the logue-sdk:
    ```bash
    cd logue-sdk
    git submodule update --init
    cd tools/gcc
    ./get_gcc_linux.sh
    cd ../../../
    ```
4. Run the makefile for the effect
    ```bash
    cd crusher  # Or whichever effect you're building
    make
    ```

You should now have a .ntkdigiunit in the effect's directory which can be loaded into your synth via logue-cli or the korg librarian application.
