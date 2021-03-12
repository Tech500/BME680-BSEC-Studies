/*
** 
**                      BME680 BSEC Gas Study  3/11/2021 @ 07:58 EST  --ESPAsyncWebserver version
**                      William Lucid
**
 * Original code from Rui Santos
 * Project Details https://RandomNerdTutorials.com
 * *
 * *    Robert Gillespie's  "OLED_BME680.odt"  --bases of modified code
 * *
 * *    BME680_DataLogger_Webserver.ino modifion code 3/3/2021 William Lucid's
 *
 */

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>  //https://github.com/me-no-dev/AsyncTCP
#include <ESPAsyncWebServer.h>  //https://github.com/me-no-dev/ESPAsyncWebServer
#include "FTPServer.h"
#include <WiFiUdp.h>
#include "FS.h"
#include <LittleFS.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "bsec.h"
#include <ThingSpeak.h>   //https://github.com/mathworks/thingspeak-arduino  --> Used for ThingSpeak.com graphing. Get it using the Library Manager.  Requires an account

#include "config.h" //Edit your data.......................

#import "index1.h"  //BME680 HTML do not remove
#import "index2.h"  //SdBrowse HTML do not remove
#import "index3.h"  //Graph1 HTML do not remove
#import "index4.h"  //Graph2 HTML do not remove

//Edit in config.h
//Replace with your network details
//const char* ssid
//const char* password

//Edit in config.h
//String linkAddress = "http://xxx.xxx.xxx.xxx:8060";  //WAN --publicIP and PORT for URL link
//String linkAddress = "http://10.0.0.110:8060";  //LAN --privateIP and PORT for URL link

//AsyncWebServer
AsyncWebServer serverAsync(8060);
AsyncWebSocket ws("/ws"); // access at ws://[esp ip]/ws
AsyncEventSource events("/events"); // event source (Server-Sent events)

void onRequest(AsyncWebServerRequest *request)
{
     //Handle Unknown Request
     request->send(404);
}

void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
     //Handle body
}

void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
     //Handle upload
}

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
{
     //Handle WebSocket event
}

WiFiClient client;

IPAddress ipREMOTE;

char* filelist[12];

String logging;

char *filename;
String fn;
char str[] = {0};
char MyBuffer[13];
String fileRead;
String PATH;

int flag;

#define online 19  //pin for online LED indicator

#define SPIFFS LittleFS

// tell the FtpServer to use LittleFS
FTPServer ftpSrv(LittleFS);

///Are we currently connected?
boolean connected = false;

WiFiUDP udp;
// local port to listen for UDP packets

/*
//Edit in config.h
const int udpPort = 1337;
char incomingPacket[255];
char replyPacket[] = "Hi there! Got the message :-)";
const char * udpAddress1 = "us.pool.ntp.org";
const char * udpAddress2 = "time.nist.gov";
*/


// Helper functions declarations
void checkIaqSensorStatus(void);
void errLeds(void);

// Create an object of the class Bsec
Bsec iaqSensor;

String output;

float baroOffset = .88;  //inHg
float tempOffset = .9;  //Degrees F.

float temperatureF = (((iaqSensor.temperature * 1.8 ) + 32) + tempOffset);  //Degrees F. 
float humidity = (iaqSensor.humidity);  //rH %
float baro =  (((iaqSensor.pressure) / 3386.39) + baroOffset);  //inHg
float gasResistance = (iaqSensor.gasResistance) / 1000.0;  //KOhms
float gasCO2 = (iaqSensor.co2Equivalent);
float gasVOC = (iaqSensor.breathVocEquivalent);

float sensor = (iaqSensor.iaq);

String IAQ;  //String for Air Quality

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32 <<-- My Oled uses 0x3D since it is 128 X 64 SCREEN ADDRESS
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Time settings
//Edit in config.h
//#define TZ "EST+5EDT,M3.2.0/2,M11.1.0/2"
int DOW, MONTH, DATE, YEAR, HOUR, MINUTE, SECOND;

