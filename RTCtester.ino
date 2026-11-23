// DS1307 + TM1637：显示“组号:分钟”
#include "TM1637.h"
#include "RTClib.h"

#define CLK 10
#define DIO 9

const int GROUP_NO = 67;  // ★改成你们的组号，范围0～99

TM1637 tm1637(CLK, DIO);
RTC_DS1307 RTC;

int8_t TimeDisp[4];

void setup() {
  Serial.begin(9600);

  if (!RTC.begin()) {
    Serial.print(F("Group: "));
    Serial.print(GROUP_NO);
    Serial.println(F("  Couldn't find RTC"));

    while (1) {
      delay(10);
    }
  }

  if (!RTC.isrunning()) {
    Serial.println(F("RTC is NOT running!"));
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  tm1637.init();
  tm1637.set(2);
  tm1637.point(POINT_ON);  // 冒号常亮
}

void loop() {
  DateTime theTime = RTC.now();

  int hour = theTime.hour();  // 补上原程序遗漏的小时读取
  int minute = theTime.minute();
  int second = theTime.second();

  // ★左两位显示组号
  TimeDisp[0] = GROUP_NO / 10;
  TimeDisp[1] = GROUP_NO % 10;

  // ★右两位显示分钟
  TimeDisp[2] = minute / 10;
  TimeDisp[3] = minute % 10;

  tm1637.display(0, TimeDisp[0]);
  tm1637.display(1, TimeDisp[1]);
  tm1637.display(2, TimeDisp[2]);
  tm1637.display(3, TimeDisp[3]);

  // ★在串口时间数据前输出组号
  Serial.print(F("Group: "));
  Serial.print(GROUP_NO);
  Serial.print(F("  Time: "));

  if (hour < 10) Serial.print('0');
  Serial.print(hour);
  Serial.print(':');

  if (minute < 10) Serial.print('0');
  Serial.print(minute);
  Serial.print(':');

  if (second < 10) Serial.print('0');
  Serial.println(second);

  delay(1000);
}