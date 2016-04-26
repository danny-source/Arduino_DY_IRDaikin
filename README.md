
## IRdaikin ARC/BRC

 IRdaikin ARC/BRC


 Start:Sep, 2014

 Copyright 2015 danny

 IRdaikin ARC/BRC library base from Ken Shirriff's IRremote library.

 IRdaikin ARC/BRC is simply to send ir command to control daikin air conditioning ARC/BRC.

 ![](https://raw.githubusercontent.com/danny-source/Arduino_IRremote_Daikin/master/Arduino_UNO_IR_Transmit.png)

## Changelog:

1.0.0  Refactory code and ARC support decode ir.
0.0.5  In order to isolate,IRremote renamed IRremoteDaikin and refactory,

## Pinout:

  - pin 3:UNO
  - pin 2:Leonardo
  - pin 9:Mega

## Setting function Daikin ARC:

- daikin_on();//turn on
- daikin_off();//turn off
- daikin_setSwing_on();//turn on swing
- daikin_setSwing_off();//turn off swing
- daikin_setMode(int mode);//0=FAN, 1=COOL, 2=DRY
- daikin_setFan(int speed);// 0~4=speed(1,2,3,4,5),5=auto,6=moon
- daikin_setTemp(int temp);//18 ~ 32 Celsius,if you using Fahrenheit ,maybe to enter Fahrenheit.
- daikin_sendCommand();
- decode();//decode ir
- description();//print switch state

## Setting function Daikin BRC:

- daikin_on();//turn on
- daikin_off();//turn off
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
