#include <Servo.h>
#include <Wire.h>
#include <RtcDS1307.h>
#include <GyverButton.h>
RtcDS1307<TwoWire> Rtc(Wire);

#define BTN_Left 3
#define BTN_Right 4
#define BTN_Enter 5

GButton Left(BTN_Left);
Gbutton Right(BTN_Right);
Gbutton Enter(BTN_Enter);

class IndicatorServo
{
  public:

  IndicatorServo();

  void attachPin(int pin);
  void setDegServo(int degServo);
  void setValueIndicator(int valueIndicator);
  void setTimeRenew(int timeRenew) 
  {
    m_timeRenew = timeRenew;
  };

  private:

  int m_actualValue = 0;

  float degHours [10] = {180, 160, 140, 120, 100, 80, 60, 40, 20, 0};

  Servo m_servo;

  unsigned long TimerTime;

  static int m_timeRenew;

  static bool runingClock;
};

int IndicatorServo::m_timeRenew = 50;
bool IndicatorServo::runingClock = false;

IndicatorServo::IndicatorServo()
{
  m_actualValue = 0;
}

void IndicatorServo::attachPin(int pin)
{
  m_actualValue = 0;
  m_servo.attach(pin);
  m_servo.write(0);
}

void IndicatorServo::setDegServo(int degServo)
{
  m_servo.write(degServo);
}
void IndicatorServo::setValueIndicator(int valueIndicator)
{
  if (valueIndicator < m_actualValue)
    {
      int i = degHours[m_actualValue];
      while (degHours[valueIndicator] > i)
      {
        runingClock = true;
        m_servo.write(i);
        if (millis() - TimerTime > m_timeRenew) {
          TimerTime = millis();
          i = i + 1;
        }
      }
      m_actualValue = valueIndicator;
      runingClock = false;
    } else {
      int i = degHours[m_actualValue];
      while (degHours[valueIndicator] < i)
      {
        runingClock = true;
        m_servo.write(i);
        if (millis() - TimerTime > m_timeRenew) {
          TimerTime = millis();
          i = i - 1;
        }
      }
      m_actualValue = valueIndicator;
      runingClock = false;
    }
}

  IndicatorServo segmentMinute;
  IndicatorServo segmentMinute10;


void setup()
{
  Serial.begin(9600);
  Rtc.Begin();
  segmentMinute.attachPin(11);
  segmentMinute10.attachPin(10);
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  Rtc.SetDateTime(compiled);
  Rtc.SetIsRunning(true);
}

void timeOut()
{
  
}

void setTimeClock()
{

}

void tempHgOut()
{

}

void sleepClock()
{

}

void loop()
{

Right.tick();
Left.tick();
Entet.tick();

  RtcDateTime now = Rtc.GetDateTime();
  //Serial.println(now.Second() % 10);
  
  segmentMinute.setValueIndicator(now.Minute() / 10);
  segmentMinute10.setValueIndicator(now.Minute() % 10);
  
//  smothChange(now.Minute() / 10, segmentSecond, currentSecond);
//  smothChange(now.Minute() % 10, segmentSecond10, currentSecond10);
}
