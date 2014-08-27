
## IRdaikin

 IRdaikin

 Version:0.0.1

 Aug, 2014

 Copyright 2014 danny

 IRremote library base from Ken Shirriff's IRremote library and add daikin IR function.
 IRdaikin is using custom IRremote library to simply to send daikin air conditioning ir command.

 ![](https://raw.githubusercontent.com/danny-source/Arduino_IRremote_Daikin/master/Arduino_UNO_IR_Transmit.png)

## Pinout:

  - pin 3:UNO
  - pin 2:Leonardo
  - pin 9:Mega

## Setting function:

- daikin_on();//turn on
- daikin_off();//turn off
- daikin_setSwing_on();//turn on swing
- daikin_setSwing_off();//turn off swing
- daikin_setMode(int mode);//0=FAN, 1=COOL, 2=DRY
- daikin_setFan(int speed);// 0~4=speed(1,2,3,4,5),5=auto,6=moon
- daikin_setTemp(int temp);//23 ~ 33 Celsius,if you using Fahrenheit ,maybe to enter Fahrenheit.
- daikin_sendCommand();

 ## Execute function:

- daikin_sendCommand();

  You must initial setting by your programming and,
  After Setting execute daikin_sendCommand()

## Example:

```
 daikin_on();
 daikin_setSwing_off();
 daikin_setMode(1)
 daikin_setFan(4);//FAN speed to MAX
 daikin_setTemp(25);
 //----everything is ok and to execute send command-----
 daikin_sendCommand();
```