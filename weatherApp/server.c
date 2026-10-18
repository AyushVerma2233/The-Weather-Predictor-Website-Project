#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main() {

  int server;
  int client;

  struct sockaddr_in address;

  int address_size = sizeof(address);

  // Creating server socket

  server = socket(AF_INET, SOCK_STREAM, 0);

  if (server < 0) {

    printf("Socket error\n");

    return 1;
  }

  // Server settings

  address.sin_family = AF_INET;

  address.sin_addr.s_addr = INADDR_ANY;

  address.sin_port = htons(PORT);

  // Connect socket to port

  if (bind(server, (struct sockaddr *)&address, sizeof(address)) < 0) {

    printf("Bind error\n");

    return 1;
  }

  // Start listening

  listen(server, 10);

  printf("C Weather Server running...\n");

  printf("http://localhost:8080\n");

  while (1) {

    // Wait for JavaScript

    client =
        accept(server, (struct sockaddr *)&address, (socklen_t *)&address_size);

    if (client < 0) {

      continue;
    }

    // Read request

    char request[4096];

    memset(request, 0, sizeof(request));

    read(client, request, sizeof(request) - 1);

    printf("\nRequest received:\n%s\n", request);

    // Analyze /analyze

    if (strstr(request, "GET /analyze") != NULL) {

      // Variables

      float temperature;
      float humidity;
      float wind;

      int weatherCode;

      /*
         Read values from URL

         Example:

         ?temperature=38
         &humidity=70
         &wind=10
         &code=0
      */

      sscanf(strstr(request, "?temperature="),

             "?temperature=%f&humidity=%f&wind=%f&code=%d",

             &temperature, &humidity, &wind, &weatherCode);

      printf("Temperature: %.1f\n", temperature);

      printf("Humidity: %.1f\n", humidity);

      printf("Wind: %.1f\n", wind);

      printf("Weather Code: %d\n", weatherCode);

      // Weather analysis

      char temperatureText[100];

      char humidityText[100];

      char windText[100];

      char rainText[100];

      char adviceText[200];

      // Temperature

      if (temperature >= 40) {

        strcpy(temperatureText, "🔥 Extreme Heat");

      }

      else if (temperature >= 35) {

        strcpy(temperatureText, "🌡️ Very Hot");

      }

      else if (temperature >= 25) {

        strcpy(temperatureText, "☀️ Warm Weather");

      }

      else if (temperature >= 15) {

        strcpy(temperatureText, "🌤️ Pleasant Weather");

      }

      else {

        strcpy(temperatureText, "🥶 Cold Weather");
      }

      /*
         Humidity
      */

      if (humidity >= 70) {

        strcpy(humidityText, "💧 High Humidity");

      }

      else if (humidity >= 40) {

        strcpy(humidityText, "💧 Comfortable Humidity");

      }

      else {

        strcpy(humidityText, "🏜️ Low Humidity");
      }

      /*
         Wind
      */

      if (wind >= 40) {

        strcpy(windText, "🌪️ Strong Wind Warning");

      }

      else if (wind >= 20) {

        strcpy(windText, "💨 Windy");

      }

      else {

        strcpy(windText, "🍃 Normal Wind");
      }

      /*
         Rain
      */

      if (weatherCode >= 51 && weatherCode <= 67) {

        strcpy(rainText, "🌧️ Rain expected");

      }

      else if (weatherCode >= 80 && weatherCode <= 82) {

        strcpy(rainText, "🌦️ Rain showers expected");

      }

      else if (weatherCode >= 95 && weatherCode <= 99) {

        strcpy(rainText, "⛈️ Thunderstorm warning");

      }

      else {

        strcpy(rainText, "☀️ No rain expected");
      }

      /*
         General advice
      */

      if (temperature >= 35) {

        strcpy(adviceText, "💧 Drink plenty of water.");

      }

      else if (temperature < 15) {

        strcpy(adviceText, "🧥 Wear warm clothes.");

      }

      else if (weatherCode >= 51 && weatherCode <= 82) {

        strcpy(adviceText, "☂️ Carry an umbrella.");

      }

      else {

        strcpy(adviceText, "👍 Weather looks comfortable.");
      }

      /*
         JSON response
      */

      char response[2000];

      snprintf(

          response, sizeof(response),

          "HTTP/1.1 200 OK\r\n"
          "Content-Type: application/json\r\n"
          "Access-Control-Allow-Origin: *\r\n"
          "Connection: close\r\n"
          "\r\n"

          "{"
          "\"temperature\":\"%s\","
          "\"humidity\":\"%s\","
          "\"wind\":\"%s\","
          "\"rain\":\"%s\","
          "\"advice\":\"%s\""
          "}",

          temperatureText, humidityText, windText, rainText, adviceText);

      /*
         Send response to JavaScript
      */

      send(client, response, strlen(response), 0);
    }

    close(client);
  }

  close(server);

  return 0;
}