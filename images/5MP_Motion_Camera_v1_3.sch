EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 1
Title "5 Mega-Pixel Motion Camera"
Date "2021-01-09"
Rev "1.3"
Comp ""
Comment1 "This schematic is in the public domain."
Comment2 "Arducam OV5642 5MP Camera, RCWL-0516, Pololu #2562"
Comment3 "Adafruit Feather M0 WiFi - ATSAMD21 + ATWINC1500"
Comment4 "Drawn by:  Patrick K. Moffitt"
$EndDescr
$Comp
L mcu_adafruit_feather_m0_jp3-connector:MCU_Adafruit_Feather_M0_JP3-Connector JP3
U 1 1 5E975E35
P 4950 4450
F 0 "JP3" H 4855 4668 50  0000 L CNN
F 1 "MCU_Adafruit_Feather_M0_JP3-Connector" H 4855 4550 50  0000 L CNN
F 2 "" H 4950 4450 50  0001 C CNN
F 3 "" H 4950 4450 50  0001 C CNN
	1    4950 4450
	1    0    0    -1  
$EndComp
$Comp
L rcwl-0516-sensor_motion:RCWL-0516-Sensor_Motion U3
U 1 1 5E978523
P 5400 1900
F 0 "U3" H 5928 1596 50  0000 L CNN
F 1 "RCWL-0516-Sensor_Motion" H 5928 1505 50  0000 L CNN
F 2 "" H 5150 1900 50  0001 C CNN
F 3 "" H 5150 1900 50  0001 C CNN
	1    5400 1900
	1    0    0    -1  
$EndComp
$Comp
L arducam_ov5642_rev_c:Arducam_OV5642_rev_C U1
U 1 1 5E97C41F
P 5150 3600
F 0 "U1" H 5478 3651 50  0000 L CNN
F 1 "Arducam_OV5642_rev_C" H 5478 3560 50  0000 L CNN
F 2 "" H 5150 3700 50  0001 C CNN
F 3 "" H 5150 3700 50  0001 C CNN
	1    5150 3600
	1    0    0    -1  
$EndComp
$Comp
L 5v_step_up_voltage_regulator_Pololu_2562_u1v11f5:5V_Step_Up_Voltage_Regulator_U1V11F5 U2
U 1 1 5E97B25E
P 5200 2900
F 0 "U2" H 5528 2946 50  0000 L CNN
F 1 "5V_Step_Up_Voltage_Regulator_U1V11F5" H 5528 2855 50  0000 L CNN
F 2 "" H 5200 2900 50  0001 C CNN
F 3 "" H 5200 2900 50  0001 C CNN
	1    5200 2900
	1    0    0    -1  
$EndComp
$Comp
L mcu_adafruit_feather_m0_jp1-connector:MCU_Adafruit_Feather_M0_JP1-Connector JP1
U 1 1 5E9753B5
P 2700 2550
F 0 "JP1" H 2208 3115 50  0000 C CNN
F 1 "MCU_Adafruit_Feather_M0_JP1-Connector" H 2208 3024 50  0000 C CNN
F 2 "" H 2700 2950 50  0001 C CNN
F 3 "" H 2700 2950 50  0001 C CNN
	1    2700 2550
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW1
U 1 1 5E9AAA31
P 3600 1850
F 0 "SW1" H 3600 2085 50  0000 C CNN
F 1 "RESET_SW_SPST" H 3600 1994 50  0000 C CNN
F 2 "" H 3600 1850 50  0001 C CNN
F 3 "~" H 3600 1850 50  0001 C CNN
	1    3600 1850
	1    0    0    -1  
$EndComp
$Comp
L Connector:Jack-DC J1
U 1 1 5E9AE05F
P 3350 4900
F 0 "J1" H 3407 5225 50  0000 C CNN
F 1 "Jack-DC" H 3407 5134 50  0000 C CNN
F 2 "" H 3400 4860 50  0001 C CNN
F 3 "~" H 3400 4860 50  0001 C CNN
	1    3350 4900
	1    0    0    -1  
$EndComp
$Comp
L Device:Battery BT1
U 1 1 5E9C2DFA
P 3450 4250
F 0 "BT1" H 3558 4296 50  0000 L CNN
F 1 "3.7V LiPo" H 3558 4205 50  0000 L CNN
F 2 "" V 3450 4310 50  0001 C CNN
F 3 "~" V 3450 4310 50  0001 C CNN
	1    3450 4250
	1    0    0    -1  
