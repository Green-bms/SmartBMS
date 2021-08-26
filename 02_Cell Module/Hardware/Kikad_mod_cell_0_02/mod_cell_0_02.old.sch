EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Cell module"
Date ""
Rev "0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	3850 3750 4400 3750
Wire Wire Line
	3850 3900 3850 3750
Wire Wire Line
	4000 3800 4000 3950
Wire Wire Line
	4000 3800 3650 3800
Wire Wire Line
	3850 3900 3650 3900
Wire Wire Line
	6800 3550 7650 3550
Wire Wire Line
	6800 2700 6800 3550
Wire Wire Line
	6200 2700 6800 2700
Wire Wire Line
	7200 3450 7650 3450
Wire Wire Line
	7200 1750 7200 3450
$Comp
L dk_Temperature-Sensors-Analog-and-Digital-Output:TMP36GT9Z U1
U 1 1 5F06272E
P 5800 2700
F 0 "U1" H 5673 2753 60  0000 R CNN
F 1 "TMP36GT9Z" H 5673 2647 60  0000 R CNN
F 2 "digikey-footprints:TO-92-3" H 6000 2900 60  0001 L CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/TMP35_36_37.pdf" H 6000 3000 60  0001 L CNN
F 4 "TMP36GT9Z-ND" H 6000 3100 60  0001 L CNN "Digi-Key_PN"
F 5 "TMP36GT9Z" H 6000 3200 60  0001 L CNN "MPN"
F 6 "Sensors, Transducers" H 6000 3300 60  0001 L CNN "Category"
F 7 "Temperature Sensors - Analog and Digital Output" H 6000 3400 60  0001 L CNN "Family"
F 8 "https://www.analog.com/media/en/technical-documentation/data-sheets/TMP35_36_37.pdf" H 6000 3500 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/analog-devices-inc/TMP36GT9Z/TMP36GT9Z-ND/820404" H 6000 3600 60  0001 L CNN "DK_Detail_Page"
F 10 "SENSOR ANALOG -40C-125C TO92-3" H 6000 3700 60  0001 L CNN "Description"
F 11 "Analog Devices Inc." H 6000 3800 60  0001 L CNN "Manufacturer"
F 12 "Active" H 6000 3900 60  0001 L CNN "Status"
	1    5800 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 6150 5900 6500
Wire Wire Line
	5450 3650 5450 5750
$Comp
L Switch:SW_Push SW1
U 1 1 5F0513E8
P 5450 5950
F 0 "SW1" H 5450 6235 50  0000 C CNN
F 1 "SW_Push" H 5450 6144 50  0000 C CNN
F 2 "" H 5450 6150 50  0001 C CNN
F 3 "~" H 5450 6150 50  0001 C CNN
	1    5450 5950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5900 5750 5900 4050
$Comp
L Switch:SW_Push SW2
U 1 1 5F04DE67
P 5900 5950
F 0 "SW2" H 5900 6235 50  0000 C CNN
F 1 "SW_Push" H 5900 6144 50  0000 C CNN
F 2 "" H 5900 6150 50  0001 C CNN
F 3 "~" H 5900 6150 50  0001 C CNN
	1    5900 5950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6300 4250 6300 5550
Wire Wire Line
	7650 4250 6300 4250
Wire Wire Line
	6850 5550 6850 4350
Wire Wire Line
	6300 5850 6300 6000
$Comp
L Device:R R3
U 1 1 5F048F9A
P 6300 5700
F 0 "R3" H 6370 5746 50  0000 L CNN
F 1 "330" H 6370 5655 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 6230 5700 50  0001 C CNN
F 3 "~" H 6300 5700 50  0001 C CNN
	1    6300 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 6300 6850 6500
Wire Wire Line
	6850 5850 6850 6000
