//index2.h
const char HTML3[] PROGMEM = R"====(
<!DOCTYPE HTML>
<html>

<head>
    <title>BME680 Graphs</title>
</head>

<body>
  <br>
	<h2>BME680 Environmental Observations</h2>
	<br>
  <frameset rows="30%,70%" cols="33%,34%">
  <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1320323/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=BME680+Temperature&type=line&xaxis=Date+and+Time&yaxis=Temperature+F."></iframe>
  <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1320323/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=BME680+Humidity&type=line&xaxis=Date+and+Time&yaxis=Humidity"></iframe>
  </frameset>
  <br><br>
  <frameset rows="30%,70%" cols="33%,34%">
  <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1320323/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=BME680+Pressure&type=line&xaxis=Date+and+Time&yaxis=Pressure+inHg."></iframe>
  <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1320323/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=BME680+Gas&type=line&xaxis=Date+and+Time&yaxis=Gas+Res++KOhms"></iframe>
  </frameset>
 	<br><br>
  <h2><a href="%LINK%/BME680" >Home</a></h2>
  <br>
</body>

</html>
)====";
