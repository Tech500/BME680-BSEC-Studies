# BME680-BSEC-Studies
Data Logger with graphing of IAQ, Gas Resistance, CO2, and VOC. Plus web interface and OLed display.

Example of use in graphing: Soaked a Q-tip in acetone, placed Q-tip very close to the BME680.  Start up requires 30 minutes for the BME680 to be ready.  Sketch will display when thirty minutes have past and test can begin. Default time for test to run is for 90 dataPoints, (three hours); which can be easily modified in "config.h," count variable specifies duration of test. Data points are created every two minutes. When count is reached sketch will display notification "End of Test -- Disconnect USB."

Free "Thingspeak.com" account is needed to create graphs: https://community.thingspeak.com/tutorials/arduino/send-data-to-thingspeak-with-arduino/

Prokect Graph for Acetone test:  https://github.com/Tech500/BME680-BSEC-Studies/blob/main/Gas%20Study%20Graphs%20--IAQ%2CResistanc%2C%20CO2%2C%20and%20VOC.JPG


