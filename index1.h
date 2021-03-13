//index1.h
const char HTML1[] PROGMEM = R"====(
<!DOCTYPE HTML>
<html>

<head>
    <title>BME680, BSEC Studies</title>
</head>
<body>
    <h1> BME680 BSEC Studies</h1>
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
    <h2><a href="%LINK%/SdBrowse" >BME680 BSEC File Browser</a>
    <br>
    <a href="%LINK%/Graphs" >BME680 BSEC Graphs</a>
    <br>
    <br> Client IP: %CLIENTIP%</h2>
</body>

</html>
)====";
