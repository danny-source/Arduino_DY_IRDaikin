
## Arduino_DY_IRDaikin ARC/BRC

 Arduino_DY_IRDaikin ARC/BRC


 Start:Sep, 2014

 Copyright 2017 danny

 Orignal name is Arduino_IRremote_Daikin, change to this name now.

 Arduino_DY_IRDaikin ARC/BRC library base from Ken Shirriff's IRremote library.

 Arduino_DY_IRDaikin ARC/BRC is simply to send ir command to control daikin air conditioning ARC/BRC.

 ![](https://raw.githubusercontent.com/danny-source/Arduino_IRremote_Daikin/master/Arduino_UNO_IR_Transmit.png)

## Changelog:

1.0.2  Support ESP8266/Ameba Soft and Arduino PWM IR Signal

1.0.1  Support ESP8266 Soft and Arduino PWM IR Signal

1.0.0  Refactory code and ARC support decode ir.

0.0.5  In order to isolate,IRremote renamed IRremoteDaikin and refactory,:

## PWM Pinout

### UNO

  - pin 3:D3

### Leonardo

  - pin 2:D2

### Mega

  - pin 9:D9

## SOFT Pinout

### ESP8266
  - pin 4:GPIO4
  - pin 5:GPIO5

### Ameba RTL8195

  - pin 2:D2
  - pin 3:D3
  - pin 4:D4
  - pin 5:D5
  - pin 6:D6
  - pin 7:D7

### Ameba RTL8710

  - pin 2:D2
  - pin 5:D5

## Setting function Daikin ARC:

- daikin_on();//turn on AC
- daikin_off();//turn off AC
- daikin_setSwing_on();//turn on swing
- daikin_setSwing_off();//turn off swing
- daikin_setMode(int mode);//0=FAN, 1=COOL, 2=DRY, 3=HEAT(if you have one)
- daikin_setFan(int speed);// 0~4=speed(1,2,3,4,5),5=auto,6=moon
- daikin_setTemp(int temp);//18 ~ 32 Celsius,if you using Fahrenheit ,maybe to enter Fahrenheit.
- daikin_sendCommand();
- decode();//decode ir
- description();//print switch state

## Setting function Daikin BRC:

- daikin_on();//turn on AC
- daikin_off();//turn off AC
- daikin_setSwing_on();//turn on swing
- daikin_setSwing_off();//turn off swing
- daikin_setFan(int speed);// 0.Low 1.High
- daikin_setTemp(int temp);//18 ~ 36 Celsius,if you using Fahrenheit ,maybe to enter Fahrenheit.
- daikin_sendCommand();

 ## Execute function:

- daikin_sendCommand();

  You must initial setting by your programming and,
  After Setting execute daikin_sendCommand()

## Example Daikin ARC:

```
 begin();
 daikin_on();
 daikin_setSwing_off();
 daikin_setMode(1)
 daikin_setFan(4);//FAN speed to MAX
 daikin_setTemp(25);
 //----everything is ok and to execute send command-----
 description();
 daikin_sendCommand();
```

## Thanks

`Kwok Fai Fong` help me build Daikin BRC remote control support.