$Comp
L Device:LED D2
U 1 1 5F04507A
P 6850 6150
F 0 "D2" V 6889 6032 50  0000 R CNN
F 1 "LED" V 6798 6032 50  0000 R CNN
F 2 "" H 6850 6150 50  0001 C CNN
F 3 "~" H 6850 6150 50  0001 C CNN
	1    6850 6150
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R4
U 1 1 5F045074
P 6850 5700
F 0 "R4" H 6920 5746 50  0000 L CNN
F 1 "330" H 6920 5655 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 6780 5700 50  0001 C CNN
F 3 "~" H 6850 5700 50  0001 C CNN
	1    6850 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 1850 7800 1950
Connection ~ 7800 1850
Wire Wire Line
	7550 3350 7650 3350
Wire Wire Line
	7550 1850 7550 3350
Wire Wire Line
	7800 1850 7550 1850
$Comp
L Device:R R5
U 1 1 5F0383E5
P 7800 1600
F 0 "R5" H 7870 1646 50  0000 L CNN
F 1 "6,8k" H 7870 1555 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 7730 1600 50  0001 C CNN
F 3 "~" H 7800 1600 50  0001 C CNN
	1    7800 1600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 5F037CFF
P 7800 2100
F 0 "R6" H 7870 2146 50  0000 L CNN
F 1 "10k" H 7870 2055 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 7730 2100 50  0001 C CNN
F 3 "~" H 7800 2100 50  0001 C CNN
	1    7800 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 4950 8250 6500
Wire Wire Line
	7650 4550 7550 4550
$Comp
L Switch:SW_Push SW3
U 1 1 5F029F6C
P 7550 5950
F 0 "SW3" H 7550 6235 50  0000 C CNN
F 1 "SW_Push" H 7550 6144 50  0000 C CNN
F 2 "" H 7550 6150 50  0001 C CNN
F 3 "~" H 7550 6150 50  0001 C CNN
	1    7550 5950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 3000 4750 3950
Wire Wire Line
	4400 3000 4400 3750
$Comp
L Device:R R2
U 1 1 5F02693C
P 4750 2850
F 0 "R2" H 4820 2896 50  0000 L CNN
F 1 "4,7k" H 4820 2805 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 4680 2850 50  0001 C CNN
F 3 "~" H 4750 2850 50  0001 C CNN
	1    4750 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5F0262B0
P 4400 2850
F 0 "R1" H 4470 2896 50  0000 L CNN
F 1 "4,7k" H 4470 2805 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 4330 2850 50  0001 C CNN
F 3 "~" H 4400 2850 50  0001 C CNN
	1    4400 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 3350 3950 3600
Wire Wire Line
	4200 3350 3950 3350
Wire Wire Line
	4200 4200 4200 3350
Connection ~ 3950 4500
Wire Wire Line
	4200 4500 3950 4500
$Comp
L Device:CP C2
U 1 1 5F024913
P 4200 4350
F 0 "C2" H 4318 4396 50  0000 L CNN
F 1 "100/16v" H 4318 4305 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.8" H 4238 4200 50  0001 C CNN
F 3 "~" H 4200 4350 50  0001 C CNN
	1    4200 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 3600 3650 3600
Wire Wire Line
	3950 4100 3650 4100
Connection ~ 8250 6500
Wire Wire Line
	3950 4100 3950 4500
Wire Wire Line
	10500 4400 10500 4600
Wire Wire Line
	10500 3950 10500 4100
Wire Wire Line
	7100 4450 7650 4450
Wire Wire Line
	7100 5250 7100 4450
Wire Wire Line
	9250 6100 9250 6500
Wire Wire Line
	9250 5250 9700 5250
Connection ~ 9250 5250
Wire Wire Line
	9250 5800 9250 5250
Wire Wire Line
	9050 5250 9250 5250
$Comp
L Device:LED D3
U 1 1 5F01BF07
P 10500 4250
F 0 "D3" V 10539 4132 50  0000 R CNN
F 1 "LED" V 10448 4132 50  0000 R CNN
F 2 "" H 10500 4250 50  0001 C CNN
F 3 "~" H 10500 4250 50  0001 C CNN
	1    10500 4250
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R7
U 1 1 5F01B62A
P 10500 3800
F 0 "R7" H 10570 3846 50  0000 L CNN
F 1 "330" H 10570 3755 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 10430 3800 50  0001 C CNN
F 3 "~" H 10500 3800 50  0001 C CNN
	1    10500 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 5F01AE07
