# IOT

## Prerequisites
* Python 3 ([Download here](https://www.python.org/downloads/))
* Platform.io dependencies ([Installation tutorial](https://docs.platformio.org/en/stable/core/installation/shell-commands.html))
* On Linux platform.io udev rules ([Installation tutorial](https://docs.platformio.org/en/stable/core/installation/udev-rules.html))

## Installation
1. Clone the project and open terminal in it's directory (Suggested: `git clone --recurse-submodules -j8 https://github.com/SEP4-VIA-2023/IOT.git`)
2. Run `python3 get-platformio.py` in the project's directory to install platformio on your device
3. Run `pio run` to compile the code
4. Run `pio run --t upload` to compile and upload the project to the device