# BME680-BSEC-Studies
Data Logger with graphing of IAQ, Gas Resistance, CO2, and VOC. Plus web interface and OLed display.

Example of use in graphing: Soaked a Q-tip in acetone, placed Q-tip very close to the BME680.  Start up requires 30 minutes for the BME680 to be ready.  Sketch will display when thirty minutes have past and test can begin. Default time for test to run is for 90 dataPoints, (three hours); which can be easily modified in "config.h," count variable specifies duration of test. Data points are created every two minutes. When count is reached sketch will display notification "End of Test -- Disconnect USB."

Free "Thingspeak.com" account is needed to create graphs: https://community.thingspeak.com/tutorials/arduino/send-data-to-thingspeak-with-arduino/
Graphing uses "Thingspeak.com" Iframes to display graphs.  Iframe are inserted into HTML code of "index3.h" and "index 4.h". 

Project Graphs for Acetone test: https://github.com/Tech500/BME680-BSEC-Studies/blob/main/BME680%20%20BSEC%20Acetone%20TEST.JPG

"Wemos D1 R2" was used developing project.  FTP was tested using "FileZilla."

