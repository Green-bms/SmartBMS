# SmartBMS
Open source Smart Battery Management System
https://www.green-bms.com/smart-bms/

## Description
Smart BMS is an Open Source Battery Management System for Lithium Cells (Lifepo4, Li-ion, LTO, NCM, etc.) Battery Pack.
![In a single picture](https://www.green-bms.com/wp-content/uploads/2021/06/Green_bms_functional-768x768.png)

The main functions of BMS are:
- To protect cells against overvoltage
- To protect cells against undervoltage
- To balance the cells 

Smart BMS consists of four main components:
1. Cell Module (https://www.green-bms.com/cell-module/)
2. Control Unit (https://www.green-bms.com/control-unit/)
3. Limiter (https://www.green-bms.com/limiter/)
4. Android App (https://www.green-bms.com/android-app/)

The voltage and the temperature values of each cell are acquired by the relevant Cell Module (based on Attiny microcontroller) and sent to Control Unit (based on Arduino Mega microcontroller) through a serial I2C line.

Control Unit starts or stop charging (by output relay) through Limiter, that includes a power relay for opening or closing the charging circuit.

Control Unit can also start or stop discharging by output relay.

When during charging a cell reaches the “Balance voltage value parameter”, the Cell Module connects the balance resistors in order to perform the cell bleeding.

When just one cell of the pack is balancing, Control Unit activates the current limit function inside the Limiter (the charging current is limited to 1A).

Control Unit can be connected with a Smartphone (or a Tablet) by Bluetooth.

By “Green BMS” Android app (available on Play Store)) it is possible to check the Battery Pack status (total voltage, current load, state of charge…) , to check the actual voltage and temperature values of each cell and to perform several settings.

## Tutorials
The instructions for a correct use of the BMS are enclosed in a series of video tutorials on the Youtube channel:

https://www.youtube.com/channel/UCdJnQnmzJSQNOAGbZR5iExA

Subscribe please... :smile:

- How to make Green BMS (https://www.youtube.com/watch?v=LHrENJF3j-4)
- How to set cell modules address (https://www.youtube.com/watch?v=e1lMxtpzjrA)

## Warning
Lithium and other batteries are dangerous and must be treated with care.
Lithium and other batteries are potentially hazardous and can present a serious FIRE HAZARD if damaged, defective or improperly used.
Green BMS 
