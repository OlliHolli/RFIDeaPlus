// Main program for the RFIDea+ project
#include <SPI.h>
#include "./custom_libs/RFIDea.h"

RFIDea rfidea;

void setup() {
    Serial.begin(115200);
    SPI.begin();
    Serial.println("Running...");

    rfidea.initialise();
    delay(1000);        // safe delay to make sure initialisation is finished before usage
}

void loop() {
    rfidea.read();
    delay(1000);
}