char strftime_buf[64];
String dtStamp(strftime_buf);
String lastUpdate;
int lc = 0;
time_t tnow = 0;

int y;  //Used for inplace of delay(time) in "for" statement

//Edit in config.h
//int count --number of datapoits in channel 2 Graphs

int dataPoint;  //Data point counter

void wifi_Start()
{

     WiFi.mode(WIFI_STA);
     WiFi.disconnect();

     Serial.println();
     Serial.print("MAC: ");
     Serial.println(WiFi.macAddress());

     // We start by connecting to a WiFi network
     Serial.print("Connecting to ");
     Serial.println(ssid);

     WiFi.begin(ssid, password);

     //setting the static addresses in function "wifi_Start
     IPAddress ip {10,0,0,110};
     IPAddress gateway {10,0,0,1};
     IPAddress subnet {255,255,255,0};
     IPAddress dns {10,0,0,1};

     WiFi.config(ip, gateway, subnet, dns);

     // Starting the web server
     //server.begin();
     Serial.println("Waiting for the ESP32 IP...");

     // Printing the ESP IP address
     Serial.print("Server IP:  ");
     Serial.println(WiFi.localIP());
     Serial.print("Port:  ");
     Serial.print("80");
     Serial.println("\n");

     WiFi.waitForConnectResult();

     Serial.printf("Connection result: %d\n", WiFi.waitForConnectResult());

     if ((WiFi.status() != WL_CONNECTED))
     {

          if(MINUTE % 20 == 0)
          {

               wifi_Start();

          }

          serverAsync.begin();



     }


}

/*


    This is the ThingSpeak channel number for the MathwWorks weather station
     https://thingspeak.com/channels/YourChannelNumber.  
     
	Fields of the channel 1:

     Field 1 - Temperature (Degrees F)
     Field 2 - Humidity (%RH)
     Field 3 - Barometric Pressure (in Hg)
     Field 4 - IAQ (Indoor Air Qualitiy)
	
	Fields of the channel 2:

     Field 1 - IAQ (Indoor Air Qualitiy)
     Field 2 - Gas Resistance KOhms
     Field 3 - CO2
     Field 4 - VOC
	
*/

//     Graphing requires "FREE" "ThingSpeak.com" account..
//     https://community.thingspeak.com/tutorials/arduino/send-data-to-thingspeak-with-arduino/

//Edit in config.h
//unsigned long myChannelNumber 
//const char * myWriteAPIKey


