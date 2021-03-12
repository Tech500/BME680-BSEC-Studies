//index3.h
const char HTML3[] PROGMEM = R"====(
<!DOCTYPE HTML>  
<html>

<head>  
    <title>BME680 BSEC Graphs</title>
</head>

<body>
  <br>
	<h2>BME680 Gas Study</h2>
	<br>
  <frameset rows="30%,70%" cols="33%,34%">
  <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1323947/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=BME680+Temperature&type=line&xaxis=Date+and+Time&yaxis=Temperature+"></iframe>
  <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1323947/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=BME680+Humidity&type=line&xaxis=Date+and+Time&yaxis=Humidity"></iframe>
  <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1323947/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=BME680+Pressure&type=line&xaxis=Date+and+Time&yaxis=Pressure"></iframe>
   </frameset>
  <br><br>
  <frameset rows="30%,70%" cols="33%,34%">
  <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1323947/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=BME680+Gas+Resistance&type=line&xaxis=Date+and+Time&yaxis=Gas+Resistance"></iframe>
  <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1323947/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=BME680+IAQ&type=line&xaxis=Date+and+Time&yaxis=IAQ"></iframe>
  <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1323947/charts/6?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=BME680+CO2&type=line&xaxis=Date+and+Time&yaxis=CO2"></iframe>
  </frameset>
  <br><br>
  <frameset rows="30%,70%" cols="33%,34%">
  <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1323947/charts/7?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=BME680-+VOC&type=line&xaxis=Date+and+Time&yaxis=VOC"></iframe>  
  </frameset>
  <br>
  <h2><a href="%LINK%/BME680" >Home</a></h2>
  <br>
</body>

</html>
)====";
