# TCS3200-ColorSensor

TCS3200 - Color Sensor

# Colorino

## How to start

If you want to start with a Debug Interface model for calibration i recommend to follow this steps.

- Install [ArduinoMenu library](https://github.com/neu-rah/ArduinoMenu) from Library Manager.
- Install [Ticker library](https://github.com/sstaub/Ticker) from Library Manager.
- Install this library from command line in Arduino/libraries folder.

```bash
git clone https://github.com/blascarr/TCS3200.git
git submodule init
git submodule update
```

### Open TCS3200Menu.ino Example

This example is a Debug menu model in order to calibrate and test **TCS3200** properly to start with Color sensor properly.

First of all, you need to calibrate dark and white raw color from **CALIBRATE SENSOR→DARK CALIBRATION** and **CALIBRATE SENSOR→WHITE CALIBRATION**. It will save this color reference in EEPROM.

Once we have Raw Reference for Black and White saved in Arduino, we should calibrate different Colors from **CALIBRATE SENSOR→MANUAL CALIBRATION**.

### **TCS3200 Description**

TCS3200 is a library for color detection with TCS3200 module.

The main objective is improve the performance detection of differente range colors for simple robotic applications.

- [x] Compute time event intervals to read input signal for non-blocking systems.
- [x] Mapping in different range values for colors.
- [x] EEPROM memory for save some calibration data and color list.
- [ ] Assign interruption pin to the INPUT signal to calculate samples.
- [ ] New branch for hardware frequency Tool measurement with [freqCount library](https://www.pjrc.com/teensy/td_libs_FreqCount.html).
- [ ] Tests in platformio.

### Reference Notes

This library is a modification of MD_TCS230 created by Marco Colli

    https://github.com/MajicDesigns/MD_TCS230

Blascarr invests time and resources providing this open source code like some other libraries, please
respect the job and support open-source software.