P 9250 5950
F 0 "R9" H 9320 5996 50  0000 L CNN
F 1 "1k" H 9320 5905 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 9180 5950 50  0001 C CNN
F 3 "~" H 9250 5950 50  0001 C CNN
	1    9250 5950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5F01A58A
P 8900 5250
F 0 "R8" V 8693 5250 50  0000 C CNN
F 1 "10" V 8784 5250 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 8830 5250 50  0001 C CNN
F 3 "~" H 8900 5250 50  0001 C CNN
	1    8900 5250
	0    1    1    0   
$EndComp
Wire Wire Line
	10000 4200 10000 4600
Wire Wire Line
	10000 3900 10000 3500
$Comp
L Device:R R10
U 1 1 5F019455
P 10000 4050
F 0 "R10" H 10070 4096 50  0000 L CNN
F 1 "3" H 10070 4005 50  0000 L CNN
F 2 "" V 9930 4050 50  0001 C CNN
F 3 "~" H 10000 4050 50  0001 C CNN
	1    10000 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 5450 10000 6850
$Comp
L Transistor_FET:IRLZ44N Q1
U 1 1 5F0161A4
P 9900 5250
F 0 "Q1" H 10104 5296 50  0000 L CNN
F 1 "IRLZ44N" H 10104 5205 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 10150 5175 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irlz44n.pdf" H 9900 5250 50  0001 L CNN
	1    9900 5250
	1    0    0    -1  
$EndComp
Connection ~ 8250 4950
Wire Wire Line
	8250 4850 8250 4950
Wire Wire Line
	11000 6850 10000 6850
Wire Wire Line
	8950 4950 8250 4950
Wire Wire Line
	8950 4250 8950 4950
$Comp
L MCU_Microchip_ATtiny:ATtiny84A-SSU U3
U 1 1 5F00A417
P 8250 3950
F 0 "U3" H 8150 4100 50  0000 R CNN
F 1 "ATtiny84A-SSU" H 8400 3900 50  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 8250 3950 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/doc8183.pdf" H 8250 3950 50  0001 C CNN
	1    8250 3950
	-1   0    0    -1  
$EndComp
$Comp
L power:-BATT #PWR05
U 1 1 5F00FDD7
P 11000 6850
F 0 "#PWR05" H 11000 6700 50  0001 C CNN
F 1 "-BATT" H 11015 7023 50  0000 C CNN
F 2 "" H 11000 6850 50  0001 C CNN
F 3 "" H 11000 6850 50  0001 C CNN
	1    11000 6850
	-1   0    0    1   
$EndComp
$Comp
L Device:C C3
U 1 1 5F00D9E1
P 8950 4100
F 0 "C3" H 9065 4146 50  0000 L CNN
F 1 "100n" H 9065 4055 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 8988 3950 50  0001 C CNN
F 3 "~" H 8950 4100 50  0001 C CNN
	1    8950 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 4850 1250 3900
Wire Wire Line
	1600 3600 2000 3600
Connection ~ 1600 3600
Wire Wire Line
	1600 4400 1600 3600
Connection ~ 2450 4100
Wire Wire Line
	1450 4100 2450 4100
Wire Wire Line
	1450 4000 1450 4100
Wire Wire Line
	1500 3600 1600 3600
Wire Wire Line
	1500 3700 1500 3600
Wire Wire Line
	2450 4100 2450 4800
Wire Wire Line
	2450 4800 2000 4800
$Comp
L Device:C C1
U 1 1 5F0A8F0A
P 1600 4550
F 0 "C1" H 1715 4596 50  0000 L CNN
F 1 "100n" H 1715 4505 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 1638 4400 50  0001 C CNN
F 3 "~" H 1600 4550 50  0001 C CNN
	1    1600 4550
	1    0    0    -1  