void setup()
{

     Serial.begin(115200);

     while(!Serial) delay(5 * 1000);

     Serial.println(F("\n\n\nBME680 BSEC Gas Study  3/11/2021 @ 07:58 EST "));

     wifi_Start();

     Wire.begin(4, 5);

     ThingSpeak.begin(client);  // Initialize ThingSpeak

     serverAsync.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest * request)
     {
          PATH = "/FAVICON";
          //accessLog();
          if (! flag == 1)
          {
               request->send(SPIFFS, "/favicon.png", "image/png");

          }
          //end();
     });

     serverAsync.on("/BME680", HTTP_GET, [](AsyncWebServerRequest * request)
     {

          PATH = "/BME680";
          accessLog();

          ipREMOTE = request->client()->remoteIP();

          if (! flag == 1)
          {
               request->send_P(200, PSTR("text/html"), HTML1, processor1);
          }
          end();
     });

     serverAsync.on("/SdBrowse", HTTP_GET, [](AsyncWebServerRequest * request)
     {
          PATH = "/SdBrowse";
          accessLog();
          if (! flag == 1)
          {
               request->send_P(200, PSTR("text/html"), HTML2, processor2);

          }
          end();
     });

     serverAsync.on("/Graph1", HTTP_GET, [](AsyncWebServerRequest * request)
     {
          PATH = "/Graph1";
          accessLog();
          AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", HTML3, processor3);
          response->addHeader("Server", "ESP Async Web Server");
          if (! flag == 1)
          {
               request->send(response);

          }
          end();
     });
	
     serverAsync.on("/Graph2", HTTP_GET, [](AsyncWebServerRequest * request)
     {
          PATH = "/Graph2";
          accessLog();
          AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", HTML4, processor4);
          response->addHeader("Server", "ESP Async Web Server");
          if (! flag == 1)
          {
               request->send(response);

          }
          end();
     });

     serverAsync.onNotFound(notFound);

     serverAsync.begin();

     bool fsok = LittleFS.begin();
     Serial.printf_P(PSTR("FS init: %s\n"), fsok ? PSTR("ok") : PSTR("fail!"));

     //LittleFS.format();

     // setup the ftp server with username and password
     // ports are defined in FTPCommon.h, default is
     //   21 for the control connection
     //   50009 for the data connection (passive mode by default)
     ftpSrv.begin(F("ftp"), F("ftp")); //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)


     // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
     display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
     // init done
     display.display();

     display.clearDisplay();
     display.display();
     display.setTextSize(1);
     display.setTextColor(WHITE);

     iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire);
     output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
     Serial.println(output);
     checkIaqSensorStatus();

    bsec_virtual_sensor_t sensorList[10] = {
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_RAW_GAS,
    BSEC_OUTPUT_IAQ,
    BSEC_OUTPUT_STATIC_IAQ,
    BSEC_OUTPUT_CO2_EQUIVALENT,
    BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
  };

  iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus();

     configTime(0, 0, udpAddress1, udpAddress2);
     setenv("TZ", "EST+5EDT,M3.2.0/2,M11.1.0/2", 3);   // this sets TZ to Indianapolis, Indiana
     tzset();

     Serial.print("wait for first valid timestamp ");


     while (time(nullptr) < 100000ul)
     {
          Serial.print(".");

          delay(5000);

          Serial.print(".");

     }
    
     Serial.println("\nSystem Time set");

     getDateTime();

     Serial.println(dtStamp);

     dataPoint = 0;

     Serial.println("\n\nAllow at least 15 dataPoints (30 Minutes) before starting Test");

}

void loop()
{

     //udp only send data when connected
     if (connected)
     {

          //Send a packet
          udp.beginPacket(udpAddress1, udpPort);
          udp.printf("Seconds since boot: %u", millis() / 1000);
          udp.endPacket();
     }

     getWeatherData();

     airQuality();

     for(int x; x < 5000; x++)
     {
          ftpSrv.handleFTP();
     }

     //Convert float to String
     
     getDateTime();

     if(SECOND ==  0)
     {

          delay(1000);

          screenOne();

          exit;
          
     }

     if(SECOND == 30)
     {

          delay(1000);

          screenTwo();

          exit;

    }

    if(dataPoint > count)
    {

        Serial.println("\n\nEnd of Test --Disconnect USB Cable");

        exit;
                   
    }
    else if((MINUTE % 15 == 0) || (MINUTE % 2 == 0) && (SECOND == 0))  //data point ever 2 minutes
    {
    
        dataPoint++;

        Serial.println("\n\ndataPoint:  " + String(dataPoint));

        if(dataPoint == 15)  //Allow BME680 to heat 30 Minutes
        {

              Serial.println("\n\nStart Test --BME680 has been 'ON' 30 Minutes");

              exit;

        }
      
        if (dataPoint < count) 
        {

            thingSpeakTwo();

            serialMonitor();

            delay(1000);

        }
        else
        {

            serialMonitor();

        }

    }
     
     

          

     if((MINUTE % 15 == 0) && (SECOND < 2))
     {

          delay(1000);

          //airQuality();

          logtoSPIFFS();

          thingSpeakOne();

          exit;

     }

     if ((HOUR == 23) && (MINUTE == 58) && (SECOND == 30)) //Start new kog file..
     {

          fileStore();

     }

}

