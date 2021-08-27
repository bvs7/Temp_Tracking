EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 19
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
L Transistor_FET:BS170 Q?
U 1 1 60FEC580
P 5250 4400
AR Path="/60FEC580" Ref="Q?"  Part="1" 
AR Path="/60FE860F/60FEC580" Ref="Q?"  Part="1" 
AR Path="/60E68C49/60FEC580" Ref="Q?"  Part="1" 
AR Path="/60E68E60/60FEC580" Ref="Q?"  Part="1" 
AR Path="/60E68F4B/60FEC580" Ref="Q?"  Part="1" 
AR Path="/60E699AB/60FEC580" Ref="Q?"  Part="1" 
AR Path="/60E699EC/60FEC580" Ref="Q?"  Part="1" 
F 0 "Q?" H 5454 4446 50  0000 L CNN
F 1 "BS170" H 5454 4355 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 5450 4325 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BS170-D.PDF" H 5250 4400 50  0001 L CNN
	1    5250 4400
	-1   0    0    -1  
$EndComp
$Comp
L Motor:Motor_DC M?
U 1 1 60FEC586
P 4850 3250
AR Path="/60FEC586" Ref="M?"  Part="1" 
AR Path="/60FE860F/60FEC586" Ref="M?"  Part="1" 
AR Path="/60E68C49/60FEC586" Ref="M?"  Part="1" 
AR Path="/60E68E60/60FEC586" Ref="M?"  Part="1" 
AR Path="/60E68F4B/60FEC586" Ref="M?"  Part="1" 
AR Path="/60E699AB/60FEC586" Ref="M?"  Part="1" 
AR Path="/60E699EC/60FEC586" Ref="M?"  Part="1" 
F 0 "M?" H 5008 3246 50  0000 L CNN
F 1 "12V Solenoid Valve" H 5008 3155 50  0000 L CNN
F 2 "" H 4850 3160 50  0001 C CNN
F 3 "~" H 4850 3160 50  0001 C CNN
	1    4850 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 4100 4950 4200
Wire Wire Line
	4850 3550 4850 3650
Connection ~ 4850 3650
Wire Wire Line
	4850 3650 4850 3700
$Comp
L Device:D D?
U 1 1 60FEC5A2
P 4500 3200
AR Path="/60FEC5A2" Ref="D?"  Part="1" 
AR Path="/60FE860F/60FEC5A2" Ref="D?"  Part="1" 
AR Path="/60E68C49/60FEC5A2" Ref="D?"  Part="1" 
AR Path="/60E68E60/60FEC5A2" Ref="D?"  Part="1" 
AR Path="/60E68F4B/60FEC5A2" Ref="D?"  Part="1" 
AR Path="/60E699AB/60FEC5A2" Ref="D?"  Part="1" 
AR Path="/60E699EC/60FEC5A2" Ref="D?"  Part="1" 
F 0 "D?" V 4454 3280 50  0000 L CNN
F 1 "D" V 4545 3280 50  0000 L CNN
F 2 "" H 4500 3200 50  0001 C CNN
F 3 "~" H 4500 3200 50  0001 C CNN
	1    4500 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	4500 3650 4500 3350
Wire Wire Line
	4850 3650 4500 3650
Wire Wire Line
	4950 3650 4850 3650
$Comp
L Switch:SW_SPDT_MSM SW?
U 1 1 60FEC5B3
P 4850 3900
AR Path="/60FEC5B3" Ref="SW?"  Part="1" 
AR Path="/60FE860F/60FEC5B3" Ref="SW?"  Part="1" 
AR Path="/60E68C49/60FEC5B3" Ref="SW?"  Part="1" 
AR Path="/60E68E60/60FEC5B3" Ref="SW?"  Part="1" 
AR Path="/60E68F4B/60FEC5B3" Ref="SW?"  Part="1" 
AR Path="/60E699AB/60FEC5B3" Ref="SW?"  Part="1" 
AR Path="/60E699EC/60FEC5B3" Ref="SW?"  Part="1" 
F 0 "SW?" V 4896 4027 50  0000 L CNN
F 1 "SW_SPDT" V 4805 3937 50  0000 L CNN
F 2 "" H 4850 3900 50  0001 C CNN
F 3 "~" H 4850 3900 50  0001 C CNN
	1    4850 3900
	0    -1   1    0   
$EndComp
Wire Wire Line
	4750 4100 4750 4600
Wire Wire Line
	5150 4200 4950 4200
Text HLabel 5600 4400 2    50   Input ~ 0
VALVE_CTRL
Wire Wire Line
	5450 4400 5500 4400
Wire Wire Line
	4500 3000 4700 3000
Wire Wire Line
	4750 4600 4950 4600
$Comp
L power:+12V #PWR?
U 1 1 610140FD
P 4700 2900
AR Path="/60E68C49/610140FD" Ref="#PWR?"  Part="1" 
AR Path="/60E68E60/610140FD" Ref="#PWR?"  Part="1" 
AR Path="/60E68F4B/610140FD" Ref="#PWR?"  Part="1" 
AR Path="/60E699AB/610140FD" Ref="#PWR?"  Part="1" 
AR Path="/60E699EC/610140FD" Ref="#PWR?"  Part="1" 
AR Path="/60FE860F/610140FD" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4700 2750 50  0001 C CNN
F 1 "+12V" H 4715 3073 50  0000 C CNN
F 2 "" H 4700 2900 50  0001 C CNN
F 3 "" H 4700 2900 50  0001 C CNN
	1    4700 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6101484B
