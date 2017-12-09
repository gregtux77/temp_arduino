
/*
 * 
 * {"function_name":"SendRadioCode","prise":"1","etat":"1"}%
 * {"function_name":"temperature"}%
 * 
 */
 
#include <ArduinoJson.h>
#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

// Serial buffer
String command = "";

// End of command marker
char endMarker = '%';
char prise ="";
char etat="";

void SendRadioCode(long code) {
   mySwitch.send(code, 24);
}


void executeFunction(String json_data) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& v = jsonBuffer.parseObject(json_data);
       if ( v["function_name"] == String("SendRadioCode") )
       {
           prise =v["prise"];
          etat= v["etat"] ;  
          Serial.print(prise,etat) ;
          allume_prise(prise,etat) ;
       }
  JsonObject& b = jsonBuffer.parseObject(json_data);
       if ( b["function_name"] == String("temperature") )
       {
       temperature();
       }
}
       

/**
 * This function is automatically called when data is received on serial port
 */
void serialEvent() {
    //lit toutes les données (vide le buffer de réception)
    char last_readed_char = Serial.read();
    if ( last_readed_char == endMarker ) {
      executeFunction(command);
      command = "";
    } else {
      command += last_readed_char;
    }
}


void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);
  mySwitch.enableTransmit(10);
  mySwitch.setPulseLength(310);
  mySwitch.setProtocol(1);
  mySwitch.setRepeatTransmit(15);

  delay(1000);
}

void loop() {
  
  if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();
    
    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      Serial.print("{\"action\":\"received\",\"value\":");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.println("}");
    }
    delay(200); 
    mySwitch.resetAvailable();
  }
} 

 
void allume_prise(int prise,int etat){
    Serial.print(prise,etat);
    if (prise==1) {
      
                if (etat==1) {
                mySwitch.send(1381716,24);
                Serial.print("prise 1 est allumée");
              }
              if (etat==0) {
                  mySwitch.send(1381717,24);
                   Serial.print("prise 0 est etiente");
                }
  }
  if (prise==2) {
      
                if (etat==1) {
                mySwitch.send(1394005,24);
                Serial.print("prise 1 est allumée");
              }
              if (etat==0) {
                  mySwitch.send(1394004,24);
                   Serial.print("prise 0 est etiente");
                }
  }
}


void temperature() {
  
  // Mesure la tension sur la broche A0 (sortie capteur) et A1 (référence du point zéro)
  int valeur_brute = analogRead(A0);
  int valeur_offset = analogRead(A1);
  
  // Transforme la mesure (nombre entier) en température via un produit en croix
  float temperature_celcius = (valeur_brute - valeur_offset) * (5.0 / 1023.0 * 100.0);
  
  // Envoi la mesure au PC pour affichage et attends 250ms
 // Serial.println(temperature_celcius);
 Serial.print("{\"devicetype\":\"18\",\"value\":");
  Serial.print(temperature_celcius);
  Serial.print("}\n");

  delay(5000);
  
} 
