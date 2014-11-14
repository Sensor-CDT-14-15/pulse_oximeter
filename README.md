# Arduino pulse oximeter
This repository contains an Arduino sketch for controlling a red and IR LED and reading from a photodiode for use as a pulse oximeter, and a Python script for reading the messages output over the serial connection.

There is now also another Python script for plotting data from the Arduino in real time, ``real-time-plot``.


## Setting up the Arduino
First, in order for the sketch to work, you will need to install Simon Monk's Timer library, which can be found [here](http://playground.arduino.cc/Code/Timer).

Out-of-the-box, the Arduino sketch ``pulse_oximeter.ino`` assumes that the red LED is connected to pin 12, the IR LED is connected to pin 7, and the photodiode is connected to pin A0.
This can be changed by editing the following declarations at the top of the sketch:
```c
const int RED_LED_PIN=12;
const int IR_LED_PIN=7;
const int PHOTODIODE_PIN=A0;
```

Also, the flash times are set to be rather long so that it is easy to check by eye that the LEDs are behaving as expected.
Once you are sure that they are, you should change the following declarations to make the flash time and interflash delay much smaller (note that the durations are in milliseconds).
```c
const int FLASH_TIME=500;
const int INTERFLASH_DELAY=500;
```


## Serial communications
The Arduino will send messages over the serial connection every time it changes the state of an LED, or takes a reading from the photodiode (which is reported simply as an ADC level).
Each message has the format ``$time $object $level``, where the time is in milliseconds, and so a set of messages may look like:
```
1000 RED ON
1400 PHOTODIODE 47
1500 RED OFF
1800 PHOTODIODE 3
2000 IR ON
2200 PHOTODIODE 82
2500 IR OFF
2600 PHOTODIODE 2
```
These messages can easily be parsed and logged using the ``read_serial.py`` script detailed below.


## Logging with Python
### Setting up the serial connection
Before the ``read_serial.py`` script will work, you will need to edit the line
```python
f = serial.Serial('/dev/tty.usbmodem1411', 57600)
```
so that ``/dev/tty.usbmodem1411`` points to the serial connection to your Arduino.
On Windows this is likely to be ``COM3``, while Linux/Mac OS X/BSD will probably have something of the form ``/dev/tty.usbmodem`` followed by some numbers which will change depending on which USB port you plug your Arduino into.
The easiest way to find the correct setting is to use the Arduino IDE to upload a program and see what port is listed in the bottom right of the application window.

### Logging data
When ``read_serial.py`` is run it will connect to the Arduino and echo its serial output to the terminal.
At the same time, it will process the output and save the time of each reading, and the reading level to three tab-separated text files, ``red.dat``, ``ir.dat`` and ``photodiode.dat``.
These, as may be expected from the filenames, will only contain the readings pertaining to that particular object, making later analysis nice and easy.

To stop the script and finish writing to the data files, simply send a ``SIGINT`` signal to it (e.g. by pressing ``Ctrl + C`` in the terminal).

**NOTE: Out-of-the-box, the script will overwrite any previous data files each time it is run!**