String processor1(const String& var)
{

     //index1.h 

     getDateTime();

     String str;

     if (var == F("TEMP"))
          return String(temperatureF);

     if (var == F("HUM"))
          return String(humidity);

     if (var == F("SEALEVEL"))
          return String(baro);

     if (var == F("GAS"))
          return String(gasResistance);    

     if (var == F("CO2"))
          return String(gasCO2);

     if (var == F("VOC"))
          return String(gasVOC);

     if (var == F("AIR"))
          return (IAQ);

     if (var == F("BME680"))
          return String(sensor);

     if (var == F("TIME"))
          return String(dtStamp);
      

     if (var == F("CLIENTIP"))
          return ipREMOTE.toString().c_str();

     if (var == F("%LINK%"))
          return linkAddress;


     return String();

}

String processor2(const String& var)
{

     //index2.h

     String str;

     if (!SPIFFS.begin())
     {
          Serial.println("SPIFFS failed to mount !");
     }
     Dir dir = SPIFFS.openDir("/");
     while (dir.next())
     {

          String file_name = dir.fileName();

          if (file_name.startsWith("LOG"))
          {

               str += "<a href=\"";
               str += dir.fileName();
               str += "\">";
               str += dir.fileName();
               str += "</a>";
               str += "    ";
               str += dir.fileSize();
               str += "<br>\r\n";
          }
     }

     client.print(str);

     if (var == F("URLLINK"))
          return  str;

     if (var == F("FILENAME"))
          return  dir.fileName();

     if (var == F("%LINK%"))
          return linkAddress;

     return String();

}

String processor3(const String& var)
{

     //index3.h

     if (var == F("%LINK%"))
          return (linkAddress);

     return String();

}

String processor4(const String& var)
{

     //index4.h

     if (var == F("%LINK%"))
          return (linkAddress);

     return String();

}


void accessLog()
{

     digitalWrite(online, HIGH);  //turn on online LED indicator

     getDateTime();

     String ip1String = "10.0.0.146";   //Host ip address
     String ip2String = ipREMOTE.toString().c_str();   //client remote IP address
     String returnedIP = ip2String;

     Serial.println("");
     Serial.println("Client connected:  " + dtStamp);
     Serial.print("Client IP:  ");
     Serial.println(returnedIP);
     Serial.print("Path:  ");
     Serial.println(PATH);
     Serial.println(F("Processing request"));

     //Open a "access.txt" for appended writing.   Client access ip address logged.
     File logFile = SPIFFS.open("/ACCESS.TXT", "a");

     if (!logFile)
     {
          Serial.println("File 'ACCESS.TXT'failed to open");
     }

     if ((ip1String == ip2String) || (ip2String == "0.0.0.0") || (ip2String == "(IP unset)"))
     {

          //Serial.println("HOST IP Address match");
          logFile.close();

     }
     else
     {

          Serial.println("Log client ip address");

          logFile.print("Accessed:  ");
          logFile.print(dtStamp);
          logFile.print(" -- Client IP:  ");
          logFile.print(ip2String);
          logFile.print(" -- ");
          logFile.print("Path:  ");
          logFile.print(PATH);
          logFile.println("");
          logFile.close();

     }

}

void end()
{

     digitalWrite(online, LOW);   //turn-off online LED indicator

     getDateTime();

     Serial.println("Client closed:  " + dtStamp);

}

