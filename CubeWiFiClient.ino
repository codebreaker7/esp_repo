/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>
#include <SPI.h>
#include <esp32-hal-spi.h>

#define AO_PIN 2
#define DAT 0
#define REG 1

const char* ssid     = "Enterprise";
const char* password = "foo";

const char* host = "192.168.1.101";
//const char* streamId   = "....................";
//const char* privateKey = "....................";
uint8_t buf[32768];
char resp[] = "1\n";

SPIClass spi(HSPI);

void sendByte(uint8_t data, int type) {
    if (type == DAT) {
        digitalWrite(AO_PIN, HIGH);
    } else {
        digitalWrite(AO_PIN, LOW);
    }
    spi.write(data);
}

void initDisplay() {
    sendByte(0x01, REG);
    delay(100);
    sendByte(0x11, REG);
    delay(100);

    sendByte(0x3a, REG);
    sendByte(0x05, DAT);

    sendByte(0x26, REG);
    sendByte(0x04, DAT);

    sendByte(0xf2, REG);
    sendByte(0x01, DAT);

    sendByte(0xE0, REG);
    sendByte(0x3f, DAT);
    sendByte(0x25, DAT);
    sendByte(0x1c, DAT);
    sendByte(0x1e, DAT);
    sendByte(0x20, DAT);
    sendByte(0x12, DAT);
    sendByte(0x2a, DAT);
    sendByte(0x90, DAT);
    sendByte(0x24, DAT);
    sendByte(0x11, DAT);
    sendByte(0x00, DAT);
    sendByte(0x00, DAT);
    sendByte(0x00, DAT);
    sendByte(0x00, DAT);
    sendByte(0x00, DAT);

    sendByte(0xE1, REG);
    sendByte(0x20, DAT);
    sendByte(0x20, DAT);
    sendByte(0x20, DAT);
    sendByte(0x20, DAT);
    sendByte(0x05, DAT);
    sendByte(0x00, DAT);
    sendByte(0x15, DAT);
    sendByte(0xa7, DAT);
    sendByte(0x3d, DAT);
    sendByte(0x18, DAT);
    sendByte(0x25, DAT);
    sendByte(0x2a, DAT);
    sendByte(0x2b, DAT);
    sendByte(0x2b, DAT);
    sendByte(0x3a, DAT);

    sendByte(0xb1, REG);
    sendByte(0x08, DAT);
    sendByte(0x08, DAT);

    sendByte(0xb4, REG);
    sendByte(0x07, DAT);

    sendByte(0xc0, REG);
    sendByte(0x0a, DAT);
    sendByte(0x02, DAT);

    sendByte(0xc1, REG);
    sendByte(0x02, DAT);

    sendByte(0xc5, REG);
    sendByte(0x50, DAT);
    sendByte(0x5b, DAT);

    sendByte(0xc7, REG);
    sendByte(0x40, DAT);

    sendByte(0x2a, REG);
    sendByte(0x00, DAT);
    sendByte(0x00, DAT);
    sendByte(0x00, DAT);
    sendByte(0x7F, DAT);

    sendByte(0x2a, REG);
    sendByte(0x00, DAT);
    sendByte(0x00, DAT);
    sendByte(0x00, DAT);
    sendByte(0x9F, DAT);
    sendByte(250, DAT);

    sendByte(0x36, REG);
    sendByte(0xc8, DAT);

    sendByte(0x29, REG);
}

void setStartAddress() {
    sendByte(0x2a, REG);
    sendByte(0x00, DAT);
    sendByte(0x02, DAT);
    sendByte(0x00, DAT);
    sendByte(129, DAT);
    sendByte(0x2b, REG);
    sendByte(0x00, DAT);
    sendByte(3, DAT);
    sendByte(0x00, DAT);
    sendByte(130, DAT);
    sendByte(0x2c, REG);
}

void setup()
{
    Serial.begin(115200);
    delay(10);

    spi.setFrequency(20000000);
    spi.setDataMode(0);
    spi.setBitOrder(1);
    spi.begin(14, 12, 13, 15);
    spi.setHwCs(true);

    pinMode(AO_PIN, OUTPUT);

    initDisplay();
    setStartAddress();
    digitalWrite(AO_PIN, HIGH);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

int value = 0;

void loop()
{
    delay(5000);
    ++value;

    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 65000;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    //String url = "/input/";
    //url += streamId;
    //url += "?private_key=";
    //url += privateKey;
    //url += "&value=";
    //url += value;

    //Serial.print("Requesting URL: ");
    //Serial.println(url);

    // This will send the request to the server
    //client.print(String("GET ") + url + " HTTP/1.1\r\n" +
    //             "Host: " + host + "\r\n" +
    //             "Connection: close\r\n\r\n");
    //unsigned long timeout = millis();
    //while (client.available() == 0) {
    //    if (millis() - timeout > 5000) {
    //        Serial.println(">>> Client Timeout !");
    //        client.stop();
    //        return;
    //    }
    //}

    // Read all the lines of the reply from server and print them to Serial
    int av;
    int pack_count = 1;
    while(true) {
        while(av = client.available()){
            int read = client.read((buf + (pack_count - 1) * 1024), 1024);
            //spi.writeBytes(buf, read);
            client.write(resp, 2);
            //Serial.println(pack_count);
            pack_count++;
            if (pack_count == 33) {
                spi.writeBytes(buf, 32768);
                pack_count = 1;
                setStartAddress();
                digitalWrite(AO_PIN, HIGH);
            }
        }
    }
    //while(client.available()) {
    //    String line = client.readStringUntil('\r');
    //    Serial.print(line);
    //}
    spi.end();
    Serial.println();
    Serial.println("closing connection");
}