P 4950 4650
AR Path="/60E68C49/6101484B" Ref="#PWR?"  Part="1" 
AR Path="/60E68E60/6101484B" Ref="#PWR?"  Part="1" 
AR Path="/60E68F4B/6101484B" Ref="#PWR?"  Part="1" 
AR Path="/60E699AB/6101484B" Ref="#PWR?"  Part="1" 
AR Path="/60E699EC/6101484B" Ref="#PWR?"  Part="1" 
AR Path="/60FE860F/6101484B" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4950 4400 50  0001 C CNN
F 1 "GND" H 4955 4477 50  0000 C CNN
F 2 "" H 4950 4650 50  0001 C CNN
F 3 "" H 4950 4650 50  0001 C CNN
	1    4950 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 4650 4950 4600
Connection ~ 4950 4600
Wire Wire Line
	4950 4600 5150 4600
Wire Wire Line
	4700 2900 4700 3000
Connection ~ 4700 3000
Wire Wire Line
	4700 3000 4850 3000
$Comp
L Device:R R?
U 1 1 60EB80F6
P 5500 4550
F 0 "R?" H 5570 4596 50  0000 L CNN
F 1 "R" H 5570 4505 50  0000 L CNN
F 2 "" V 5430 4550 50  0001 C CNN
F 3 "~" H 5500 4550 50  0001 C CNN
	1    5500 4550
	1    0    0    -1  
$EndComp
Connection ~ 5500 4400
Wire Wire Line
	5500 4400 5600 4400
$Comp
L Device:LED D?
U 1 1 60EB8B34
P 5500 4900
F 0 "D?" V 5539 4782 50  0000 R CNN
F 1 "LED" V 5448 4782 50  0000 R CNN
F 2 "" H 5500 4900 50  0001 C CNN
F 3 "~" H 5500 4900 50  0001 C CNN
	1    5500 4900
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60EBA2C3
P 5500 5100
AR Path="/60E68C49/60EBA2C3" Ref="#PWR?"  Part="1" 
AR Path="/60E68E60/60EBA2C3" Ref="#PWR?"  Part="1" 
AR Path="/60E68F4B/60EBA2C3" Ref="#PWR?"  Part="1" 
AR Path="/60E699AB/60EBA2C3" Ref="#PWR?"  Part="1" 
AR Path="/60E699EC/60EBA2C3" Ref="#PWR?"  Part="1" 
AR Path="/60FE860F/60EBA2C3" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5500 4850 50  0001 C CNN
F 1 "GND" H 5505 4927 50  0000 C CNN
F 2 "" H 5500 5100 50  0001 C CNN
F 3 "" H 5500 5100 50  0001 C CNN
	1    5500 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 5050 5500 5100
Wire Wire Line
	5500 4700 5500 4750
$Comp
L Device:D D?
U 1 1 60FEC599
P 5100 3650
AR Path="/60FEC599" Ref="D?"  Part="1" 
AR Path="/60FE860F/60FEC599" Ref="D?"  Part="1" 
AR Path="/60E68C49/60FEC599" Ref="D?"  Part="1" 
AR Path="/60E68E60/60FEC599" Ref="D?"  Part="1" 
AR Path="/60E68F4B/60FEC599" Ref="D?"  Part="1" 
AR Path="/60E699AB/60FEC599" Ref="D?"  Part="1" 
AR Path="/60E699EC/60FEC599" Ref="D?"  Part="1" 
F 0 "D?" V 5054 3730 50  0000 L CNN
F 1 "D" V 5145 3730 50  0000 L CNN
F 2 "" H 5100 3650 50  0001 C CNN
F 3 "~" H 5100 3650 50  0001 C CNN
	1    5100 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 3650 5600 3650
Text HLabel 5600 3650 2    50   Output ~ 0
VALVE_SENSE
$Comp
L Device:R R?
U 1 1 60ECB7E2
P 4200 3150
F 0 "R?" H 4270 3196 50  0000 L CNN
F 1 "R" H 4270 3105 50  0000 L CNN
F 2 "" V 4130 3150 50  0001 C CNN
F 3 "~" H 4200 3150 50  0001 C CNN
	1    4200 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 60ECB7E8
P 4200 3500
F 0 "D?" V 4239 3382 50  0000 R CNN
F 1 "LED" V 4148 3382 50  0000 R CNN
F 2 "" H 4200 3500 50  0001 C CNN
F 3 "~" H 4200 3500 50  0001 C CNN
	1    4200 3500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4200 3650 4500 3650
Connection ~ 4500 3650
Wire Wire Line
	4500 3050 4500 3000
Wire Wire Line
	4850 3000 4850 3050
Wire Wire Line
	4500 3000 4200 3000
Connection ~ 4500 3000
Wire Wire Line
	4200 3300 4200 3350
$EndSCHEMATC
