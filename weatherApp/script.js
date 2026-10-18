async function getWeather() {

    let city = document.getElementById("cityInput").value.trim();

    if (city === "") {
        alert("Please enter a city name");
        return;
    }

    try {

        console.log("Searching for:", city);

        // Find city coordinates
        let locationResponse = await fetch(
            `https://geocoding-api.open-meteo.com/v1/search?name=${encodeURIComponent(city)}&count=1&language=en&format=json`
        );

        if (!locationResponse.ok) {
            throw new Error("Geocoding API error");
        }

        let locationData = await locationResponse.json();

        console.log("Location data:", locationData);

        if (!locationData.results ||
            locationData.results.length === 0) {

            alert("City not found");
            return;
        }

        let location = locationData.results[0];

        let latitude = location.latitude;
        let longitude = location.longitude;


        // Get weather
        let weatherResponse = await fetch(
            `https://api.open-meteo.com/v1/forecast?latitude=${latitude}&longitude=${longitude}&current=temperature_2m,relative_humidity_2m,apparent_temperature,weather_code,wind_speed_10m`
        );

        if (!weatherResponse.ok) {
            throw new Error("Weather API error");
        }

        let weatherData = await weatherResponse.json();

        console.log("Weather data:", weatherData);

        let weather = weatherData.current;


        // Display data
        document.getElementById("city").textContent =
            location.name;

        document.getElementById("temperature").textContent =
            Math.round(weather.temperature_2m) + "°C";

        document.getElementById("humidity").textContent =
            weather.relative_humidity_2m + "%";

        document.getElementById("wind").textContent =
            Math.round(weather.wind_speed_10m) + " km/h";

        document.getElementById("feels").textContent =
            Math.round(weather.apparent_temperature) + "°C";


        let condition =
            getWeatherCondition(weather.weather_code);

        document.getElementById("condition").textContent =
            condition.text;

        document.getElementById("icon").textContent =
            condition.icon;

        //Send weather infromation to C
        analyzeWithC(
            weather.temperature_2m,
            weather.relative_humidity_2m,
            weather.wind_speed_10m,
            weather.weather_code
        );

    }

    catch (error) {

        console.error("ERROR:", error);

        alert("Error: " + error.message);
    }
}


function getWeatherCondition(code) {

    if (code === 0)
        return {
            text: "Clear Sky",
            icon: "☀️"
        };

    if (code === 1)
        return {
            text: "Mainly Clear",
            icon: "🌤️"
        };

    if (code === 2)
        return {
            text: "Partly Cloudy",
            icon: "⛅"
        };

    if (code === 3)
        return {
            text: "Cloudy",
            icon: "☁️"
        };

    if (code >= 45 && code <= 48)
        return {
            text: "Foggy",
            icon: "🌫️"
        };

    if (code >= 51 && code <= 57)
        return {
            text: "Drizzle",
            icon: "🌦️"
        };

    if (code >= 61 && code <= 67)
        return {
            text: "Rain",
            icon: "🌧️"
        };

    if (code >= 71 && code <= 77)
        return {
            text: "Snow",
            icon: "❄️"
        };

    if (code >= 80 && code <= 82)
        return {
            text: "Rain Showers",
            icon: "🌦️"
        };

    if (code >= 95 && code <= 99)
        return {
            text: "Thunderstorm",
            icon: "⛈️"
        };

    return {
        text: "Unknown",
        icon: "🌤️"
    };
}

async function analyzeWithC(temperature, humidity, wind, weatherCode) {
    try {
        const url =
            `http://127.0.0.1:8080/analyze` +
            `?temperature=${temperature}` +
            `&humidity=${humidity}` +
            `&wind=${wind}` +
            `&code=${weatherCode}`;

        console.log("Sending to C:", url);

        const response = await fetch(url);

        if (!response.ok) {
            throw new Error("C server error");
        }

        const data = await response.json();

        console.log("C Response:", data);

        document.getElementById("temperatureAdvice").textContent =
            data.temperature;

        document.getElementById("humidityAdvice").textContent =
            data.humidity;

        document.getElementById("windAdvice").textContent =
            data.wind;

        document.getElementById("rainAdvice").textContent =
            data.rain;

        document.getElementById("generalAdvice").textContent =
            data.advice;

    } catch (error) {
        console.error("C SERVER ERROR:", error);

        document.getElementById("generalAdvice").textContent =
            "❌ C server is not connected";
    }
}