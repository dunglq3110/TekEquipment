#include "WServer.h"

WServer::WServer(Communication *comm, PlayerData *data, PingPong *ping) : _communication(comm), _playerData(data), _ping(ping), _server(80)
{
    _espNowInstance = new EspNow(_communication, _playerData, _ping);
}

void WServer::init()
{
    /* Init Wifi to get MAC, otherwise we get continuous crashes */
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.macAddress(_mac);

    /* Take the MAC and make it the name of the device shown in the list of discovered WiFis on the phone*/
    for (uint8_t i = 0; i < 6; i++)
    {
        _ssid += String(_mac[i], HEX);
        if (i < 5)
            _ssid += ":";
    }

    /* Uppercase the thing, just to look nice */
    _ssid.toUpperCase();
}

void WServer::setup()
{
    /* Receives credentials from WebServer, if valid, save it to preferences for later uses */
    if (_server.hasArg("ssid") && _server.hasArg("password"))
    {
        _wifiName = (_server.arg("ssid")).c_str();
        _wifiPass = (_server.arg("password")).c_str();
        _pref.begin("wifi", false);
        _pref.putString("_wifiName", _wifiName.c_str());
        _pref.putString("_wifiPass", _wifiPass.c_str());
        _pref.end();

        /* Notify the WebServer that the esp has received the credentials */
        _server.send(200, "text/plain", "Received");
        WiFi.softAPdisconnect(true);
        delay(2000);
        ESP.restart();
    }
}

uint8_t WServer::getWiFiChannel()
{
    return _channel;
}

void WServer::handleClient()
{
    /* Handler for WebServer's credentials waiting */
    _server.handleClient();
}

uint8_t WServer::findWiFiChannel()
{
    /* Get the WiFi name from preferences to search for free channel */
    _pref.begin("wifi", true);
    const char *ssid = _pref.getString("_wifiName", "").c_str();
    _pref.end();

    /* See if there are any available channels */
    int16_t n = WiFi.scanNetworks(false, false, false, 300U);
    if (n > 0)
    {
        for (uint8_t i = 0; i < n; i++)
        {
            if (!strcmp(ssid, WiFi.SSID(i).c_str()))
            {
                Serial.print("Free channel: ");
                Serial.println(WiFi.channel(i));
                _channel = WiFi.channel(i);
                return WiFi.channel(i);
            }
        }
    }
    /* Just return 13 (invalid) if nothing were found */
    Serial.println("No free channels available");
    WiFi.scanDelete();
    _channel = 13;
    return 13;
}

void WServer::connect()
{
    /* Take WiFi credentials out of preferences*/
    _pref.begin("wifi", true);
    _wifiName = (_pref.getString("_wifiName", "")).c_str();
    _wifiPass = (_pref.getString("_wifiPass", "")).c_str();
    _pref.end();

    /* Attempt to connect to that WiFi */
    WiFi.begin(_wifiName.c_str(), _wifiPass.c_str());

    /* Wait for 5 seconds for WiFi to connect */
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - startTime >= 5000)
        {
            break;
        }
    }

    /* If success */
    if (WiFi.status() == WL_CONNECTED)
    {
        isConnected();
    }

    /* If unsuccess */
    else
    {
        unConnected();
    }
}

void WServer::unConnected()
{
    /* Create a local Webserver of 192.168.4.1 to wait for the new WiFi credentials */
    WiFi.softAP(_ssid.c_str());
    _server.on("/setup", [this]()
               { this->setup(); });
    WiFi.setAutoReconnect(false);
    _server.begin();
}

void WServer::isConnected()
{
    /* Set IP to local IP */
    IPAddress myIP = WiFi.localIP();
    Serial.print("Local IP address: ");
    Serial.println(myIP);
    WiFi.setAutoReconnect(true);
}