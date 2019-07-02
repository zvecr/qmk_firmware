Bluepill CS32 clones
====================

There are also clone STM32 bluepills on the market with the part
number `CS32F103C8T6`. They are very cheap, usually less than 2usd.
They work almost identically to the STM32 F1s they are meant to
replace, even down to the wrong resistor value on the USB+ line.
It is possible to run QMK on these, but it requires a bit more work.
Assuming you have an STLink and some technical aptitude.

First you will need to download a copy of texane/stlink. stlink is
the premier open source tooling for the stlink programmer, but they
do not support the CS32 clone officially. The clone identifies
itself with a different core id (0x2ba01477), so the stlink software
refuses to flash it. Unfortunately they used the core id from a
genuine STM32 F3 product! So while it is actually a STM32 F1
compatible, it doesn't look like one to the stlink software.

  1. git clone [texane/stlink](https://github.com/texane/stlink/issues/756)
  1. checkout of this specific commit `git checkout b9c315d990abfde3008a917e767c63d2c1c1ddf2`
  1. rebuild stlink using `make`
  1. download and install openocd
  1. unlock bootloader on fake stm32 using openocd - use instructions from [cannonkeys](https://docs.cannonkeys.com/flashing/) with one minor modification: `openocd -f interface/stlink-v2.cfg -f target/stm32f3x.cfg -c "init; reset halt; stm32f1x unlock 0; reset halt; exit"`. This will fool the openocd software into talking to your chip.
  1. unplug/replug the fake bluepill to reboot it
  1. flash on the stm32duino bootloader from maple using the modified st-flash command you built using the instructions from cannonkeys. You will find it in `build\Release` in the stlink repo. It should succeed now.
  1. unplug/replug the fake bluepill to reboot it
  1. add a 1.8K Ohm resistor between 3.3v and A12 to get reliable USB operation
  1. now the board will come up in DFU mode and you will see a maple device at address `0x1eaf::0x0003`
  1. now you can flash it using QMK `qmk handwired/onekey/bluepill:dfu-util`
  1. unplug/replug the fake bluepill to reboot it
  1. profit! You should now have a working onekey keyboard. Short pins `b0` and `a7` to show an `a` character.

Congratulations, now you have a working stm32 clone running a dfu-util compatible bootloader and QMK. From now on you only need to do the dfu-util step.
