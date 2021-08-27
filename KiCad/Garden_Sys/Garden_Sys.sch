EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 19
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Arduino:Arduino_Pro_Mini U?
U 1 1 60E638E0
P 2800 2500
F 0 "U?" H 2800 3389 60  0000 C CNN
F 1 "Arduino_Pro_Mini" H 2800 3283 60  0000 C CNN
F 2 "Arduino:Arduino_Pro_Mini" H 3600 1750 60  0001 C CNN
F 3 "https://www.sparkfun.com/products/11113" H 3000 1300 60  0001 C CNN
	1    2800 2500
	1    0    0    -1  
$EndComp
$Comp
L Connector:Barrel_Jack J?
U 1 1 60E659BF
P 1150 1050
F 0 "J?" H 1207 1375 50  0000 C CNN
F 1 "Barrel_Jack" H 1207 1284 50  0000 C CNN
F 2 "" H 1200 1010 50  0001 C CNN
F 3 "~" H 1200 1010 50  0001 C CNN
	1    1150 1050
	1    0    0    -1  
$EndComp
$Comp
L SparkFun-IC-Power:V_REG_LD1117VXX U?
U 1 1 60E66743
P 4300 1100
F 0 "U?" H 4300 1354 45  0000 C CNN
F 1 "V_REG_LD1117V33" H 4300 1270 45  0000 C CNN
F 2 "78XXL" H 4300 1300 20  0001 C CNN
F 3 "" H 4300 1100 60  0001 C CNN
F 4 "" H 4300 1281 60  0000 C CNN "Field4"
	1    4300 1100
	1    0    0    -1  
$EndComp
$Comp
L ESP8266:ESP-01v090 U?
U 1 1 60E67408
P 5150 2350
F 0 "U?" H 5150 2865 50  0000 C CNN
F 1 "ESP-01v090" H 5150 2774 50  0000 C CNN
F 2 "" H 5150 2350 50  0001 C CNN
F 3 "http://l0l.org.uk/2014/12/esp8266-modules-hardware-guide-gotta-catch-em-all/" H 5150 2350 50  0001 C CNN
	1    5150 2350
	-1   0    0    -1  
$EndComp
$Sheet
S 8500 950  1300 500 
U 60E68C49
F0 "Valve_0" 50
F1 "../Common/Solenoid_Valve_Controller.sch" 50
F2 "VALVE_CTRL" I L 8500 1100 50 
F3 "VALVE_SENSE" O L 8500 1250 50 
$EndSheet
$Sheet
S 8500 1850 1300 500 
U 60E699AB
F0 "Valve_1" 50
F1 "../Common/Solenoid_Valve_Controller.sch" 50
F2 "VALVE_CTRL" I L 8500 2000 50 
F3 "VALVE_SENSE" O L 8500 2150 50 
$EndSheet
$Sheet
S 8500 2650 1300 500 
U 60E699EC
F0 "Valve_2" 50
F1 "../Common/Solenoid_Valve_Controller.sch" 50
F2 "VALVE_CTRL" I L 8500 2800 50 
F3 "VALVE_SENSE" O L 8500 2950 50 
$EndSheet
$Sheet
S 8500 3800 550  200 
U 60E6A37B
F0 "Soil_0" 50
F1 "../Common/Capacitive_Soil_Moisture_Sensor.sch" 20
F2 "SENSE" O L 8500 3900 50 
$EndSheet
$Sheet
S 8500 5700 550  200 
U 60E6D9A3
F0 "Soil_4" 50
F1 "../Common/Capacitive_Soil_Moisture_Sensor.sch" 20
F2 "SENSE" O L 8500 5800 50 
$EndSheet
$Sheet
S 8500 4700 550  200 
U 60E6D8EB
F0 "Soil_2" 50
F1 "../Common/Capacitive_Soil_Moisture_Sensor.sch" 20
F2 "SENSE" O L 8500 4800 50 
$EndSheet
$Sheet
S 8500 4250 550  200 
U 60E6D7F7
F0 "Soil_1" 50
F1 "../Common/Capacitive_Soil_Moisture_Sensor.sch" 20
F2 "SENSE" O L 8500 4350 50 
$EndSheet
$Sheet
S 4450 3750 550  200 
U 60E6E065
F0 "Temp_0" 50
F1 "../Common/Temperature_Sensor.sch" 28
F2 "SENSE" O L 4450 3850 50 
$EndSheet
$Sheet
S 4450 5050 550  200 
U 60E6E1E4
F0 "Light_0" 50
F1 "../Common/Light_Sensor.sch" 28
F2 "SENSE" O L 4450 5150 50 
$EndSheet
$Comp
L power:+12V #PWR?
U 1 1 60E73A32
P 1600 850
F 0 "#PWR?" H 1600 700 50  0001 C CNN
F 1 "+12V" H 1615 1023 50  0000 C CNN
F 2 "" H 1600 850 50  0001 C CNN
F 3 "" H 1600 850 50  0001 C CNN
	1    1600 850 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60E742F1
