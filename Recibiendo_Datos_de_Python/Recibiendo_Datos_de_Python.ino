#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

String receivedData = "";  // Variable para almacenar los datos recibidos

const int pinTrigger = 2;  //Variable que contiene el número del pin al cual conectamos la señal "trigger"
const int pinEcho = 3;   //Variable que contiene el número del pin al cual conectamos la señal "echo"

void setup() {
  Serial.begin(9600);
  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);

  pinMode(pinTrigger, OUTPUT); //Configuramoms el pin de "trigger" como salida
  pinMode(pinEcho, INPUT);  //Configuramoms el pin de "echo" como entrada
  digitalWrite(pinTrigger, LOW);//Ponemos en voltaje bajo(0V) el pin de "trigger"
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    
    if (c == '\n') {
      processMessage();
      receivedData = "";  // Reinicia la variable para el siguiente mensaje
    } else {
      receivedData += c;  // Agrega el carácter al mensaje recibido
    }
  }

  unsigned long t; //Variable de tipo unsigned long que contendrá el tiempo que le toma a la señal ir y regresar
  float d; //Variable de tipo float que contendrá la distancia en cm

  digitalWrite(pinTrigger, HIGH);//Ponemos en voltaje alto(5V) el pin de "trigger"
  delayMicroseconds(10);//Esperamos en esta línea para conseguir un pulso de 10us
  digitalWrite(pinTrigger, LOW);//Ponemos en voltaje bajo(0V) el pin de "trigger"
  
  t = pulseIn(pinEcho, HIGH);         //Utilizamos la función  pulseIn() para medir el tiempo del pulso/echo
  d = t * 0.000001 * 34300.0 / 2.0; //Obtenemos la distancia considerando que la señal recorre dos veces la distancia a medir y que la velocidad del sonido es 343m/s
  
  Serial.print("Distancia: ");
  Serial.print(d);
  Serial.print("cm");
  Serial.println();
  delay(10);
}

void processMessage() {
  receivedData.trim();  // Elimina espacios en blanco al inicio y al final
  
  if (receivedData.length() > 0) {
    int pos1 = receivedData.indexOf(',');
    int pos2 = receivedData.lastIndexOf(',');
    
    if (pos1 != -1 && pos2 != -1) {
      int val1 = receivedData.substring(0, pos1).toInt();
      int val2 = receivedData.substring(pos1 + 1, pos2).toInt();
      int val3 = receivedData.substring(pos2 + 1).toInt();
      
      servo1.write(val1);
      servo2.write(val2);
      servo3.write(val3);
      
      Serial.print(val1);
      Serial.print(';');
      Serial.print(val2);
      Serial.print(';');
      Serial.println(val3);
    }
  }
}
