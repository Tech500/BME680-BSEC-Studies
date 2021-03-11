/*

	"BME BSEC Gas Study.ino"
	config.h  William Lucid
	3/11/2021 @ 01:18 EST 
	
*/

// Replace with your network details
const char* ssid = "R2D2";
const char* password = "sissy4357";

//String linkAddress = "http://73.102.122.239:8060";  //WAN --publicIP and PORT for URL link
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
unsigned long myChannelNumber1 = 1320323;
const char * myWriteAPIKey1 = "95NA12W3LS7IOE65";
unsigned long myChannelNumber2 = 1323947;
const char * myWriteAPIKey2 = "CKZ7R5FYV9XRI1WA";

int count = 90;  //180 minutes; data points every 2 minutes

