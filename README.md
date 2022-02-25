<!-- PROJECT LOGO -->
<h3 align="center">
  PROJECT_YONDU
</h3>

  <p align="center">
  </div>
    <br />
<!-- Add video -->

<!-- PROJECT DESCRIPTION -->
In this Microcontroller Applications project, I will be doing an **Arduino Based MIDI Controller** using Arduino Mega 2560. I have completed this project by myself and it worked suprisingly well. Competed in a design contest and won **second place**.

[![](https://user-images.githubusercontent.com/91014375/155715698-1c502149-e41b-424a-a9af-3386f0ef14fc.JPG)](https://www.youtube.com/watch?v=ii_Ar1L6reU)  
[![](https://user-images.githubusercontent.com/91014375/155703079-b93c8b0b-ebb6-42dd-9d14-817c5e653eb3.png)](https://www.youtube.com/watch?v=ii_Ar1L6reU)

## Requirements
### Programs
For PC/LAPTOP:
1. [LoopMIDI](https://loopmidi.freedownloadscenter.com/windows/) Only for Windows (Mac/Linux - no need)
2. [Hairless MIDI](https://projectgus.github.io/hairless-midiserial/) For All users

 (*If LoopMidi link does not work, download here: https://www.tobias-erichsen.de/software/loopmidi.html*)

### Header Files
Open ArduinoIDE > Go to Sketch Tab > Hover mouse over Include Library > Click Manage Libraries
<br >
[If ArduinoIDE Libraries do not have the files, click the individual files below]
* [Midi.h](https://github.com/FortySevenEffects/arduino_midi_library/blob/master/src/MIDI.h) by Francois Best
* Wire.h
* [LiquidCrystal_I2C](https://github.com/jenschr/Arduino-libraries/blob/master/LiquidCrystal/LiquidCrystal_I2C.h) by Francisco Malpartida
* [LCD](https://github.com/jenschr/Arduino-libraries/blob/master/LiquidCrystal/LCD.h) by Francisco Malpartida
* [FastLED](https://github.com/FastLED/FastLED/blob/master/src/FastLED.h) by Mark Kriegsman & Sam Guyer
* TimerOne

### Components
These are components that I have used in this project, the websites may not be exact as I bought most in person at shops: [Koba Electronics](https://sg.asiafirms.com/singapore/koba-electronics-249106.html) & [Sun Light Electronics](http://sun-light.com.sg/).

* 1x [Arduino Mega 2560](https://www.amazon.sg/ELEGOO-ATmega2560-ATMEGA16U2-Compatible-Compliant/dp/B01H4ZLZLQ/ref=asc_df_B01H4ZLZLQ/?tag=googleshoppin-22&linkCode=df0&hvadid=403846345380&hvpos=&hvnetw=g&hvrand=11661593939603679031&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9062511&hvtargid=pla-490931309987&psc=1)
* 16x [Transparent White Arcaade Buttons 30mm](https://www.aliexpress.com/item/32649646757.html?spm=a2g0o.productlist.0.0.20797370ICdAaj&algo_pvid=13c4433f-08ab-4585-8936-488cb3c5b8b3&algo_exp_id=13c4433f-08ab-4585-8936-488cb3c5b8b3-6&pdp_ext_f=%7B%22sku_id%22%3A%2264212854868%22%7D)
* 2x [Slider Potentiometers 100kΩ](https://www.amazon.sg/uxcell-Variable-Resistors-Potentiometer-Potentiometers/dp/B07W3HW3P7/ref=sr_1_6?crid=2II2UI9IPB4BR&keywords=slider+potentiometer&qid=1645788806&sprefix=slider+potentiomet%2Caps%2C267&sr=8-6)
* 8 [Potentiometers 100kΩ](https://www.amazon.sg/Taiss-Terminals-Linear-Potentiometer-Control/dp/B07DHKQVG5/ref=sr_1_24?crid=26KI0MXX305VH&keywords=potentiometer+100kohm&qid=1645788901&sprefix=potentiometer+100ko%2Caps%2C245&sr=8-24)
* 2x [Square Buttons with LED](https://www.monotaro.sg/g/1000361319.html)
* 1x [16x2 LCD Green Backlight](https://www.digikey.sg/en/products/detail/orient-display/AMC1602AR-B-Y6WFDY-I2C/13168754?utm_adgroup=Display%20Modules&utm_source=google&utm_medium=cpc&utm_campaign=Shopping_Product_Optoelectronics&utm_term=&productid=13168754&gclid=Cj0KCQiAmeKQBhDvARIsAHJ7mF4EapRhGKoFxbaBBheiTn-52XvDfnNE02VnSTfxM0TcVFHcilV9RxQaAoLEEALw_wcB) with [I2C](https://sg.cytron.io/p-i2c-module-for-character-lcd)
* 1x [Rotary Encoder](https://shopee.sg/360-Degrees-Rotary-Encoder-Module-For-Arduino-Brick-Sensor-Switch-Development-Board-KY-040-With-Pins-i.237143839.7420234508?gclid=Cj0KCQiAmeKQBhDvARIsAHJ7mF40nUupJyL_mxzvZGf76F0KHRR0hpKcp8ybyVZlZDCDKHtuVeZ2JXcaAslDEALw_wcB)
* 1 x [RGB LED Strip WS2812B](https://shopee.sg/Xnbada-1m-2m-3m-4m-5m-WS2812B-30Led-m-RGB-Led-Strip-WS2812-5050SMD-Black-White-Board-i.313937784.6859719116)
* 1 x [3-Pin Rocker Switch](https://www.elecbee.com/en-12209-rocker-switch-power-switch-3-pin-with-light-solder-cable-2-position-kcd3n-102-operation-panel)
* 1x [Long Copper PCB](https://www.lazada.sg/products/2pcs-set-stripboard-veroboard-uncut-pcb-board-platine-single-side-circuit-board-i2054180268-s11268991060.html)
* 1 x [1000uF Capacitor 40/50V](https://www.digikey.sg/en/products/detail/nichicon/UPS1H102MHD/2599950?utm_adgroup=Aluminum%20Electrolytic%20Capacitors&utm_source=google&utm_medium=cpc&utm_campaign=Shopping_Product_Capacitors&utm_term=&productid=2599950&gclid=Cj0KCQiAmeKQBhDvARIsAHJ7mF4MUYNTcHivYd7aovXRrSR3mJGhxqygGD37bclIkqBM6gjSByUcgw4aAqaYEALw_wcB)
* 1 x [330Ω Resistor](https://www.digikey.sg/en/products/detail/stackpole-electronics-inc/CFM12JT330R/2617599?utm_adgroup=Through%20Hole%20Resistors&utm_source=google&utm_medium=cpc&utm_campaign=Shopping_Product_Resistors&utm_term=&productid=2617599&gclid=Cj0KCQiAmeKQBhDvARIsAHJ7mF4JFX-bKbHLxWuc8nm7BgbyPVbc7NW0f4RPRww-mzDjvCEV3SEYoOwaAl9hEALw_wcB)
* 3-4 x [220Ω Resistor](https://www.digikey.sg/en/products/detail/stackpole-electronics-inc/CFM12JT220R/2617576?utm_adgroup=Through%20Hole%20Resistors&utm_source=google&utm_medium=cpc&utm_campaign=Shopping_Product_Resistors&utm_term=&productid=2617576&gclid=Cj0KCQiAmeKQBhDvARIsAHJ7mF6k8tj6mS52GrAFVjv2fND6P6ZndRIFUBbsAj0yKAwDIXXPlkmkiB8aAmSjEALw_wcB)
* [Cable Sleeves](https://shopee.sg/-GUYU--164pcs-Heat-Shrink-Tubing-Insulated-Shrinkable-Tube-Wire-Cable-Sleeve-Kit-HOO-i.283475765.3243647732?gclid=Cj0KCQiAmeKQBhDvARIsAHJ7mF720QjoB-JlWpmpWwgmhkbXn3imK0l97Xx7uL-Aom3VZfx3V0RI9agaAoxPEALw_wcB) (Red/Black/Yellow)
* [Female Crimp Contact 2.54mm](https://www.harwin.com/products/M20-1160042/) (not confirmed)
* [Male-Female Spade Crimp 7mm](https://shopee.sg/%3Clittleboss%3E-50PCS-6-3-Crimp-Terminal-Male-Female-Spade-Connector-Crimping-Terminals--SG--i.141602779.13571663336)
* Jumper Wires (Male-Male/Male-Female/Female-Female)
* Solid-core Wires (For PCB Connections)
* Stranded-core Wires (For Soldering on Component Pins as extensions)
* Pin Connectors (Male-Male/Male-Female)

#### Acrylic Casing
* 4 Outer Acrylic Walls
* 1 Top Acrylic Cover
* 1 Base Acrylic Cover
* 4 Big Inner Walls (For RGB Division)
* 12 Small Inner Walls (For RGB Division)

#### Mounting
* [Hexagonal Single-Headed Copper Studs](https://www.beboxx.com/pages/product/hexagonal-single-headed-copper-studs?attribute_pa_b5f694488326076ff200c7=m3&attribute_pa_ba2a9c6c8c77e03f83ef8b=5mm&utm_source=Google%20Shopping&utm_campaign=GMC2022Feb&utm_medium=cpc&utm_term=250522)
* [5mm Round Head Screws](https://tzwfjs.en.made-in-china.com/product/qdlQFumHbahV/China-Diameter-5mm-Special-Price-Promotion-304-Stainless-Steel-Screw-Cross-Pan-Head-Machine-Screw-Round-Head-Bolt-GB818.html)
* Nuts
* 1x [Micro USB Cable](https://www.digikey.sg/product-detail/en/molex,-llc/0687840002/900-0687840002-ND/1952431?utm_adgroup=Cable%20Assemblies&utm_source=google&utm_medium=cpc&utm_campaign=Shopping_Supplier_Molex_0900_Co-op&utm_term=&productid=1952431&gclid=CjwKCAjwk6-LBhBZEiwAOUUDp_7nQXQXKrzknThne9085QWNJ5nhmFlHBlWl4rWz2RthVSJd9ZoSNBoC8BYQAvD_BwE)

## Pin Connections

The following diagram shown below is the pin connections between the Arduino Mega 2560 & other seperate components.

![PinConfig](https://user-images.githubusercontent.com/91014375/155714824-8a79001e-fbb5-4af7-bacb-ad13e2b05bec.jpg)

## PCB Layout

![V1Config](https://user-images.githubusercontent.com/91014375/155717162-43d454d7-3550-4f62-9e2a-f112e5d34434.jpg)

1. Use a cutter and cut the copper strip pcb into half.
2. Terminate connections labelled in picture above.
3. Drill through 4 holes on each corner labelled as 'X' in picture above.
4. Start Soldering.

Suggestions or soldering:
* Use a breadboard or vise or pair of extra hands when soldering for pin extensions.
* When crimping without crimping tool, use plyers to bend both end pieces together then crimp the tips down towards the wire. (Illustrations coming soon)

## Extra Steps

In here, I will explain the other steps I have taken to make the device less capable of spoiling from shaking, dropping or vibrations.

![extrasteps](https://user-images.githubusercontent.com/91014375/155720772-67bb397a-507e-4f64-bd19-378b923cd68d.jpg)


<p align="center">
  </div>
  More Coming Soon
    <br />


![PROJECT_YONDU_Logo](https://user-images.githubusercontent.com/91014375/155701599-c7715ec6-b693-4936-aedf-efa84dc7cbbf.png)
