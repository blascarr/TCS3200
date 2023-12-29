# TCS3200-ColorSensor

TCS3200 - Color Sensor

# Colorino

## How to start

If you want to start with a Debug Interface model for calibration; I recommend to follow this steps.

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

TCS3200 is a library for color detection with **TCS3200** module.

The main objective is improve the performance detection of differente range colors for simple robotic applications.

- [x] Compute time event intervals to read input signal for non-blocking systems.
- [x] Mapping in different range values for colors.
- [x] EEPROM memory for save some calibration data and color list.
- [x] Support to Neurona classication **Multi Layer Perceptron** by [Morettic](http://www.moretticb.com/Neurona).
- [ ] Assign interruption pin to the INPUT signal to calculate samples.
- [ ] Tests in platformio.


### Download and Print your workspace setup to test
Print these pieces to create this test workspace.
- ArduinoSupportTest_TCS3200.
- TesterUP_TCS3200.
- TesterBottom_TCS3200.

  ![Screenshot from 2023-12-29 16-12-47](https://github.com/blascarr/TCS3200/assets/6161706/df43a128-c655-49dd-a9b0-6599bd008005)


### **Colorino Project**
This library is used in  [Colorino Project](https://github.com/blascarr/Colorino) as a classificator model with Lacasitos.

Blascarr invests time and resources providing this open source code like some other libraries, please
respect the job and support open-source software.
