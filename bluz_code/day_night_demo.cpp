
//variable to keep track of when to publish
int timeLastPublish = 0;
int timeBetweenPublishes = 1000;

void setup() {
    //if bluz, set some connection parameters to keep us consistent across gateways
    #if PLATFORM_ID==103
    BLE.setConnectionParameters(20,40);
    #endif
}

void loop() {
    // if it is time to send the event, send it!
    if (millis() - timeLastPublish > timeBetweenPublishes) {
        int val = analogRead(A4);
        Particle.publish("Photo_Resistor", String(convertADC(val)));
        timeLastPublish = millis();
    }
    #if PLATFORM_ID==103
    //sleep the CPU if bluz
    System.sleep(SLEEP_MODE_CPU);
    #endif
}

int convertADC(int val) {
    //converts the raw ADC value to an opacity level the web app understands, will return 0-100
    int returnVal = 0;
    
    //bluz and the Photon have different values for their ADC (10-bit vs 12-bit), so we need to adjust them differently
    #ifdef PLATFORM_ID==6
    // The 370 can be adjusted based on light conditions in your surroundings. Decrease it if you have less light
    returnVal =val*10/370;
    #endif
    
    #ifdef PLATFORM_ID==103
    // The 65 can be adjusted based on light conditions in your surroundings. Decrease it if you have less light
    returnVal =val*10/65;
    #endif
    
    // in case we fall out of range, adjust to our boundaries 0-100
    if (returnVal < 0) {
        return 0;
    } else if (returnVal > 100) {
        return 100;
    } else {
        return returnVal;
    }
}
