/*
 * DHT22Node.cpp
 * Homie Node for DHT22 sensors using Adafruit DHT22 library.
 *
 * Version: 1.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include "DHT22Node.hpp"
#include "Debug.hpp"

#define DHTTYPE DHT22

DHT22Node::DHT22Node(const char *name, const int sensorPin, const int measurementInterval)
    : HomieNode(name, "DHT22Sensor"),
      _sensorPin(sensorPin),
      _measurementInterval(measurementInterval),
      _lastMeasurement(0)
{
  if (_sensorPin > DEFAULTPIN)
  {
    dht = new DHT(_sensorPin, DHTTYPE);
  }
}

void DHT22Node::printCaption()
{
  Homie.getLogger() << cCaption << " pin[" << _sensorPin << "]:" << endl;
}

void DHT22Node::loop()
{
  if (_sensorPin > DEFAULTPIN)
  {
    if ((millis() - _lastMeasurement >= _measurementInterval * 1000UL) ||
        (_lastMeasurement == 0))
    {
      temperature = dht->readTemperature();
      humidity = dht->readHumidity();

      printCaption();

      if (isnan(temperature) || isnan(humidity))
      {
        Homie.getLogger() << cIndent << "Error reading from Sensor" << endl;
        Debug::debugSend(setProperty(cStatus).send("error"));
      }
      else
      {
        Homie.getLogger() << cIndent << "Temperature: " << temperature << " °C" << endl;
        Homie.getLogger() << cIndent << "Humidity: " << humidity << " %" << endl;

        Debug::debugSend(setProperty(cStatus).send("ok"));
        Debug::debugSend(setProperty(cTemperature).send(String(temperature)));
        Debug::debugSend(setProperty(cHumidity).send(String(humidity)));
      }
      _lastMeasurement = millis();
    }
  }
}

void DHT22Node::onReadyToOperate()
{
  Debug::debugSend(setProperty(cTemperatureUnit).send("°C"));
  Debug::debugSend(setProperty(cHumidityUnit).send("%"));
};

void DHT22Node::setup()
{
  advertise(cStatus);
  advertise(cTemperature);
  advertise(cTemperatureUnit);
  advertise(cHumidity);
  advertise(cHumidityUnit);

  printCaption();
  Homie.getLogger() << cIndent << "Reading interval: " << _measurementInterval << " s" << endl;

  if (dht)
  {
    dht->begin();
  }
}
