#ifndef STATION_MANAGER_H
#define STATION_MANAGER_H

#include <vector>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class Station
{
public:
  String id;
  int x;
  int y;
  int availableBikes;
  int availableStands;
  bool updated;

  Station(String stationId, int posX, int posY);
  void updateAvailability(int bikes, int stands);
  void printInfo() const;
};

class StationManager
{
private:
  std::vector<Station> &stations;
  const char *baseUrl = "https://data.grandlyon.com/fr/datapusher/ws/rdata/jcd_jcdecaux.jcdvelov/all.json?compact=false&maxfeatures=50&start=";

  bool allStationsUpdated() const;
  void processStations(JsonArray &values);

public:
  StationManager(std::vector<Station> &stationsToMonitor);
  void updateStations();
  void printAllStations() const;

  const std::vector<Station> &getAllStations() const;
};

#endif