P 1600 1250
F 0 "#PWR?" H 1600 1000 50  0001 C CNN
F 1 "GND" H 1605 1077 50  0000 C CNN
F 2 "" H 1600 1250 50  0001 C CNN
F 3 "" H 1600 1250 50  0001 C CNN
	1    1600 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 1150 1600 1150
Wire Wire Line
	1600 1150 1600 1250
Wire Wire Line
	1450 950  1600 950 
Wire Wire Line
	1600 950  1600 850 
$Comp
L power:GND #PWR?
U 1 1 60E750D7
P 3500 2000
F 0 "#PWR?" H 3500 1750 50  0001 C CNN
F 1 "GND" H 3505 1827 50  0000 C CNN
F 2 "" H 3500 2000 50  0001 C CNN
F 3 "" H 3500 2000 50  0001 C CNN
	1    3500 2000
	0    -1   -1   0   
$EndComp
$Comp
L power:+12V #PWR?
U 1 1 60E7587B
P 3600 1900
F 0 "#PWR?" H 3600 1750 50  0001 C CNN
F 1 "+12V" H 3615 2073 50  0000 C CNN
F 2 "" H 3600 1900 50  0001 C CNN
F 3 "" H 3600 1900 50  0001 C CNN
	1    3600 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	3400 2000 3500 2000
$Comp
L power:+12V #PWR?
U 1 1 60E760A3
P 3900 1000
F 0 "#PWR?" H 3900 850 50  0001 C CNN
F 1 "+12V" H 3915 1173 50  0000 C CNN
F 2 "" H 3900 1000 50  0001 C CNN
F 3 "" H 3900 1000 50  0001 C CNN
	1    3900 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 1900 3600 1900
$Comp
L power:+3.3V #PWR?
U 1 1 60E766A0
P 4700 1000
F 0 "#PWR?" H 4700 850 50  0001 C CNN
F 1 "+3.3V" H 4715 1173 50  0000 C CNN
F 2 "" H 4700 1000 50  0001 C CNN
F 3 "" H 4700 1000 50  0001 C CNN
	1    4700 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 1100 4700 1100
Wire Wire Line
	4700 1100 4700 1000
Wire Wire Line
	3900 1000 3900 1100
Wire Wire Line
	3900 1100 4000 1100
$Comp
L power:GND #PWR?
U 1 1 60E7736F
P 4300 1500
F 0 "#PWR?" H 4300 1250 50  0001 C CNN
F 1 "GND" H 4305 1327 50  0000 C CNN
F 2 "" H 4300 1500 50  0001 C CNN
F 3 "" H 4300 1500 50  0001 C CNN
	1    4300 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 1400 4300 1500
