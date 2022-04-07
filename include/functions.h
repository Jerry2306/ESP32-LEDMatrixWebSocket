void webpage()
{
    server.send(200, "text/html", webpageCode);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t welength)
{
    if (type == WStype_CONNECTED)
    {
        uint8_t ledCnt = 0;
        int i;
        for (i = 0; i < LEDCount; i++)
        {
            if (leds[i].r == 0 && leds[i].g == 0 && leds[i].b == 0)
                continue;

            ledCnt++;
        }

        Serial.println(ledCnt);

        uint8_t ledByteArray[ledCnt * 4];
        uint8_t ledByteArrayIndex = 0;

        for (i = 0; i < LEDCount; i++)
        {
            if (leds[i].r == 0 && leds[i].g == 0 && leds[i].b == 0)
                continue;

            if (ledByteArrayIndex * 4 + 3 >= ledCnt * 4)
                break;

            ledByteArray[ledByteArrayIndex * 4] = i;
            ledByteArray[ledByteArrayIndex * 4 + 1] = leds[i].r;
            ledByteArray[ledByteArrayIndex * 4 + 2] = leds[i].g;
            ledByteArray[ledByteArrayIndex * 4 + 3] = leds[i].b;
            ledByteArrayIndex++;
        }

        webSocket.sendBIN(num, ledByteArray, ledCnt * 4);
    }

    if (type == WStype_BIN)
    {
        try
        {
            FastLED.clear();

            for (int i = 0; i < welength; i += 4)
                leds[payload[i]] = CRGB(payload[i + 1], payload[i + 2], payload[i + 3]);

            FastLED.show();

            webSocket.broadcastBIN(payload, welength);
        }
        catch (...)
        {
            Serial.println("ERROR parsing data...");
        }
    }
}