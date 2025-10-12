#version 330 core

out vec4 FragColor;
uniform float temperature; // Kelvin

// Tanner Helland blackbody to RGB algorithm
vec3 temperatureToRGB(float kelvin)
{
    // Clamp temperature
    kelvin = clamp(kelvin, 1000.0, 40000.0);

    // All calculations use temperature / 100
    float temp = kelvin / 100.0;

    float r;
    // --- Red ---
    if (temp <= 66.0) {
        r = 255.0;
    } else {
        float t = temp - 60.0;
        r = 329.698727446 * pow(t, -0.1332047592);
        r = clamp(r, 0.0, 255.0);
    }

    float g;
    // --- Green ---
    if (temp <= 66.0) {
        g = 99.4708025861 * log(temp) - 161.1195681661;
    } else {
        float t = temp - 60.0;
        g = 288.1221695283 * pow(t, -0.0755148492);
    }
    g = clamp(g, 0.0, 255.0);

    float b;
    // --- Blue ---
    if (temp >= 66.0) {
        b = 255.0;
    } else if (temp <= 19.0) {
        b = 0.0;
    } else {
        float t = temp - 10.0;
        b = 138.5177312231 * log(t) - 305.0447927307;
        b = clamp(b, 0.0, 255.0);
    }

    // Convert to 0..1
    vec3 c = vec3(r, g, b) / 255.0;

    return c;
}

void main()
{
    vec3 col = temperatureToRGB(temperature);
    FragColor = vec4(col, 1.0);
}


