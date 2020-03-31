
## Arduino_DY_IRDaikin ARC/BRC

 Arduino_DY_IRDaikin ARC/BRC


 Start:Sep, 2014

 Copyright 2017 danny

 Orignal name is Arduino_IRremote_Daikin, change to this name now.

 Arduino_DY_IRDaikin ARC/BRC library base from Ken Shirriff's IRremote library.

 Arduino_DY_IRDaikin ARC/BRC is simply to send ir command to control daikin air conditioning ARC/BRC.

### Arduino UNO
 ![](https://raw.githubusercontent.com/danny-source/Arduino_IRremote_Daikin/master/Arduino_UNO_IR_Transmit.png)

### Ameba Arduino

 ![](https://raw.githubusercontent.com/danny-source/Arduino_IRremote_Daikin/master/Arduino_Ameba_IR_Transmit.png)

## Changelog:

1.1.2  fix PWM frequency and refactory DYIRDaikinRecv class

1.1.1  fix soft ir frequency and wave width problem

1.1.0  change class and function name,support STM32Duino/Arduino ESP8266/Ameba and  Arduino PWM IR Signal,Arduino UNO/Nano/mega 2560 support SOFT IR

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

Default is pin 3 when you use `begin()`.Remeber! SOFT IR must assign pin by `begin(int irSendPin)`

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

### STM32F1

  - pin 2:PA2
  - pin 3:PA3

## Setting function Daikin ARC:

- on();//turn on AC
- off();//turn off AC
- setSwing_on();//turn on swing
- setSwing_off();//turn off swing
- setMode(int mode);//0=FAN, 1=COOL, 2=DRY, 3=HEAT(if you have one)
- setFan(int speed);// 0~4=speed(1,2,3,4,5),5=auto,6=moon
- setTemp(int temp);//18 ~ 32 Celsius,if you using Fahrenheit ,maybe to enter Fahrenheit.
- sendCommand();
- decode();//decode ir
- description();//print switch state

## Setting function Daikin BRC:

- on();//turn on AC
- off();//turn off AC
- setSwing_on();//turn on swing
- setSwing_off();//turn off swing
- setFan(int speed);// 0.Low 1.High
- setTemp(int temp);//18 ~ 36 Celsius,if you using Fahrenheit ,maybe to enter Fahrenheit.
- sendCommand();

 ## Execute function:

- sendCommand();

  You must initial setting by your programming and,
  After Setting execute sendCommand()

## Example Daikin ARC:

```
 begin();
 on();
 setSwing_off();
 setMode(1)
 setFan(4);//FAN speed to MAX
 setTemp(25);
 //----everything is ok and to execute send command-----
 description();
 sendCommand();
```

## Thanks

`Kwok Fai Fong` help me build Daikin BRC remote control support.

## Donate

[
  ![Donate with PayPal]
  (https://raw.githubusercontent.com/stefan-niedermann/paypal-donate-button/master/paypal-donate-button.png)
]
(https://www.paypal.me/dannytwdanny)
