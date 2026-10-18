#include <stdio.h>

struct Weather
{
  float temperature;
  float humidity;
  float wind;
  int weather_code;
};

void checkTemperature(float temperature)
{
  if (temperature >= 40)
  {
    printf("🔥 Extreme Heat\n");
  }
  else if (temperature >= 35)
  {
    printf("🌡️ Very Hot\n");
  }
  else if (temperature >= 25)
  {
    printf("☀️ Warm Weather\n");
  }
  else if (temperature >= 15)
  {
    printf("🌤️ Pleasant Weather\n");
  }
  else
  {
    printf("🥶 Cold Weather\n");
  }
}

void checkHumidity(float humidity)
{

  if (humidity >= 70)
  {
    printf("💧 High Humidity\n");
  }
  else if (humidity >= 40)
  {
    printf("💧 Comfortable Humidity\n");
  }
  else
  {
    printf("🏜️ Low Humidity\n");
  }
}

void checkWind(float wind)
{
  if (wind >= 40)
  {
    printf("🌪️ Strong Wind Warning\n");
  }
  else if (wind >= 20)
  {
    printf("💨 Windy\n");
  }
  else
  {
    printf("🍃 Normal Wind\n");
  }
}

void checkRain(int weather_code)
{
  if (weather_code >= 51 && weather_code <= 67)
  {
    printf("🌧️ Rain expected\n");
  }
  else if (weather_code >= 80 && weather_code <= 82)
  {
    printf("🌦️ Rain showers expected\n");
  }
  else if (weather_code >= 95 && weather_code <= 99)
  {

    printf("⛈️ Thunderstorm warning\n");
  }

  else
  {
    printf("☀️ No rain expected\n");
  }
}

void giveAdvice(struct Weather weather)
{
  printf(("\n========== WEATHER ANALYSIS ==========\n\n"));

  checkTemperature(weather.temperature);
  checkHumidity(weather.humidity);
  checkWind(weather.wind);
  checkRain(weather.weather_code);

  printf("\n========== ADVICE ==========\n");

  if (weather.temperature >= 35)
  {
    printf("Drink plenty of water.\n");
  }
  if (weather.temperature < 15)
  {
    printf("Wear warm cloths.\n");
  }
  if (weather.temperature >= 25 && weather.temperature < 35)
  {
    printf("Light cloths should be comfortable.\n");
  }
  if (weather.humidity >= 70)
  {
    printf("It may feet more humid then usual.\n");
  }
  if (weather.wind >= 40)
  {
    printf("Avoid unnecessary outdoor activity.\n");
  }
  if ((weather.weather_code >= 51 && weather.weather_code <= 67) ||
      (weather.weather_code >= 80 && weather.weather_code <= 82))
  {
    printf("☂️ Carry an umbrella.\n");
  }

  printf("\n=======================================\n");
}

int main()
{
  struct Weather weather;

  printf("Enter temperature (C): ");
  scanf("%f", &weather.temperature);

  printf("Enter humidity (%%): ");
  scanf("%f", &weather.humidity);

  printf("Enter wind speed (km/h): ");
  scanf("%f", &weather.wind);

  printf("Enter weather code: ");
  scanf("%d", &weather.weather_code);

  giveAdvice(weather);

  return 0;
}