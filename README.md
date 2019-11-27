# RasPreenFM2

!/RasPreenFM2/RasPreenFM2_test.jpg

BareMetal RasperryPi Port of the PreenFM2 Synthesizer https://github.com/Ixox/preenfm2
included extrafilters from https://github.com/pvig/preenfm2

The PreenFM2-Synth looked really good but i was not happy with the STM32-uController...
I really like the STM32 uControllers and did some Projects with them but i was constantly annoyed with the buggy and rather strange Software-Support of STM (HAL-Layer rather kludgy, CubeMX-Code-Generator produces buggy code, poor support etc, its a pitty because i really liked the STM32F-Disco Kits, and an SPI BUSY Errata that costed me lot of time & hair....).
Then i stumbled over the Circle-Framework for BareMetal Programming of the Raspberry Pi https://github.com/rsta2/minisynth
and ported PreenFM2 with it to Raspi 3B, HiFiBerry DAC+ Audiocard and a MAX6957 for the connection to the encoders and buttons.
(see hardware/Encoders.cpp for details) and got a latency of about < 3ms @ 48kHz

The Port is a really ugly hack, was does not work:
- Bootmode
- usb-midi doesnt work (now...), MIDI is done thru the serial-interface UART0 (GPIO 14/TX Pin8, GPIO 15/RX Pin10 )
- Oled-Timer
- Clock-Led
- probably more :-(

About the code, i wanted to get this quick up and running without changing to much, so i made some evil things...

Its compiled as 64Bit, just put the kernel8.img, bootcode.bin, start.elf & fixup.dat in the root of the sd-card
of your Raspi 3B(+) and off you go!

The best way to build it ist to get https://github.com/rsta2/minisynth and build it to test the framework,
the addon/fats must also be build. I used gcc-arm-8.3-2019.03-x86_64-aarch64-elf/bin/aarch64-elf- as compiler
then copy the raspreen directory instead of the src directory
i had to change line 63 in circle/Rules.mk to

`ARCH    ?= -DAARCH=64 -march=armv8-a+fp+simd -mtune=cortex-a53 -mlittle-endian  -mcmodel=small`

and the flags (line 121) to

`CFLAGS    += $(ARCH) -Wall -fsigned-char -ffreestanding -mstrict-align $(DEFINE) $(INCLUDE) $(OPTIMIZE) -g`

go in the raspreen directory and do make

What i didn't find out was a compatible assembler instruction for

`#define __USAT(ARG1,ARG2) \`
`({                          \`
`  uint32_t __RES, __ARG1 = (ARG1); \`
`  __ASM ("usat %0, %1, %2" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1) ); \`
`  __RES; \`
` })`
 
 
but it worked without :-) (but if somebody could give me a hint it would be great!)

The soundbuffer-calculating async in the main loop didn't work to well, gave glitches while screen
writes, so i put the calculating in the DMA-Irq of the sounddevice.
A strange thing happened then, it crashed in prepareMatrixForNewBlock in the LFO Section
while loading a new patch (during the usb-file operations). A long and fruitless search
why? didnt help so i did a really ugly hack and disabled the prepareMatrixForNewBlock()
while loading patches.

Other things i changed:
- Changed BLOCK_SIZE to 16, gave me first horrible aliasing in the higher notes, changing 
  Osc.h Line 128
    for (int k=0; k<32; ) 
    to
    for (int k=0; k<BLOCK_SIZE; ) {
  seemed to help

- the BPM/LFO/Env/ARP/etc internal temp changed to the smaler BLOCKSIZE

- the Operators start a 0°, didn't like the Click to much at 90° 

- When the same note is struck again it allocates a new voice (if voice > 1) instead cutting off the voice.
  On NoteOff it switches of the longer running voice first.

- The ComboName is memorized on load/save and used as default for saving 

- integrated the extra filters

- changed the glide-times to longer

- some checks reading the settings values and prevent loading DX7-Patches > 31 over MIDI ProgramChange
  that would crash the Raspreen.

known Bugz:
- sometimes it ignores a note, no idea what it could be, will take probably long to figure out, till then
  it goes as "mandatory humanize"-function :-)   

ToDo:
- More Polyphony and Operators :-)
  the RaspberryPi is quite powerful and the load with 4Timbres à 2Voices with 6 Operators + Filters is about 15%
  of the SoundDevice Interrupt so this should be doable when i understand how to do it....

- usb-midi

- sort of bootmode to access the SD-Card or USB-Stick from outside, the RaspberryPi does not have a USB-Device/OTG
  functionality, so probably over ethernet or so, but in the meantime popping them out and putting them in the
  computer to transfer data is not so terrible....    

- nicer GUI
  actually i find the usability very good for the limited amount of Buttons/Encoders and the editor is great,
  and changing this would take quite some time so this is not on the priority-list


Hope that anybody has fun using the synth and many thanks for bug reports!

Many thanks goes out to Xavier Hosxe for the great PreenFM2-Synth and R. Stange for the amazing Circle Framework!

 * THIS IS ALPHA-SOFTWARE! DONT USE IT FOR LIVE PURPOSES! 
 * I AM NOT RESPONSIBLE FOR ANY TRAGEDIES AT GIGS :-)






