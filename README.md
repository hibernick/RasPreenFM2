# RasPreenFM2

https://github.com/styro2000/RasPreenFM2/blob/master/RasPreenFM2_test.jpg

BareMetal RasperryPi Port of the PreenFM2 Synthesizer https://github.com/Ixox/preenfm2
included the extrafilters from https://github.com/pvig/preenfm2

The PreenFM2-Synth looked really good but i was not happy with the STM32-uController...
I really like the STM32 uControllers and did some Projects with them but i was constantly annoyed with the buggy and rather strange Software-Support of STM (CubeMX-Code-Generator produces buggy code, poor support etc, its a pitty because i really liked the STM32F-Disco Kits).
Then i stumbled over the Circle-Framework for BareMetal Programming of the Raspberry Pi https://github.com/rsta2/minisynth
and ported PreenFM2 with it to Raspi 3B, HiFiBerry DAC+ Audiocard and a MAX6957 for the connection to the encoders and buttons.
(see hardware/Encoders.cpp for details) and got a latency of about < 3ms @ 48kHz

The Port is a really ugly hack, was does not work:
- Bootmode
- usb-midi doesnt work (now...), MIDI is done thru the serial-interface UART0
- Oled-Timer
- probably more :-(

About the code, there are probably much better ways to do things...

Its compiled as 64Bit, just put the kernel8.img, bootcode.bin, start.elf & fixup.dat in the root of the sd-card
of your Raspi 3B(+) and off you go!

The best way to build it ist to get https://github.com/rsta2/minisynth and build it to test the framework,
the addon/fats must also be build. I used gcc-arm-8.3-2019.03-x86_64-aarch64-elf/bin/aarch64-elf- as compiler
then copy the raspreen directory instead of the src directory
i had to change line 63 in circle/Rules.mk to
ARCH    ?= -DAARCH=64 -march=armv8-a+fp+simd -mtune=cortex-a53 -mlittle-endian  -mcmodel=small
and the flags (line 121) to
CFLAGS    += $(ARCH) -Wall -fsigned-char -ffreestanding -mstrict-align $(DEFINE) $(INCLUDE) $(OPTIMIZE) -g
and in the raspreen directory do make

What i didn't find out was a compatible assembler instruction for

#define __USAT(ARG1,ARG2) \
({                          \
  uint32_t __RES, __ARG1 = (ARG1); \
  __ASM ("usat %0, %1, %2" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1) ); \
  __RES; \
 })
 
but it worked without :-) (but if somebody could give me a hint it would be great!)

The soundbuffer-calculating in the main loop didn't work to well, gave glitches while screen
writes, so i put the calculating in the DMA-Irq of the sounddevice.
A strange thing happened then, it crashed in prepareMatrixForNewBlock in the LFO Section
while loading a new patch (during the usb-file operations). A long and fruitless search
why? didnt help so i did a really ugly hack and disabled the prepareMatrixForNewBlock
while loading patches.

Hope that anybody has fun using the synth and many thanks for bug reports!

Many thanks goes out to Xavier Hosxe for the great PreenFM2 and R. Stange for the amazing Circle Framework!

 * THIS IS ALPHA-SOFTWARE! DONT USE IT FOR LIVE PURPOSES! 
 * I AM NOT RESPONSIBLE FOR ANY TRAGEDIES AT GIGS :-)






