# BME680-BSEC-Studies
Data Logger with graphing of IAQ, Gas Resistance, CO2, and VOC.  Plus web interface and OLed display.

Example of use in charting:  Soaked a Q-tip in acetone, placing Q-tip very close to the BME680.  Start up requires 30 minutes for the BME680 to be ready.  
Sketch will display when thirty minutes hacve pasted and test can begin.  Default time for test to run is 3 hours; which can be easily modified in "config.h," 
count variable specifies duration of test.  Data points are created every two minutes.  When count is reached sketch will display notification "End of Test --
Disconnect USB."

Free "Thingspeak.com" account is needed to create graphys:  https://community.thingspeak.com/tutorials/arduino/send-data-to-thingspeak-with-arduino/

