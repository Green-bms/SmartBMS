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

By “Green BMS” Android app (available on [Play Store] (https://play.google.com/store/apps/details?id=appinventor.ai_sergio_ghirardelli75.Green_bms_0_0&pcampaignid=pcampaignidMKT-Other-global-all-co-prtnr-py-PartBadge-Mar2515-1)) it is possible to check the Battery Pack status (total voltage, current load, state of charge…) , to check the actual voltage and temperature values of each cell and to perform several settings.