$EndComp
$Comp
L ADUM1250:ADUM1250 IC1
U 1 1 5F07C719
P 3050 3850
F 0 "IC1" H 3050 4342 50  0000 C CNN
F 1 "ADUM1250" H 3050 4251 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 3050 3850 50  0001 L BNN
F 3 "" H 3050 3850 50  0001 C CNN
	1    3050 3850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1600 4700 1600 4800
$Comp
L Device:LED D1
U 1 1 5F048FA0
P 6300 6150
F 0 "D1" V 6339 6032 50  0000 R CNN
F 1 "LED" V 6248 6032 50  0000 R CNN
F 2 "" H 6300 6150 50  0001 C CNN
F 3 "~" H 6300 6150 50  0001 C CNN
	1    6300 6150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6300 6300 6300 6500
$Comp
L Device:D_Schottky D4
U 1 1 5F0BA5E9
P 2000 4550
F 0 "D4" V 1954 4630 50  0000 L CNN
F 1 "D_Sch." V 2045 4630 50  0000 L CNN
F 2 "Diode_SMD:D_SMA" H 2000 4550 50  0001 C CNN
F 3 "~" H 2000 4550 50  0001 C CNN
	1    2000 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	1150 4750 1150 3800
Wire Wire Line
	1050 4650 1050 3700
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 5F0AF784
P 700 3800
F 0 "J1" H 618 4117 50  0000 C CNN
F 1 "Conn_01x04" H 618 4026 50  0000 C CNN
F 2 "" H 700 3800 50  0001 C CNN
F 3 "~" H 700 3800 50  0001 C CNN
	1    700  3800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	900  3700 1050 3700
Wire Wire Line
	900  4000 1350 4000
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 5F0D5262
P 700 4750
F 0 "J2" H 618 5067 50  0000 C CNN
F 1 "Conn_01x04" H 618 4976 50  0000 C CNN
F 2 "" H 700 4750 50  0001 C CNN
F 3 "~" H 700 4750 50  0001 C CNN
	1    700  4750
	-1   0    0    -1  
$EndComp
Wire Wire Line
	900  4650 1050 4650
Wire Wire Line
	900  4750 1150 4750
Wire Wire Line
	900  3800 1150 3800
Wire Wire Line
	900  4850 1250 4850
Wire Wire Line
	900  4950 1350 4950
Connection ~ 1050 3700
Connection ~ 1150 3800
Connection ~ 1250 3900
Wire Wire Line
	1250 3900 900  3900
Connection ~ 1350 4000
Wire Wire Line
	1350 4000 1450 4000
Wire Wire Line
	1150 3800 2450 3800
Wire Wire Line
	1050 3700 1500 3700
Wire Wire Line
	1250 3900 2450 3900
Wire Wire Line
	1350 4000 1350 4950
Wire Wire Line
	2000 4400 2000 3600
Connection ~ 2000 3600
Wire Wire Line
	2000 3600 2450 3600
Wire Wire Line
	2000 4700 2000 4800
Connection ~ 2000 4800
Wire Wire Line
	2000 4800 1600 4800
Wire Wire Line
	8250 6500 9250 6500
Wire Wire Line
	10500 3650 10500 3500
Wire Wire Line
	10500 3500 10000 3500
Wire Wire Line
	10500 4600 10000 4600
Connection ~ 10000 4600
Wire Wire Line
	10000 4600 10000 5050
Wire Wire Line
	7100 5250 8750 5250
$Comp
L Connector_Generic:Conn_02x06_Top_Bottom J3
U 1 1 5F14268F
P 1750 1350
F 0 "J3" H 1800 1767 50  0000 C CNN
F 1 "Conn_02x06_Top_Bottom" H 1800 1676 50  0000 C CNN
F 2 "" H 1750 1350 50  0001 C CNN
F 3 "~" H 1750 1350 50  0001 C CNN
	1    1750 1350
	1    0    0    -1  
