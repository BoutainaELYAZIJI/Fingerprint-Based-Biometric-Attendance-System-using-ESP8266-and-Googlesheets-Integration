#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>


LiquidCrystal_I2C lcd(0x3F, 20, 4);

SoftwareSerial mySerial(D7, D8);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

const char* ssid     = "INSEA-WLAN";   //replace with your own SSID
const char* password = "";    //replace with your own password
const char* host = "api.pushingbox.com";

String member = "";
int flag = 0;

void setup(){

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("WELCOME");
  delay(2000);

  lcd.clear();

  Serial.begin(57600);
  delay(10);
  Serial.println();
  Serial.println();
  lcd.setCursor(0, 0);
  Serial.write("Connecting to WiFi...       ");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  lcd.setCursor(0, 0);
  lcd.print("WiFi connected          ");
  lcd.setCursor(0, 1);
  lcd.print("IP:");
  lcd.setCursor(4, 1);
  lcd.print(WiFi.localIP());

  delay(1500);


while (!Serial);
  delay(100);
  Serial.write("Waiting for Fingerprint Sensor");
  lcd.setCursor(0, 0);
  lcd.print("    Waiting for           ");
  lcd.setCursor(0, 1);
  lcd.print("      Sensor              ");
  delay(1500);
  finger.begin(57600);
  if (finger.verifyPassword())
  {
    Serial.println("Found Successfully");
    lcd.setCursor(0, 0);
    lcd.print("Sensor found!         ");
    lcd.setCursor(0, 1);
    lcd.print("                      ");
    delay(1500);
  } else
  {
    Serial.println("Fingerprint sensor not found!!!");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Not found!         ");
    lcd.setCursor(0, 1);
    lcd.print("                      ");
    while (1)
    {
      delay(1);
    }
  }
}
int getFingerprintID()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  return finger.fingerID;
}



void loop()
{

  int fingerprintID = getFingerprintID();
  delay(50);
  if (fingerprintID == 1)
  {
    Serial.println("Welcome Boutaina");
    lcd.setCursor(0, 0);
    lcd.print("Welcome Boutaina        ");
    lcd.setCursor(0, 1);
    lcd.print("                       ");
    connecthost("Boutaina");
    flag = 0;
  }else
  {
    Serial.println("Waiting for valid finger!!!");
    lcd.setCursor(0, 0);
    lcd.print(" Place a Valid       ");
    lcd.setCursor(0, 1);
    lcd.print("     Finger           ");
  }
}
void connecthost(String data)
{
  if (flag == 0)
  {
    member = data;
    flag = 1;
    Serial.print("connecting to ");
    Serial.println(host);
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) 
    {
      Serial.println("connection failed");
      return;
    }

    // We now create a URL for the request
    String url = "/pushingbox?";
    url += "devid=";
    url += "v9DEF04C912228FC";
    url += "&Name=" + String(member);

    Serial.print("Requesting URL: ");
    Serial.println(url);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
      Serial.print("Data Sent!");
    }

    Serial.println();
    Serial.println("closing connection");
  }
}




 