$Comp
L power:+3.3V #PWR?
U 1 1 60E7960D
P 6200 2650
F 0 "#PWR?" H 6200 2500 50  0001 C CNN
F 1 "+3.3V" H 6215 2823 50  0000 C CNN
F 2 "" H 6200 2650 50  0001 C CNN
F 3 "" H 6200 2650 50  0001 C CNN
	1    6200 2650
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60E79A0A
P 4050 2150
F 0 "#PWR?" H 4050 1900 50  0001 C CNN
F 1 "GND" H 4055 1977 50  0000 C CNN
F 2 "" H 4050 2150 50  0001 C CNN
F 3 "" H 4050 2150 50  0001 C CNN
	1    4050 2150
	1    0    0    1   
$EndComp
Wire Wire Line
	4200 2200 4050 2200
Wire Wire Line
	4050 2200 4050 2150
Wire Wire Line
	6100 2500 6200 2500
Wire Wire Line
	6200 2500 6200 2650
Wire Wire Line
	6450 2200 6100 2200
Wire Wire Line
	6100 2300 6200 2300
Wire Wire Line
	6200 2300 6200 2500
Connection ~ 6200 2500
$Comp
L power:+3.3V #PWR?
U 1 1 60E7F799
P 2350 4050
F 0 "#PWR?" H 2350 3900 50  0001 C CNN
F 1 "+3.3V" H 2365 4223 50  0000 C CNN
F 2 "" H 2350 4050 50  0001 C CNN
F 3 "" H 2350 4050 50  0001 C CNN
	1    2350 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 4050 2350 4050
Wire Wire Line
	2800 3400 2800 4350
Wire Wire Line
	2800 4350 2650 4350
Text GLabel 1450 5350 0    50   Input ~ 0
S0
Text GLabel 1450 5450 0    50   Input ~ 0
S1
Text GLabel 1450 5550 0    50   Input ~ 0
S2
Wire Wire Line
	1450 5350 1650 5350
Wire Wire Line
	1650 5450 1450 5450
Wire Wire Line
	1450 5550 1650 5550
$Comp
L power:GND #PWR?
U 1 1 60E8B0E2
P 2250 5950
F 0 "#PWR?" H 2250 5700 50  0001 C CNN
F 1 "GND" H 2255 5777 50  0000 C CNN
F 2 "" H 2250 5950 50  0001 C CNN
F 3 "" H 2250 5950 50  0001 C CNN
	1    2250 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 5850 2250 5900
Wire Wire Line
	2250 5900 2150 5900
Wire Wire Line
	2150 5900 2150 5850
Connection ~ 2250 5900
Wire Wire Line
	2250 5900 2250 5950
Wire Wire Line
	1650 5250 1550 5250
Wire Wire Line
	1550 5250 1550 5900
Wire Wire Line
	1550 5900 2150 5900
Connection ~ 2150 5900
Text GLabel 2000 3000 0    50   Output ~ 0
S0
Wire Wire Line
	2000 3000 2200 3000
Text GLabel 3600 2700 2    50   Output ~ 0
S2
Wire Wire Line
	3600 2700 3400 2700
Text GLabel 3600 2800 2    50   Output ~ 0
S1
Wire Wire Line
	3400 2800 3600 2800
Text GLabel 8350 1100 0    50   Input ~ 0
VALVE0_CTRL
Text GLabel 8350 2000 0    50   Input ~ 0
VALVE1_CTRL
Text GLabel 8350 2800 0    50   Input ~ 0
VALVE2_CTRL
Wire Wire Line
	8350 2800 8500 2800
Wire Wire Line
	8350 2000 8500 2000
Wire Wire Line
	8350 1100 8500 1100
Text GLabel 8350 1250 0    50   Output ~ 0
VALVE0_SENSE
Text GLabel 8350 2150 0    50   Output ~ 0
VALVE1_SENSE
Text GLabel 8350 2950 0    50   Output ~ 0
VALVE2_SENSE
Wire Wire Line
	8350 2950 8500 2950
Wire Wire Line
	8350 2150 8500 2150
Wire Wire Line
	8350 1250 8500 1250
