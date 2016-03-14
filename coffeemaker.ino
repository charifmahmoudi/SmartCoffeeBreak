// This #include statement was automatically added by the Particle IDE.
//#include "SparkJson/SparkJson.h"

// This #include statement was automatically added by the Particle IDE.
#include "Structure-MQTT-device/Structure-MQTT-device.h"



// The Photon's onboard LED.
int LED = D7;

int MAKE_COFFEE = 90;
int STOP_COFFEE = 0;
Servo coffee;

// The initialization of your device
GetStructure::Device photon("YOUR-DEVICE-ID", "YOUR-ACCESS-KEY", "YOUR-ACCESS-SECRET");

void publishStatus(bool status){
    
        // Build the json payload: { "make" : ON/OFF, "time" : yyy}
        StaticJsonBuffer<100> jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        
        root["make"] = status;
        root["time"] =  millis();
        
        
       //Publish directly your JsonObject !!!  
       photon.publish(root);
}

// Callback signature for GetStructure.io  subscriptions.
void myCallback(JsonObject& command);

// Callback for GetStructure.io  subscriptions.
// Receive Your JsonObject from GetStructure.io
void myCallback(JsonObject& command) {

    Serial.print("Received : ");
    Serial.println(String(command["name"].asString()));
    
    if(String(command["name"].asString()).equals(String("make"))) {
      Serial.println("Making Coffee");
      digitalWrite(LED, HIGH);
      coffee.write(MAKE_COFFEE);
      publishStatus(true);
      delay(1000);
      digitalWrite(LED, LOW);
      coffee.write(STOP_COFFEE);
      publishStatus(false);
      Serial.println("Coffee is ready");
  }
  else{
      digitalWrite(LED, LOW);
      coffee.write(STOP_COFFEE);
      publishStatus(false);
      Serial.println("Emergency Stop");
  }
}

void setup() {
    Serial.begin(9600);
    Serial.println("Starting the Setup");
    pinMode(LED, OUTPUT);
    coffee.attach(D2);
	
	//Connect the photon to GetStructure.io broker
	photon.connect(myCallback);
	Serial.println("Setup complete");
}

void loop() {
	
    photon.loop();
    
}


