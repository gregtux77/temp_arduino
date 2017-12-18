


/*

   {"function_name":"temperature"}%

  all off
   {"function_name":"SendRadioCode","prise":"0","etat":"0"}%
  all on
   {"function_name":"SendRadioCode","prise":"0","etat":"1"}%


  prise 1 :10 ,11
  prise 2 :20 ,21
  prise 3 :30 ,31
  prise 4 :40 ,41
  prise all :90 ,91

  {"function_name":"SendRadioCode","code":"10"}%
 {"function_name":"temperature"}%
  {"function_name":"lumiere""}%


*/

#include <ArduinoJson.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

// Serial buffer
String command = "";

// End of command marker
char endMarker = '%';



void executeFunction(String json_data) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& v = jsonBuffer.parseObject(json_data);
  if ( v["function_name"] == String("SendRadioCode") )
  {
    allume_prise(v["code"]);
  }

  JsonObject& b = jsonBuffer.parseObject(json_data);
  if ( b["function_name"] == String("temperature") )
  {
    temperature();
  }
  JsonObject& c = jsonBuffer.parseObject(json_data);
  if ( c["function_name"] == String("lumiere") )
  {
    lumiere();
  }
}


/**
   This function is automatically called when data is received on serial port
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
  mySwitch.setRepeatTransmit(15);
  mySwitch.enableTransmit(10);
  mySwitch.setProtocol(1);
  mySwitch.setPulseLength(489);
  Serial.println("init ");

  delay(1000);
 
}


void loop() {


  if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();

    if (value == 0) {
      Serial.print("Unknown encoding");
    }
    else {
      Serial.print(mySwitch.getReceivedValue());
      Serial.println("}");
    }
    delay(200);
    mySwitch.resetAvailable();
  }

}



void allume_prise(long code) {
  Serial.print(code);
 


  
  if (code == 1)
  {
    mySwitch.send(1381716, 24);
    Serial.print("prise 1 est etiente");
    Serial.print("{\"devicetype\":\"18\",\"value\":PRISE 1 OFF");
    Serial.print("}\n");
  }
 else if (code == 2)
  {
    mySwitch.send(1381717, 24);
    Serial.print("prise 1 est allumÃ©e");
    Serial.print("{\"devicetype\":\"18\",\"value\":PRISE 1 ON");
    Serial.print("}\n");
  }

 else if (code == 3) {
    mySwitch.send(1394004, 24);
    Serial.print("prise 2 est etiente");
    Serial.print("{\"devicetype\":\"18\",\"value\":PRISE 2 OFF");
    Serial.print("}\n");
  }
 else if (code == 4)
  {
    mySwitch.send(1394005, 24);
    Serial.print("prise 2 est allumÃ©e");
    Serial.print("{\"devicetype\":\"18\",\"value\":PRISE 2 ON");
    Serial.print("}\n");
  }


 else if (code == 5)
  {
    sendCommand(0, 0, 0);
    Serial.print("prise 3 est etiente");
    Serial.print("{\"devicetype\":\"18\",\"value\":PRISE 3 OFF");
    Serial.print("}\n");
  }
 else if (code == 6) {
    sendCommand(0, 0, 1);
    Serial.print("prise 3 est allumÃ©e");
    Serial.print("{\"devicetype\":\"18\",\"value\":PRISE 3 ON");
    Serial.print("}\n");
  }


 else if (code == 7)
  {
    sendCommand(1, 0, 0);
    Serial.print("prise 4 est etiente");
    Serial.print("{\"devicetype\":\"18\",\"value\":PRISE 4 OFF");
    Serial.print("}\n");
  }

else  if (code == 8)
  {
    sendCommand(1, 0, 1);
    Serial.print("prise 4 est allumÃ©e");
    Serial.print("{\"devicetype\":\"18\",\"value\":PRISE 4 ON");
    Serial.print("}\n");
  }


 else if (code == 10)
  {
    sendCommand(0, 0, 0);
    delay(50);
    sendCommand(1, 0, 0);
    delay(50);
    mySwitch.send(1394004, 24);
    delay(50);
    mySwitch.send(1381716, 24);
    Serial.print("toutes les prises sont etientes");
    Serial.print("{\"devicetype\":\"18\",\"value\":PRISE ALL OFF");
    Serial.print("}\n");
  }

else  if (code == 11)
  {
    mySwitch.send(1394005, 24);
    delay(50);
    mySwitch.send(1381717, 24);
    delay(50);
    sendCommand(0, 0, 1);
    delay(50);
    sendCommand(1, 0, 1);
    Serial.print("toutes les prises sont allumees");
    Serial.print("{\"devicetype\":\"18\",\"value\":PRISE ALL ON");
    Serial.print("}\n");
  }
else
{
  Serial.print("code inconnu");
}
}


void sendCommand(int selector, int number, int state) {
  // all list are zero based
  // Selector list (A, B, C)
  char *selectorCodes[3] = { "0FF", "F0F", "FF0"};

  // Button number list
  char *buttonCodes[4] = {"0FFFFFFF", "F0FFFFFF", "FF0FFFFF", "FFF0FFFF"};

  // State list
  char *stateCodes[2] = { "0", "1" };

  // Concatenate into Tri-State command
  char sendCommand[12] = "";
  strcat(sendCommand, selectorCodes[selector]);
  strcat(sendCommand, buttonCodes[number]);
  strcat(sendCommand, stateCodes[state]);

  // Send the code
  Serial.println(sendCommand);
  mySwitch.sendTriState(sendCommand);
}



void temperature() {

  // Mesure la tension sur la broche A0 (sortie capteur) et A1 (rÃ©fÃ©rence du point zÃ©ro)
  int valeur_brute = analogRead(A0);
  int valeur_offset = analogRead(A1);

  // Transforme la mesure (nombre entier) en tempÃ©rature via un produit en croix
  float temperature_celcius = (valeur_brute - valeur_offset) * (5.0 / 1023.0 * 100.0);

  // Envoi la mesure au PC pour affichage et attends 250ms
  // Serial.println(temperature_celcius);
  Serial.print("{\"devicetype\":\"18\",\"value\":");
  Serial.print(temperature_celcius);
  Serial.print("}\n");

}

void lumiere()
{
  int valeur = analogRead(A2);
  if(valeur >= 0 && valeur <= 350)
  {
    Serial.print("il fait nuit noir");
  }
  else  if(valeur >= 351 && valeur <= 670)
  {
   Serial.print("il fait jour la lumiere est faible");
  }
  else  if(valeur >= 671 && valeur <= 1000)
  {
   Serial.print("il fait jour");
  }
   else 
   
   Serial.print("il fait plein soleil");
  
  Serial.println(valeur);

}





