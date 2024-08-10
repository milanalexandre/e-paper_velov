#include "StationManager.h"

Station::Station(String stationId, int posX, int posY)
    : id(stationId), x(posX), y(posY), availableBikes(0), availableStands(0), updated(false) {}

void Station::updateAvailability(int bikes, int stands)
{
  availableBikes = bikes;
  availableStands = stands;
  updated = true;
}

void Station::printInfo() const
{
  Serial.print("Station ");
  Serial.print(id);
  Serial.print(" (X: ");
  Serial.print(x);
  Serial.print(", Y: ");
  Serial.print(y);
  Serial.print(") : ");
  Serial.print(availableBikes);
  Serial.print(" vélos disponibles, ");
  Serial.print(availableStands);
  Serial.println(" emplacements libres");
}

StationManager::StationManager(std::vector<Station> &stationsToMonitor) : stations(stationsToMonitor) {}

bool StationManager::allStationsUpdated() const
{
  for (const Station &station : stations)
  {
    if (!station.updated)
    {
      return false;
    }
  }
  return true;
}

void StationManager::processStations(JsonArray &values)
{
  for (JsonVariant value : values)
  {
    String stationId = value["number"].as<String>();

    for (Station &station : stations)
    {
      if (station.id == stationId && !station.updated)
      {
        JsonObject totalStands = value["total_stands"];
        JsonObject availabilities = totalStands["availabilities"];
        int availableBikes = availabilities["bikes"].as<int>();
        int availableStands = availabilities["stands"].as<int>();

        station.updateAvailability(availableBikes, availableStands);

        if (allStationsUpdated())
        {
          return;
        }
      }
    }
  }
}

void StationManager::updateStations()
{
  int start = 1;
  bool moreData = true;

  for (Station &station : stations)
  {
    station.updated = false;
  }

  while (moreData && !allStationsUpdated())
  {
    String url = String(baseUrl) + start;
    HTTPClient http;
    http.begin(url);

    int httpResponseCode = http.GET();

    Serial.print("url : ");
    Serial.println(url);

    Serial.print("httpResponseCode : ");
    Serial.println(httpResponseCode);

    if (httpResponseCode > 0)
    {
      String payload = http.getString();

      {
        DynamicJsonDocument doc(50 * 1024);
        DeserializationError error = deserializeJson(doc, payload);

        if (error)
        {
          Serial.print("deserializeJson() a échoué : ");
          Serial.println(error.c_str());
          http.end();
          break;
        }

        JsonArray values = doc["values"];
        processStations(values);

        if (values.size() < 50)
        {
          moreData = false;
        }
        else
        {
          start += 30;
        }
      }
      payload.clear();
    }
    else
    {
      Serial.print("Erreur dans la requête HTTP : ");
      Serial.println(httpResponseCode);
      http.end();
      break;
    }

    http.end();
  }
}

void StationManager::printAllStations() const
{
  for (const Station &station : stations)
  {
    station.printInfo();
  }
}

const std::vector<Station> &StationManager::getAllStations() const
{
  return stations;
}