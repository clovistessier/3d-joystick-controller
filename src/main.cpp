#include <Arduino.h>
#include <MCP3208.h>

#define ARDUINOOSC_ENABLE_ETHER
#include <ArduinoOSC.h>

#define ETHER_SS_PIN 17
#define LEFT_BUTTON_PIN 33
#define RIGHT_BUTTON_PIN 32

// Uncomment the relevant line if you want to use WiFi or Ethernet
// WiFi will create an Access Point your device needs to connect to.

// #define USE_WIFI
#define USE_ETHER

// WiFi Stuff
constexpr char ssid[] = "ESP32 Access Point";
constexpr char password[] = "password";

const IPAddress ip(192, 168, 1, 201);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);

// MAC for Ethernet
uint8_t mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// for ArduinoOSC
const char* host = "192.168.1.202";
const int send_port_osculator = 8000;

MCP3208 adc;
uint16_t lx, ly, lz, rx, ry, rz;
bool l, r;

int counter;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  adc.init();

  pinMode(LEFT_BUTTON_PIN, INPUT);
  pinMode(RIGHT_BUTTON_PIN, INPUT);


#ifdef USE_WIFI
  // WiFi stuff
  WiFi.disconnect(true, true);
  delay(2000);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  WiFi.softAPsetHostname(ssid);
  WiFi.softAPConfig(ip, gateway, subnet);

  Serial.println("WiFi connected, IP = ");
  Serial.println(WiFi.softAPIP());

#endif

#ifdef USE_ETHER
  // Ethernet Stuff
  Ethernet.init(ETHER_SS_PIN);
  Ethernet.begin(mac, ip);

  if (Ethernet.hardwareStatus() == EthernetNoHardware)
  {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
  }
  if (Ethernet.linkStatus() == LinkOFF)
  {
    Serial.println("Ethernet cable is not connected.");
  }

  Serial.print("Ethernet IP Address: ");
  Serial.println(Ethernet.localIP());

#endif
}
void loop()
{
  // put your main code here, to run repeatedly:

  // Take readings from all joystick axes and buttons
  rx = adc.readADC(CH_RX);
  ry = adc.readADC(CH_RY);
  rz = adc.readADC(CH_RZ);
  lx = adc.readADC(CH_LX);
  ly = adc.readADC(CH_LY);
  lz = adc.readADC(CH_LZ);
  l = static_cast<float>(digitalRead(LEFT_BUTTON_PIN));
  r = static_cast<float>(digitalRead(RIGHT_BUTTON_PIN));

  #ifdef USE_ETHER
  // Send them to OSCulator over Ethernet
  OscEther.send(host, send_port_osculator, "/left/x", static_cast<float>(lx/4095.0f));
  OscEther.send(host, send_port_osculator, "/left/y", static_cast<float>(ly/4095.0f));
  OscEther.send(host, send_port_osculator, "/left/z", static_cast<float>(lz/4095.0f));
  OscEther.send(host, send_port_osculator, "/left/button", l);
  OscEther.send(host, send_port_osculator, "/right/x", static_cast<float>(rx/4095.0f));
  OscEther.send(host, send_port_osculator, "/right/y", static_cast<float>(ry/4095.0f));
  OscEther.send(host, send_port_osculator, "/right/z", static_cast<float>(rz/4095.0f));
  OscEther.send(host, send_port_osculator, "/right/button", r);
  #endif

  #ifdef USE_WIFI
  // Send them to OSCulator over WiFi
  OscWiFi.send(host, send_port_osculator, "/left/x", static_cast<float>(lx/4095.0f));
  OscWiFi.send(host, send_port_osculator, "/left/y", static_cast<float>(ly/4095.0f));
  OscWiFi.send(host, send_port_osculator, "/left/z", static_cast<float>(lz/4095.0f));
  OscWiFi.send(host, send_port_osculator, "/left/button", l);
  OscWiFi.send(host, send_port_osculator, "/right/x", static_cast<float>(rx/4095.0f));
  OscWiFi.send(host, send_port_osculator, "/right/y", static_cast<float>(ry/4095.0f));
  OscWiFi.send(host, send_port_osculator, "/right/z", static_cast<float>(rz/4095.0f));
  OscWiFi.send(host, send_port_osculator, "/right/button", r);
  #endif

  if (++counter >= 30)
  {
    Serial.printf("%d\t%d\t%d\t\t%d\t%d\t%d\n", lx, ly, lz, rx, ry, rz);
    counter = 0;
  }
  delay(1000 / 60);
}