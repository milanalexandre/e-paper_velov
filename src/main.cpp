#include <vector>
#include <WiFi.h>
#include "StationManager.h"

const char *ssid = "ssid";
const char *password = "password";

// Définition des stations à surveiller
std::vector<Station> stationsToMonitor = {
    Station("2003", 430, 200),
    Station("2004", 420, 360),
    Station("2012", 600, 185),
    Station("2022", 380, 300),
    Station("2037", 310, 460),
    Station("2038", 500, 210),
    Station("2041", 570, 155),
    Station("2042", 308, 260),
    Station("5015", 210, 250),
    Station("5053", 109, 50),
    Station("7002", 710, 300),
    Station("7010", 730, 500),
    Station("7033", 670, 370),
    Station("7034", 790, 160),
    // Ajoutez d'autres stations ici
};
StationManager stationManager(stationsToMonitor);

//////////////////////////////////////////////////////////////////
//
//                          E-PAPER
//
//////////////////////////////////////////////////////////////////
#include "screen/utility/converteur.h"
#include "screen/utility/draw/GUI_Paint.h"
#include "screen/image.h"

UBYTE *BlackImage;

void setup_epaper()
{
  UWORD Imagesize = ((WIDTH % 8 == 0) ? (WIDTH / 8) : (WIDTH / 8 + 1)) * HEIGHT;
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL)
  {
    printf("Failed to apply for black memory...\r\n");
    while (1)
      ;
  }

  // Paint_NewImage(BlackImage, WIDTH, HEIGHT, 0, WHITE);
}

void init_epaper()
{
  GPIO_Config();
  EPD_Init();
  EPD_Clear();
  Paint_NewImage(BlackImage, WIDTH, HEIGHT, 0, WHITE);
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);
}

void print_maps()
{
  Paint_DrawImage(maps_image, 40, 24, 800, 480);
}

void print_station()
{
  const std::vector<Station> &stations = stationManager.getAllStations();
  for (const Station &station : stations)
  {
    Paint_DrawRectangle(station.x - 33, station.y - 38, station.x + 33, station.y - 5, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL, LINE_STYLE_SOLID);
    Paint_DrawRectangle(station.x - 30, station.y - 35, station.x + 30, station.y - 8, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL, LINE_STYLE_SOLID);
    Paint_DrawRectangle(station.x - 1, station.y - 35, station.x + 1, station.y, BLACK, DOT_PIXEL_2X2, DRAW_FILL_FULL, LINE_STYLE_SOLID);
    Paint_DrawNum(station.x - 30, station.y - 30, station.availableBikes, &Font20, WHITE, BLACK);
    Paint_DrawNum(station.x + 3, station.y - 30, station.availableStands, &Font20, WHITE, BLACK);
  }
}

void help()
{
  int g = 0;
  for (g = 0; WIDTH > g; g += 10)
  {
    Paint_DrawLine(g, 0, g, HEIGHT, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  }

  for (g = 0; HEIGHT > g; g += 10)
  {
    Paint_DrawLine(0, g, WIDTH, g, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  }

  for (g = 0; WIDTH > g; g += 50)
  {
    Paint_DrawLine(g, 0, g, HEIGHT, BLACK, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
  }
  for (g = 0; HEIGHT > g; g += 50)
  {
    Paint_DrawLine(0, g, WIDTH, g, BLACK, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
  }

  const std::vector<Station> &stations = stationManager.getAllStations();
  for (const Station &station : stations)
  {
    Paint_DrawCircle(station.x, station.y, 10, BLACK, DOT_PIXEL_3X3, DRAW_FILL_FULL);
    Paint_DrawCircle(station.x, station.y, 5, WHITE, DOT_PIXEL_3X3, DRAW_FILL_FULL);
  }

  Paint_Drawx_y();
}

//////////////////////////////////////////////////////////////////
//
//                          Wifi
//
//////////////////////////////////////////////////////////////////

void setupWiFi()
{
  WiFi.begin(ssid, password);
  Serial.println("Connexion au WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1);
  }
  Serial.println("Connecté au WiFi");
}

//////////////////////////////////////////////////////////////////
//
//                          Code
//
//////////////////////////////////////////////////////////////////


void setup()
{
  Serial.begin(115200);
  setupWiFi();

  stationManager.updateStations();
  stationManager.printAllStations();
  setup_epaper();
  init_epaper();
  print_maps();
  print_station();
  // help();
  EPD_Display_Base(BlackImage);
}

void loop()
{
}