$EndComp
Connection ~ 10000 3500
Connection ~ 3950 3350
$Comp
L power:+BATT #PWR04
U 1 1 5F00F160
P 10950 800
F 0 "#PWR04" H 10950 650 50  0001 C CNN
F 1 "+BATT" H 10965 973 50  0000 C CNN
F 2 "" H 10950 800 50  0001 C CNN
F 3 "" H 10950 800 50  0001 C CNN
	1    10950 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 800  4400 800 
Wire Wire Line
	3950 800  3950 3350
Wire Wire Line
	4400 2700 4400 800 
Wire Wire Line
	4750 2700 4750 800 
Connection ~ 4750 800 
Connection ~ 4400 800 
Wire Wire Line
	4400 800  4750 800 
$Comp
L dk_Temperature-Sensors-Analog-and-Digital-Output:TMP36GT9Z U2
U 1 1 5F06042B
P 6800 1750
F 0 "U2" H 6673 1803 60  0000 R CNN
F 1 "TMP36GT9Z" H 6673 1697 60  0000 R CNN
F 2 "digikey-footprints:TO-92-3" H 7000 1950 60  0001 L CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/TMP35_36_37.pdf" H 7000 2050 60  0001 L CNN
F 4 "TMP36GT9Z-ND" H 7000 2150 60  0001 L CNN "Digi-Key_PN"
F 5 "TMP36GT9Z" H 7000 2250 60  0001 L CNN "MPN"
F 6 "Sensors, Transducers" H 7000 2350 60  0001 L CNN "Category"
F 7 "Temperature Sensors - Analog and Digital Output" H 7000 2450 60  0001 L CNN "Family"
F 8 "https://www.analog.com/media/en/technical-documentation/data-sheets/TMP35_36_37.pdf" H 7000 2550 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/analog-devices-inc/TMP36GT9Z/TMP36GT9Z-ND/820404" H 7000 2650 60  0001 L CNN "DK_Detail_Page"
F 10 "SENSOR ANALOG -40C-125C TO92-3" H 7000 2750 60  0001 L CNN "Description"
F 11 "Analog Devices Inc." H 7000 2850 60  0001 L CNN "Manufacturer"
F 12 "Active" H 7000 2950 60  0001 L CNN "Status"
	1    6800 1750
	1    0    0    -1  
$EndComp
Text Notes 9550 600  0    50   ~ 0
Cell positive (from 2,8V to 4,2 V)
Wire Wire Line
	3950 800  1150 800 
Wire Wire Line
	1150 800  1150 1150
Wire Wire Line
	1150 1150 1550 1150
Connection ~ 3950 800 
Text GLabel 2250 1150 2    50   Input ~ 0
vcc_tiny
Wire Wire Line
	2050 1150 2250 1150
Text GLabel 5400 1150 0    50   Input ~ 0
vcc_tiny
Wire Wire Line
	5800 1150 5800 2400
Wire Wire Line
	6800 1150 6800 1450
Wire Wire Line
	5400 1150 5800 1150
Connection ~ 5800 1150
Wire Wire Line
	7800 1150 7800 1450
Wire Wire Line
	5800 1150 6800 1150
Connection ~ 6800 1150
Wire Wire Line
	6800 1150 7800 1150
Wire Wire Line
	7800 1150 8250 1150
Wire Wire Line
	8250 1150 8250 3050
Connection ~ 7800 1150
Wire Wire Line
	8250 1150 8950 1150
Wire Wire Line
	8950 1150 8950 3950
Connection ~ 8250 1150
Text GLabel 5000 3750 2    50   Input ~ 0
scl_cel
Wire Wire Line
	4400 3750 5000 3750
Connection ~ 4400 3750
Text GLabel 5000 3950 2    50   Input ~ 0
sda_cel
Wire Wire Line
	4000 3950 4750 3950
Connection ~ 4750 3950
Wire Wire Line
	4750 3950 5000 3950
Text GLabel 850  1250 0    50   Input ~ 0
sda_cel
Text GLabel 1000 1350 0    50   Input ~ 0
scl_cel
Wire Wire Line
	1550 1250 850  1250