void fileStore()   //If Midnight, rename "LOGXXYYZZ.TXT" to ("log" + month + day + ".txt") and create new, empty "LOGXXYYZZ.TXT"
{

     int temp;
     String Date;
     String Month;

     temp = (DATE);
     if (temp < 10)
     {
          Date = ("0" + (String)temp);
     }
     else
     {
          Date = (String)temp;
     }

     temp = (MONTH);
     if (temp < 10)
     {
          Month = ("0" + (String)temp);
     }
     else
     {
          Month = (String)temp;
     }

     String logname;  //file format /LOGxxyyzzzz.txt
     logname = "/LOG";
     logname += Month;   ///logname += Clock.getDate();
     logname += Date; ////logname += Clock.getMonth(Century);
     logname += YEAR;
     logname += ".TXT";

     //Open file for appended writing
     File log = SPIFFS.open(logname.c_str(), "a");

     if (!log)
     {
          Serial.println("file open failed");
     }

}

String getDateTime()
{
     struct tm *ti;

     tnow = time(nullptr) + 1;
     ti = localtime(&tnow);
     DOW = ti->tm_wday;
     YEAR = ti->tm_year + 1900;
     MONTH = ti->tm_mon + 1;
     DATE = ti->tm_mday;
     HOUR  = ti->tm_hour;
     MINUTE  = ti->tm_min;
     SECOND = ti->tm_sec;

     strftime(strftime_buf, sizeof(strftime_buf), "%a , %m/%d/%Y , %H:%M:%S %Z", localtime(&tnow));
     dtStamp = strftime_buf;
     return (dtStamp);

}

void getWeatherData()
{

    unsigned long time_trigger = millis();
    if (iaqSensor.run()) 
    { //     

       temperatureF = (((iaqSensor.temperature * 1.8 ) + 32) + tempOffset);  //Degrees F. 
       baro = (((iaqSensor.pressure) / 3386.39) + baroOffset);  //inHg  
       humidity = (iaqSensor.humidity);  //rH %
       gasResistance = ((iaqSensor.gasResistance) / 1000.0);  //KOhms
       gasCO2 = (iaqSensor.co2Equivalent);
       gasVOC = (iaqSensor.breathVocEquivalent); 

       sensor = (iaqSensor.iaq);

    }
    else 
    {
      checkIaqSensorStatus();
    }

}

float logtoSPIFFS()   //Output to SPIFFS every fifthteen minutes
{

     int header = 0;;

     int tempy;
     String Date;
     String Month;

     tempy = (DATE);
     if (tempy < 10)
     {
          Date = ("0" + (String)tempy);
     }
     else
     {
          Date = (String)tempy;
     }

     tempy = (MONTH);
     if (tempy < 10)
     {
          Month = ("0" + (String)tempy);
     }
     else
     {
          Month = (String)tempy;
     }

     String logname;
     logname = "/LOG";
     logname += Month;   ///logname += Clock.getDate();
     logname += Date; ////logname += Clock.getMonth(Century);
     logname += YEAR;
     logname += ".TXT";

     // Open a "log.txt" for appended writing
     File log = SPIFFS.open(logname.c_str(), "a");

     if (!log)
     {
          Serial.println("file 'LOG.TXT' open failed");
     }

     if ((HOUR == 0) && (MINUTE == 0) && (SECOND < 3)) //Create header
     {
          header = 1;
     }

     if(header == 1)
     {

          log.print("BME680 Bsec Data Logger   ");
          log.println(dtStamp);
          log.println("");

          header = 0;

     }

     log.print(dtStamp);
     log.print("  , ");

     airQuality();

     log.print("Air Qualitiy:  ");
     log.print(IAQ);
     log.print("  , ");

     log.print("BME680 IAQ:  ");
     log.print(sensor);
     log.print("  , ");

     log.print("Gas Resistance:  ");
     log.print(gasResistance);
     log.print(" KOhms , ");

     log.print("CO2:  ");
     log.print(gasCO2);
     log.print(" KOhms , ");

     log.print("VOC:  ");
     log.print(gasVOC);
     log.print(" KOhms , ");

     log.print("Temperature:  ");
     log.print(temperatureF, 1);
     log.print(" F. , ");

     log.print("Humidity:  ");
     log.print(humidity);
     log.print(" % ");
     log.print(" , ");

     log.print("Pressure:  ");
     log.print(baro, 3);
     log.print(" inHg. ");
     log.println("");

     log.close();

     Serial.println("\nData written to  " + logname + "  " + dtStamp);

}

