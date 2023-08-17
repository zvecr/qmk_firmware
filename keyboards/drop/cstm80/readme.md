# CSTM80

![CSTM80](TODO)

* Keyboard Maintainer: [zvecr](https://github.com/zvecr)
* Hardware Availability: <TODO>

Make example for this keyboard (after setting up your build environment):

    make drop/cstm80:default

Flashing example for this keyboard:

    make drop/cstm80:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard
* **Physical reset button**: Quickly double tap the button located under the spacebar
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
