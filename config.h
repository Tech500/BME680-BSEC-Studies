/*

	"BME BSEC Gas Study.ino"
	config.h  William Lucid
	3/11/2021 @ 01:18 EST 
	
*/

// Replace with your network details
const char* ssid = "yourSSID";
const char* password = "yourPASSWORD";

//String linkAddress = "http://xxx.xxx.xxx.xxx:8060";  //WAN --publicIP and PORT for URL link
String linkAddress = "http://10.0.0.110:8060";  //LAN --privateIP and PORT for URL link

// local port to listen for UDP packets
const int udpPort = 1337;
char incomingPacket[255];
char replyPacket[] = "Hi there! Got the message :-)";
const char * udpAddress1 = "us.pool.ntp.org";
const char * udpAddress2 = "time.nist.gov";

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

//Graphing requires "FREE" "ThingSpeak.com" account..
//Enter "ThingSpeak.com" data here....
unsigned long myChannelNumber1 = 1234567890;
const char * myWriteAPIKey1 = "9abcdefg";
unsigned long myChannelNumber2 = 11234567890;
const char * myWriteAPIKey2 = "abcdefg";

int count =90;  //default = 90 data points one every 2 minutes

int interval = 2;  //time between dataPoints  --60 Minutes needs to be divided evenly, no reminder   
//(MINUTES % interval == 0)  default =60/2 = 40 dataPoints prt Hour

int wait = 15; //Number of datapoints to wait --pre-heating BME680