$EndComp
Text HLabel 4700 5300 0    50   Input ~ 0
BAT_SENS
$Comp
L 5v_step_up_voltage_regulator_Pololu_2562_u1v11f5:5V_Step_Up_Voltage_Regulator_U1V11F5 U4
U 1 1 5EE451F6
P 5200 1600
F 0 "U4" H 5528 1646 50  0000 L CNN
F 1 "5V_Step_Up_Voltage_Regulator_U1V11F5" H 5528 1555 50  0000 L CNN
F 2 "" H 5200 1600 50  0001 C CNN
F 3 "" H 5200 1600 50  0001 C CNN
	1    5200 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 3350 4750 3350
Wire Wire Line
	2400 3450 4750 3450
Wire Wire Line
	2400 3250 2750 3250
Wire Wire Line
	2750 3250 2750 3550
Wire Wire Line
	2750 3550 4750 3550
Wire Wire Line
	2400 2550 3800 2550
Wire Wire Line
	4150 2550 4150 2150
Wire Wire Line
	4150 2150 4400 2150
Wire Wire Line
	4400 2950 4750 2950
Wire Wire Line
	4750 3050 4250 3050
Wire Wire Line
	4800 5700 2750 5700
Wire Wire Line
	2750 5700 2750 3850
Wire Wire Line
	4750 3950 2850 3950
Wire Wire Line
	2850 3950 2850 5600
Wire Wire Line
	2850 5600 4800 5600
Wire Wire Line
	4800 5400 2950 5400
Wire Wire Line
	2950 5400 2950 3250
Wire Wire Line
	2950 3250 4750 3250
Wire Wire Line
	4400 3650 4750 3650
Wire Wire Line
	4250 3050 4250 3750
Wire Wire Line
	4400 2950 4400 3650
Wire Wire Line
	4400 2950 4400 2150
Connection ~ 4400 2950
Connection ~ 4400 2150
Wire Wire Line
	4400 2150 4750 2150
Wire Wire Line
	2400 2250 3400 2250
Wire Wire Line
	3400 2250 3400 1850
Wire Wire Line
	3800 1850 3800 2550
Connection ~ 3800 2550
Wire Wire Line
	3800 2550 4150 2550
Wire Wire Line
	3650 4800 4800 4800
Wire Wire Line
	3900 5000 3650 5000
Wire Wire Line
	2750 3850 4750 3850
Wire Wire Line
	4250 3750 4750 3750
Wire Wire Line
	3900 5000 3900 4450
Wire Wire Line
	3900 4450 3450 4450
Wire Wire Line
	4800 4600 4050 4600
Wire Wire Line
	4050 4050 3450 4050
Wire Wire Line
	4400 3650 4400 5000
Wire Wire Line
	4400 5000 3900 5000
Connection ~ 4400 3650
Connection ~ 3900 5000
Wire Wire Line
	4700 5300 4800 5300
Wire Wire Line
	4750 2750 2550 2750
Wire Wire Line
	2550 2750 2550 5200
Wire Wire Line
	2550 5200 4800 5200
Wire Wire Line
	4750 2850 4050 2850
Wire Wire Line
	4750 1650 4400 1650
Wire Wire Line
	4400 1650 4400 2150
Connection ~ 4050 2850
Wire Wire Line
	4050 1550 4050 2850
Wire Wire Line
	4750 1550 4050 1550
Wire Wire Line
	4750 2250 4250 2250
Wire Wire Line
	4250 2250 4250 2650
Wire Wire Line
	4250 2650 2650 2650
Wire Wire Line
	2650 2650 2650 5500
Wire Wire Line
	2650 5500 4800 5500
Wire Wire Line
	2400 2650 2550 2650
Wire Wire Line
	2550 2650 2550 2350
Wire Wire Line
	2550 2350 3950 2350
Wire Wire Line
	3950 2350 3950 1450
Wire Wire Line
	3950 1450 4750 1450
Wire Wire Line
	4750 1750 4500 1750
Wire Wire Line
	4500 1750 4500 2350
Wire Wire Line
	4500 2350 4750 2350
$Comp
L Switch:SW_SPST SW2
U 1 1 5FFA1C81
P 3850 2850
F 0 "SW2" H 3850 2650 50  0000 C CNN
F 1 "BATT_SW_SPST" H 3850 2750 50  0000 C CNN
F 2 "" H 3850 2850 50  0001 C CNN
F 3 "~" H 3850 2850 50  0001 C CNN
	1    3850 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 4600 4050 4050
Wire Wire Line
	3450 4050 3450 2850
Wire Wire Line
	3450 2850 3650 2850
Connection ~ 3450 4050
$EndSCHEMATC
