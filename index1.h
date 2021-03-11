//index1.h
const char HTML1[] PROGMEM = R"====(
<!DOCTYPE HTML>
<html>

<head>
    <title>BME680, BSEC Data Logger</title>
</head>
<body>
    <h1> BME680 Bsec Data Logger</h1>
    <br><h2> Temperature: %TEMP% F.
    <br> Humidity: %HUM% %%
    <br> Barometric Pressure: %SEALEVEL%  inHg.
    <br> GAS: %GAS%  Kohms
    <br> CO2:  %CO2% Kohms
    <br> VOC:  %VOC% Kohms
    <br>
    <br> Air Quality:  %AIR%
    <br> BME680 IAQ: %BME680%
    <br>
    <br> Last Update:  %TIME%
    <br>
    <h2><a href="%LINK%/SdBrowse" >File Browser</a>
    <br>
    <a href="%LINK%/Graph1" >BME680 Environmental Observations</a>
    <br>
    <a href="%LINK%/Graph2" >BME680 Gas Observations</a>
    <br>
    <br> Client IP: %CLIENTIP%</h2>
</body>

</html>
)====";
