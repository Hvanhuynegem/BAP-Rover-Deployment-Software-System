//
// Created by henri on 16/06/2024.
//

// Function to calculate frequency from the period
float calculateFrequency(float period) {
    if (period == 0.0) {
        return 0.0;
    }
    const float clockFrequency = 4000000; // Assume SMCLK is 4 MHz
    return clockFrequency / period;       // Calculate the frequency
}

// Function to convert frequency to temperature
float frequency_to_temperature(float frequency) {
    if (frequency >= 150 && frequency <= 800) {
        float capacitance = 0.0000022; // Capacitance in Farads
        float x = 1.0 / (0.4055 * 2.0 * capacitance * frequency); // Intermediate calculation using the frequency
        float temperature = (x - 1000.0) / 3.85; // Convert intermediate value to temperature
        return temperature; // Return the calculated temperature
    } else {
        return -99.0;  // returns an error value that is impossible to record
    }
}