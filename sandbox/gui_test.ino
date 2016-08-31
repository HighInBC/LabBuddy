#include <Nextion.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define nextion Serial
#define ONE_WIRE_BUS 5

NexWaveform s0 = NexWaveform(1, 2, "s0");

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress thermometer = { 0x28, 0xFF, 0x96, 0x0A, 0x63, 0x16, 0x04, 0x1E };

int graph_channel = 0;
int graph_x_offset;

void setup() {
  sensors.setResolution(thermometer, 12);
  nexInit();
  delay(2000);
  Alarm.timerRepeat(10, reportTemp);
  reportTemp();
}

void loop() {
 Alarm.delay(0);
}

void reportTemp() {
  sensors.requestTemperatures();
  float temp = sensors.getTempC(thermometer);
  float graphFValue = constrain( mapfloat( temp, 0, 14, 0, 140 ), 0, 140 );
  s0.addValue( graph_channel, graphFValue );
  tone(A3, 2600, 10);
  graph_x_offset++;
  if( graph_x_offset >= 360 ) {
    graph_channel++;
    graph_x_offset = 1;
    if( graph_channel > 2 ) {
      graph_channel = 0;
    }
  }
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
