EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 19
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
L Connector_Generic:Conn_01x03 J?
U 1 1 60E6A728
P 5350 3400
AR Path="/60E6A37B/60E6A728" Ref="J?"  Part="1" 
AR Path="/60E6D7F7/60E6A728" Ref="J?"  Part="1" 
AR Path="/60E6D8EB/60E6A728" Ref="J?"  Part="1" 
AR Path="/60E6D92D/60E6A728" Ref="J?"  Part="1" 
AR Path="/60E6D9A3/60E6A728" Ref="J?"  Part="1" 
AR Path="/60E75254/60E6A728" Ref="J?"  Part="1" 
AR Path="/60E75257/60E6A728" Ref="J?"  Part="1" 
AR Path="/60E7525A/60E6A728" Ref="J?"  Part="1" 
AR Path="/60E7525D/60E6A728" Ref="J?"  Part="1" 
AR Path="/60E75260/60E6A728" Ref="J?"  Part="1" 
F 0 "J?" H 5430 3442 50  0000 L CNN
F 1 "Soil_Moisture_Sensor" H 5430 3351 50  0000 L CNN
F 2 "" H 5350 3400 50  0001 C CNN
F 3 "~" H 5350 3400 50  0001 C CNN
	1    5350 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60E6B607
P 5050 3300
AR Path="/60E6A37B/60E6B607" Ref="#PWR?"  Part="1" 
AR Path="/60E6D7F7/60E6B607" Ref="#PWR?"  Part="1" 
AR Path="/60E6D8EB/60E6B607" Ref="#PWR?"  Part="1" 
AR Path="/60E6D92D/60E6B607" Ref="#PWR?"  Part="1" 
AR Path="/60E6D9A3/60E6B607" Ref="#PWR?"  Part="1" 
AR Path="/60E75254/60E6B607" Ref="#PWR?"  Part="1" 
AR Path="/60E75257/60E6B607" Ref="#PWR?"  Part="1" 
AR Path="/60E7525A/60E6B607" Ref="#PWR?"  Part="1" 
AR Path="/60E7525D/60E6B607" Ref="#PWR?"  Part="1" 
AR Path="/60E75260/60E6B607" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5050 3050 50  0001 C CNN
F 1 "GND" V 5055 3172 50  0000 R CNN
F 2 "" H 5050 3300 50  0001 C CNN
F 3 "" H 5050 3300 50  0001 C CNN
	1    5050 3300
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 60E6C02F
P 5050 3400
AR Path="/60E6A37B/60E6C02F" Ref="#PWR?"  Part="1" 
AR Path="/60E6D7F7/60E6C02F" Ref="#PWR?"  Part="1" 
AR Path="/60E6D8EB/60E6C02F" Ref="#PWR?"  Part="1" 
AR Path="/60E6D92D/60E6C02F" Ref="#PWR?"  Part="1" 
AR Path="/60E6D9A3/60E6C02F" Ref="#PWR?"  Part="1" 
AR Path="/60E75254/60E6C02F" Ref="#PWR?"  Part="1" 
AR Path="/60E75257/60E6C02F" Ref="#PWR?"  Part="1" 
AR Path="/60E7525A/60E6C02F" Ref="#PWR?"  Part="1" 
AR Path="/60E7525D/60E6C02F" Ref="#PWR?"  Part="1" 
AR Path="/60E75260/60E6C02F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5050 3250 50  0001 C CNN
F 1 "+3.3V" V 5065 3528 50  0000 L CNN
F 2 "" H 5050 3400 50  0001 C CNN
F 3 "" H 5050 3400 50  0001 C CNN
	1    5050 3400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5050 3300 5150 3300
Wire Wire Line
	5050 3400 5150 3400
Text HLabel 5050 3500 0    50   Output ~ 0
SENSE
Wire Wire Line
	5050 3500 5150 3500
$EndSCHEMATC
