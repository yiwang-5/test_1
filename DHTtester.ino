#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

const int GROUP_NO = 1;  // 改成你们的组号

DHT dht(DHTPIN, DHTTYPE);

unsigned long lastRead = 0;
unsigned long lastBlink = 0;
unsigned long blinkInterval = 600;  // 每次亮灭切换的间隔，毫秒

bool ledState = false;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  dht.begin();
}

void loop() {
  unsigned long now = millis();

  // 控制 LED 闪烁
  if (now - lastBlink >= blinkInterval) {
    lastBlink = now;
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
  }

  // 每 2 秒读取一次温湿度
  if (now - lastRead >= 2000UL) {
    lastRead = now;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // 每条数据前先输出组号
    Serial.print(F("Group: "));
    Serial.print("cd");

    if (isnan(h) || isnan(t)) {
      Serial.println(F("  Failed to read from DHT sensor!"));
      return;
    }

    // 示例规则：15~35 摄氏度对应 1000~100 毫秒
    // 温度越高，间隔越小，闪烁越快
    float limitedT = constrain(t, 15.0f, 35.0f);
    blinkInterval =
        (unsigned long)(1000.0f - (limitedT - 15.0f) * 45.0f);

    Serial.print(F("  Humidity: "));
    Serial.print(h, 1);
    Serial.print(F("%  Temperature: "));
    Serial.print(t, 1);
    Serial.print(F(" C  Toggle interval: "));
    Serial.print(blinkInterval);
    Serial.println(F(" ms"));
  }
}