//readFile  --AsyncWebServer version with much help from Pavel
void notFound(AsyncWebServerRequest *request)
{

     digitalWrite(online, HIGH);   //turn-on online LED indicator

     if (! request->url().endsWith(F(".TXT")))
     {
          request->send(404);
     }
     else
     {
          if (request->url().endsWith(F(".TXT")))
          {
               //.endsWith(F(".txt")))

               // here comes some mambo-jambo to extract the filename from request->url()
               int fnsstart = request->url().lastIndexOf('/');

               fn = request->url().substring(fnsstart);

               PATH = fn;

               accessLog();

               Serial.print("File:  ");
               Serial.println(fn);



               File webFile = SPIFFS.open(fn, "r");

               Serial.print("File size: ");

               Serial.println(webFile.size());

               if (!webFile)
               {

                    Serial.println("File:  " + fn + " failed to open");
                    Serial.println("\n");

               }
               else if (webFile.size() == 0)
               {

                    webFile.close();

               }
               else
               {

                    //request->send(SPIFFS, fn, String(), true);  //Download file
                    request->send(SPIFFS, fn, String(), false);   //Display file

                    webFile.close();

               }

               fn = "";

               end();

          }

     }

     digitalWrite(online, LOW);   //turn-off online LED indicator

}

void screenOne()
{


     //----------- Start First Screen

     display.setCursor(0,0);
     display.clearDisplay();
     display.setCursor(0,18);
     display.setTextSize(1);
     display.print("Temp: ");
     display.setCursor(45,18);
     display.print(String(temperatureF));
     display.setCursor(82,18);
     display.print("F");

     display.setCursor(0,30);
     display.setTextSize(1);
     display.print("Press: ");
     display.setCursor(45,30);
     display.print(String(baro));
     display.setCursor(82,30);
     display.print("inHg");

     display.setCursor(0,42);
     display.setTextSize(1);
     display.print("Hum: ");
     display.setCursor(45,42);
     display.print(String(humidity));
     display.setCursor(82,42);
     display.print("%");

      display.display();

}

void screenTwo()
{

     //-------------- Start Second Screen

     display.setCursor(0,0);
     display.clearDisplay();

     display.setCursor(3,18);
     display.setTextSize(1);
     display.print("Air:");
     display.setCursor(32,18);
     display.print(IAQ);
         
	   display.setCursor(3,30);
     display.setTextSize(1);
     display.print("Gas:  ");
     display.setCursor(34,30);
     display.print(String(gasResistance));
     display.setCursor(84,30);
     display.print("KOhms");
     
	   display.setCursor(3,42);
     display.setTextSize(1);
     display.print("CO2: ");
     display.setCursor(34,42);
     display.print(String(gasCO2));
     display.setCursor(84,42);
     display.print("KOhms");

     display.setCursor(3,54);
     display.setTextSize(1);
     display.print("VOC:  ");
     display.setCursor(34,54);
     display.print(String(gasVOC));
     display.setCursor(84,54);
     display.print("KOhms");
  
     display.display();

}

void airQuality()
{

     if((sensor >= 0) && (sensor <= 50))
     {

          IAQ = "Good";
     }

     if((sensor >= 51) && (sensor <= 100))
     {

          IAQ = "Aver.";
     }

     if((sensor >= 101) && (sensor <= 150))
     {

          IAQ = "Less Avg.";
     }

     if((sensor >= 151) && (sensor <= 200))
     {

          IAQ = "Bad";
     }

     if((sensor >= 201) && (sensor <= 300))
     {

          IAQ = "Poor";

     }

     if((sensor >= 301) && (sensor <= 500))
     {

          IAQ = "Very Poor";

     }

}

