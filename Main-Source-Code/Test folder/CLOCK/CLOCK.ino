#include "esp32-hal-cpu.h"

void setup() {
  Serial.begin(115200);
  
  // Get the CPU frequency
  uint32_t cpuFrequency = ESP.getCpuFreqMHz();
  
  // Print CPU frequency to Serial Monitor
  Serial.print("CPU Frequency: ");
  Serial.print(cpuFrequency);
  Serial.println(" MHz");
}

void loop() {
  // Your main code here (if needed)
}
