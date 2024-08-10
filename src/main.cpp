#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>



//////////////////////////////////////////////////////////////////
//
//                                   E-PAPER config
//
//////////////////////////////////////////////////////////////////
#include "screen/utility/converteur.h"
#include "screen/utility/draw/GUI_Paint.h"
#include "screen/image.h"

UBYTE *BlackImage;
#if EPD_7in5B_HD
  UBYTE *RYImage;
#endif

//////////////////////////////////////////////////////////////////
//
//                                  
//
//////////////////////////////////////////////////////////////////



// const char* ssid = "WIFI_SSID";
// const char* password = "WIFI_PASS";

// const char* baseUrl = "https://data.grandlyon.com/fr/datapusher/ws/rdata/jcd_jcdecaux.jcdvelov/all.json?compact=false&maxfeatures=50&start=";

// // Liste prédéfinie des IDs de stations à surveiller
// const String stationsToMonitor[] = {"", "", ""}; // Ajoutez ou modifiez les IDs selon vos besoins

// const int numStationsToMonitor = sizeof(stationsToMonitor) / sizeof(stationsToMonitor[0]);

void setup() {
  // Serial.begin(115200);
  // WiFi.begin(ssid, password);
  
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connexion au WiFi...");
  // }
  // Serial.println("Connecté au WiFi");
  GPIO_Config();

  EPD_Init();
  EPD_Clear();

  UWORD Imagesize = ((WIDTH % 8 == 0)? (WIDTH / 8 ): (WIDTH / 8 + 1)) * HEIGHT;
  if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
      printf("Failed to apply for black memory...\r\n");
      while(1);
  }
  #if EPD_7in5B_HD
    if ((RYImage = (UBYTE *)malloc(Imagesize)) == NULL) {
      printf("Failed to apply for red memory...\r\n");
      while(1);
    }
  #endif
  Paint_NewImage(BlackImage, WIDTH, HEIGHT, 0, WHITE);
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);
  Paint_Drawx_y();
  Paint_DrawImage(maps_image, 40, 24, 800, 480);
  
  EPD_Display_Base(BlackImage);
}

void processStations(JsonArray& values) {
  // for (JsonVariant value : values) {
  //   String stationId = value["number"].as<String>();
    
  //   for (int i = 0; i < numStationsToMonitor; i++) {
  //     if (stationId == stationsToMonitor[i]) {
  //       JsonObject totalStands = value["total_stands"];
  //       JsonObject availabilities = totalStands["availabilities"];
  //       int availableBikes = availabilities["bikes"].as<int>();
  //       int availableStands = availabilities["stands"].as<int>();
  //       int capacity = totalStands["capacity"].as<int>();

  //       Serial.print("Station ");
  //       Serial.print(stationId);
  //       Serial.print(" : ");
  //       Serial.print(availableBikes);
  //       Serial.print(" vélos disponibles, ");
  //       Serial.print(availableStands);
  //       Serial.print(" emplacements libres, capacité totale de ");
  //       Serial.println(capacity);
  //     }
  //   }
  // }
}

void loop() {
  // if (WiFi.status() == WL_CONNECTED) {
  //   int start = 1;
  //   bool moreData = true;

  //   while (moreData) {
  //     String url = String(baseUrl) + start;
  //     HTTPClient http;
  //     http.begin(url);
      
  //     int httpResponseCode = http.GET();
      
  //     if (httpResponseCode > 0) {
  //       String payload = http.getString();
        
  //       DynamicJsonDocument doc(32 * 1024); // Ajustez la taille si nécessaire
  //       DeserializationError error = deserializeJson(doc, payload);
        
  //       if (error) {
  //         Serial.print("deserializeJson() a échoué : ");
  //         Serial.println(error.c_str());
  //         break;
  //       }
        
  //       JsonArray values = doc["values"];
  //       processStations(values);

  //       // Vérifier s'il y a plus de données
  //       if (values.size() < 50) {
  //         moreData = false;
  //       } else {
  //         start += 50;
  //       }
  //     } else {
  //       Serial.print("Erreur dans la requête HTTP : ");
  //       Serial.println(httpResponseCode);
  //       break;
  //     }
      
  //     http.end();
  //   }
  // }
  
  // delay(300000); // Attendre 5 minutes avant la prochaine vérification
}