void serialMonitor()
{

     Serial.print("\nTemperature:     " + String(temperatureF) + " F "); ;
     Serial.print("\nPressure:        " + String(baro) + " inHg");
     Serial.print("\nHumidity:        " + String(humidity) + " %");
     Serial.print("\nGas:             " + String(gasResistance) + " KOhms");
     Serial.print("\nCO2:             " + String(gasCO2) + " KOhms");
     Serial.print("\nVOC:             " + String(gasVOC) + " KOhms");
    	
     Serial.println("\n\nAir Quality:  " + IAQ + "");
     Serial.println("BME680 IAQ:  " + String(sensor));
     Serial.println(dtStamp);

}

void thingSpeakOne()
{

    char fltemperatureF[15];
    dtostrf(temperatureF, 2, 3, fltemperatureF);
    char flhumidity[15];
    dtostrf(humidity, 2, 3, flhumidity);
    char flpressure[15];
    dtostrf(baro, 2, 3, flpressure);
    char fliaq[15];
    dtostrf(sensor,6, 3, fliaq);


     // set the fields with the values
     ThingSpeak.setField(1, fltemperatureF);
     ThingSpeak.setField(2, flhumidity);
     ThingSpeak.setField(3, flpressure);
     ThingSpeak.setField(4, fliaq);

     // write to the ThingSpeak channel
     int x = ThingSpeak.writeFields(myChannelNumber1, myWriteAPIKey1);

     if(x == 200)
     {
          Serial.println("\nChannel 1 update successful.");
     }
     else
     {
          Serial.println("\nProblem updating channel 1. HTTP error code " + String(x));
     }

     for( y = 1; y < 1000; y++);

}

void thingSpeakTwo()
{

    char fliaq[15];
    dtostrf((sensor), 2, 3, fliaq);
    char flgas[15];
    dtostrf(((gasResistance) / 1000.0),6, 3, flgas);
    char flCO2[15];
    dtostrf((gasCO2), 2, 3, flCO2);
    char flVOC[15];
    dtostrf((gasVOC), 2, 3, flVOC);
    
     // set the fields with the values
     ThingSpeak.setField(1, fliaq);
     ThingSpeak.setField(2, flgas);
     ThingSpeak.setField(3, flCO2);
     ThingSpeak.setField(4, flVOC);
     
     // write to the ThingSpeak channel
     int x = ThingSpeak.writeFields(myChannelNumber2, myWriteAPIKey2);

     if(x == 200)
     {
          Serial.println("\nChannel 2 update successful.");
     }
     else
     {
          Serial.println("\nProblem updating channel 2. HTTP error code " + String(x));
     }

     for( y = 1; y < 1000; y++);

}
// Helper function definitions
void checkIaqSensorStatus(void)
{
	if (iaqSensor.status != BSEC_OK) 
	{
	  
		if (iaqSensor.status < BSEC_OK) 
		{
		    
		 output = "BSEC error code : " + String(iaqSensor.status);
		 Serial.println(output);
		 for (;;)
		   errLeds(); /* Halt in case of failure */
		} 
		else 
		{
		    
		 output = "BSEC warning code : " + String(iaqSensor.status);
		 Serial.println(output);
		 
		}
	}

	if (iaqSensor.bme680Status != BME680_OK) 
	{
		
		if (iaqSensor.bme680Status < BME680_OK) 
		{
			
			output = "BME680 error code : " + String(iaqSensor.bme680Status);
			Serial.println(output);
			
			for (;;)
		   
			errLeds(); /* Halt in case of failure */
		} 
		else 
		{
			
			output = "BME680 warning code : " + String(iaqSensor.bme680Status);
			Serial.println(output);

		}
	}
}

void errLeds(void)
{
	
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
	for( y = 1; y < 100; y++);
	digitalWrite(LED_BUILTIN, LOW);
	for( y = 1; y < 100; y++);
  
}