Text GLabel 2050 2300 0    50   Output ~ 0
VALVE0_CTRL
Text GLabel 2050 2500 0    50   Output ~ 0
VALVE1_CTRL
Text GLabel 2050 2700 0    50   Output ~ 0
VALVE2_CTRL
Wire Wire Line
	2050 2700 2200 2700
Wire Wire Line
	2050 2500 2200 2500
Wire Wire Line
	2050 2300 2200 2300
Text GLabel 2050 2400 0    50   Input ~ 0
VALVE0_SENSE
Text GLabel 2050 2600 0    50   Input ~ 0
VALVE1_SENSE
Text GLabel 2050 2800 0    50   Input ~ 0
VALVE2_SENSE
Wire Wire Line
	2050 2800 2200 2800
Wire Wire Line
	2050 2600 2200 2600
Wire Wire Line
	2050 2400 2200 2400
Text GLabel 1550 4350 0    50   Input ~ 0
SOIL0
Text GLabel 1550 4450 0    50   Input ~ 0
SOIL1
Text GLabel 1550 4550 0    50   Input ~ 0
SOIL2
Text GLabel 1550 4650 0    50   Input ~ 0
SOIL3
Text GLabel 1550 4750 0    50   Input ~ 0
SOIL4
Wire Wire Line
	1650 4350 1550 4350
Wire Wire Line
	1550 4450 1650 4450
Wire Wire Line
	1650 4550 1550 4550
Wire Wire Line
	1550 4650 1650 4650
Wire Wire Line
	1650 4750 1550 4750
Text GLabel 3500 2300 2    50   Input ~ 0
TEMP0
Text GLabel 3500 2400 2    50   Input ~ 0
TEMP1
Text GLabel 3500 2500 2    50   Input ~ 0
LIGHT0
Text GLabel 3500 2600 2    50   Input ~ 0
LIGHT1
Text GLabel 1550 4850 0    50   Input ~ 0
SOIL5
Text GLabel 1550 4950 0    50   Input ~ 0
SOIL6
Text GLabel 1550 5050 0    50   Input ~ 0
SOIL7
Wire Wire Line
	1550 5050 1650 5050
Wire Wire Line
	1550 4950 1650 4950
Wire Wire Line
	1550 4850 1650 4850
Wire Wire Line
	3400 2300 3500 2300
Wire Wire Line
	3400 2400 3500 2400
Wire Wire Line
	3400 2500 3500 2500
Wire Wire Line
	3400 2600 3500 2600
Text GLabel 3000 3500 3    50   Input ~ 0
SOIL8
Text GLabel 2700 3500 3    50   Input ~ 0
SOIL9
Wire Wire Line
	2700 3400 2700 3500
Wire Wire Line
	3000 3400 3000 3500
Text GLabel 2900 3500 3    50   Input ~ 0
LIGHT2
Wire Wire Line
	2900 3400 2900 3500
$Comp
L 4xxx:4051 U?
U 1 1 60E6E5EF
P 2150 4950
F 0 "U?" H 2150 6031 50  0000 C CNN
F 1 "4051" H 2150 5940 50  0000 C CNN
F 2 "" H 2150 4950 50  0001 C CNN
F 3 "http://www.intersil.com/content/dam/Intersil/documents/cd40/cd4051bms-52bms-53bms.pdf" H 2150 4950 50  0001 C CNN
	1    2150 4950
	1    0    0    -1  
