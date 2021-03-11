//index4.h
const char HTML4[] PROGMEM = R"====(
<!DOCTYPE HTML>
<html>

<head>
    <title>BME680 Graphs</title>
</head>

<body>
  <br>
	<h2>BME680 Gas Study</h2>
	<br>
  <frameset rows="30%,70%" cols="33%,34%">
  <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1323947/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>
  <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1323947/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>
  </frameset>
  <br><br>
  <frameset rows="30%,70%" cols="33%,34%">
  <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1323947/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>
 <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1323947/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>
  </frameset>
 	<br><br>
  
  <h2><a href="%LINK%/BME680" >Home</a></h2>
  <br>
</body>

</html>
)====";