Wire Wire Line
	1550 1350 1000 1350
Text GLabel 2350 1250 2    50   Input ~ 0
sda_tiny
Text GLabel 2550 1350 2    50   Input ~ 0
scl_tiny
Wire Wire Line
	2350 1250 2050 1250
Wire Wire Line
	2050 1350 2550 1350
Text GLabel 6200 3750 0    50   Input ~ 0
scl_tiny
Text GLabel 6200 3950 0    50   Input ~ 0
sda_tiny
Wire Wire Line
	6200 3750 7650 3750
Wire Wire Line
	6200 3950 7650 3950
Text GLabel 2700 1450 2    50   Input ~ 0
spr_tiny
Wire Wire Line
	2700 1450 2050 1450
Text GLabel 7250 3850 0    50   Input ~ 0
spr_tiny
Wire Wire Line
	7250 3850 7650 3850
Wire Wire Line
	10000 800  10000 3500
$Comp
L Device:Fuse F1
U 1 1 5F2214CC
P 10400 800
F 0 "F1" V 10512 800 50  0000 C CNN
F 1 "3A" V 10591 800 50  0000 C CNN
F 2 "Fuse:Fuse_1206_3216Metric" V 10330 800 50  0001 C CNN
F 3 "~" H 10400 800 50  0001 C CNN
	1    10400 800 
	0    1    1    0   
$EndComp
Wire Wire Line
	10000 800  10250 800 
Connection ~ 10000 800 
Wire Wire Line
	5450 3650 7650 3650
Wire Wire Line
	5900 4050 7650 4050
Connection ~ 5900 6500
Connection ~ 6300 6500
Wire Wire Line
	6850 4350 7650 4350
Connection ~ 6850 6500
Wire Wire Line
	6850 6500 7550 6500
Wire Wire Line
	6300 6500 6850 6500
Wire Wire Line
	5450 6500 5900 6500
Wire Wire Line
	5900 6500 6300 6500
Wire Wire Line
	7550 6150 7550 6500
Connection ~ 7550 6500
Wire Wire Line
	7550 6500 8250 6500
Wire Wire Line
	10000 6850 3950 6850
Wire Wire Line
	3950 4500 3950 6850
Connection ~ 10000 6850
$Comp
L power:-BATT #PWR0101
U 1 1 5F2FF679
P 1400 1800
F 0 "#PWR0101" H 1400 1650 50  0001 C CNN
F 1 "-BATT" H 1415 1973 50  0000 C CNN
F 2 "" H 1400 1800 50  0001 C CNN
F 3 "" H 1400 1800 50  0001 C CNN
	1    1400 1800
	-1   0    0    1   
$EndComp
Wire Wire Line
	1400 1650 1550 1650
Text GLabel 2900 1650 2    50   Input ~ 0
gnd_tiny
Wire Wire Line
	2900 1650 2050 1650
Text GLabel 4800 6500 0    50   Input ~ 0
gnd_tiny
Wire Wire Line
	4800 6500 5450 6500
Connection ~ 5450 6500
Text GLabel 1450 1550 0    50   Input ~ 0
rst_cel
Wire Wire Line
	1450 1550 1550 1550
Wire Wire Line
	5450 6150 5450 6500
Text GLabel 2850 1550 2    50   Input ~ 0
rst_tiny
Wire Wire Line
	2850 1550 2050 1550
Text GLabel 7550 5700 1    50   Input ~ 0
rst_cel
Wire Wire Line
	7550 5700 7550 5750
Text GLabel 7550 4700 3    50   Input ~ 0
rst_tiny
Wire Wire Line
	7550 4550 7550 4700
Wire Notes Line
	3300 2200 3300 2800
Wire Notes Line
	3300 2800 650  2800
Wire Notes Line
	650  2800 650  2200
Wire Notes Line
	650  2200 3300 2200
Text Notes 850  2650 0    50   ~ 0
J3 connector.\n\nNormal Functions: connect a 6 jumpers connector\n\nProgramming: connect the arduino uno connector
Wire Wire Line
	