$EndComp
$Sheet
S 9900 3800 550  200 
U 60E75254
F0 "Soil_5" 50
F1 "../Common/Capacitive_Soil_Moisture_Sensor.sch" 20
F2 "SENSE" O L 9900 3900 50 
$EndSheet
$Sheet
S 9900 5200 550  200 
U 60E75257
F0 "Soil_8" 50
F1 "../Common/Capacitive_Soil_Moisture_Sensor.sch" 20
F2 "SENSE" O L 9900 5300 50 
$EndSheet
$Sheet
S 9900 5700 550  200 
U 60E7525A
F0 "Soil_9" 50
F1 "../Common/Capacitive_Soil_Moisture_Sensor.sch" 20
F2 "SENSE" O L 9900 5800 50 
$EndSheet
$Sheet
S 9900 4700 550  200 
U 60E7525D
F0 "Soil_7" 50
F1 "../Common/Capacitive_Soil_Moisture_Sensor.sch" 20
F2 "SENSE" O L 9900 4800 50 
$EndSheet
$Sheet
S 9900 4250 550  200 
U 60E75260
F0 "Soil_6" 50
F1 "../Common/Capacitive_Soil_Moisture_Sensor.sch" 20
F2 "SENSE" O L 9900 4350 50 
$EndSheet
Text GLabel 8350 3900 0    50   Output ~ 0
SOIL0
Text GLabel 8350 4350 0    50   Output ~ 0
SOIL1
Text GLabel 8350 4800 0    50   Output ~ 0
SOIL2
Text GLabel 8350 5300 0    50   Output ~ 0
SOIL3
Text GLabel 8350 5800 0    50   Output ~ 0
SOIL4
Text GLabel 9750 4800 0    50   Output ~ 0
SOIL7
Text GLabel 9750 4350 0    50   Output ~ 0
SOIL6
Text GLabel 9750 3900 0    50   Output ~ 0
SOIL5
Text GLabel 9750 5300 0    50   Output ~ 0
SOIL8
$Sheet
S 8500 5200 550  200 
U 60E6D92D
F0 "Soil_3" 50
F1 "../Common/Capacitive_Soil_Moisture_Sensor.sch" 20
F2 "SENSE" O L 8500 5300 50 
$EndSheet
Text GLabel 9750 5800 0    50   Output ~ 0
SOIL9
Wire Wire Line
	9750 3900 9900 3900
Wire Wire Line
	9750 4350 9900 4350
Wire Wire Line
	9750 4800 9900 4800
Wire Wire Line
	9750 5300 9900 5300
Wire Wire Line
	9750 5800 9900 5800
Wire Wire Line
	8350 5800 8500 5800
Wire Wire Line
	8500 5300 8350 5300
Wire Wire Line
	8350 4800 8500 4800
Wire Wire Line
	8500 4350 8350 4350
Wire Wire Line
	8350 3900 8500 3900
$Sheet
S 4450 5550 550  200 
U 60EA4997
F0 "Light_1" 50
F1 "../Common/Light_Sensor.sch" 28
F2 "SENSE" O L 4450 5650 50 
$EndSheet
$Sheet
S 4450 6050 550  200 
U 60EA6752
F0 "Light_2" 50
F1 "../Common/Light_Sensor.sch" 28
F2 "SENSE" O L 4450 6150 50 
$EndSheet
$Sheet
S 4450 4250 550  200 
U 60EA8683
F0 "Temp_1" 50
F1 "../Common/Temperature_Sensor.sch" 28
F2 "SENSE" O L 4450 4350 50 
$EndSheet
Text GLabel 4350 3850 0    50   Output ~ 0
TEMP0
Text GLabel 4350 4350 0    50   Output ~ 0
TEMP1
Text GLabel 4350 5150 0    50   Output ~ 0
LIGHT0
Text GLabel 4350 5650 0    50   Output ~ 0
LIGHT1
Text GLabel 4350 6150 0    50   Output ~ 0
LIGHT2
Wire Wire Line
	4350 6150 4450 6150
Wire Wire Line
	4450 5650 4350 5650
Wire Wire Line
	4350 5150 4450 5150
Wire Wire Line
	4350 4350 4450 4350
Wire Wire Line
	4350 3850 4450 3850
Wire Wire Line
	3400 2900 4200 2900
Wire Wire Line
	4200 2900 4200 2500
Wire Wire Line
	3400 3000 6450 3000
Wire Wire Line
	6450 3000 6450 2200
$EndSCHEMATC
