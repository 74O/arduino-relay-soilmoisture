// YL-39 + YL-69 humidity sensor
byte sensorHumedad = A1;// Pin Analogico del chip yl-39 proporciona dato de humedad de 0 a 1023
byte sensorHumedadV = 6;// Pin que proporciona alimentacion al chip yl-39
// Valores minimo y maximo proporcionados por el sensor yl-69
int minv = 0;// Establecemos valor de lectura en seco. seria el 0 en un sensor perfecto
int maxv = 800;// Establecemos valor de lectura en humedo. seria el valor 1024
// Configuramos el rele
byte releV = 8;// Pin que alimenta el rele


void setup() {
    // Init the humidity sensor board
    pinMode(sensorHumedadV, OUTPUT);  // Establece pin de alimentacion del sensor de humedad como SALIDA
    digitalWrite(sensorHumedadV, LOW);// Apaga alimentacion del sensor de humedad

    // Init the relay
    pinMode(releV, OUTPUT);   // Establece el pin de alimentacion del rele como SALIDA
    digitalWrite(releV, LOW);// Apaga alimentacion del rele

    // Setup Serial
    while (!Serial);
    delay(500);
    Serial.begin(9600);
}


int riego(int p) {
    // Funcion que activa el rele en funcion del porcentaje de humedad
    //Configuramos valor minimo de humedad para activar riego
    int val = 20;
    //Configuramos tiempo de riego
    int segundos = 3;
    int milis = segundos * 1000;
    //Comprobamos si el porcentaje es superior u inferior
    if (p <= val) {
        digitalWrite(releV, HIGH);
        delay(milis);
        digitalWrite(releV, LOW);
        Serial.println("Hemos regado");
      } else {
        digitalWrite(releV, LOW);
        Serial.print("No es necesario regar");
      }
}


int humedad() {
    // Funcion que lee el valor analogico del sensor y lo convierte en porcentaje. Devuelve el valor del porcentaje.
    digitalWrite(sensorHumedadV, HIGH);// Enciende alimentacion del sensor de humedad
    delay(500);// Espera medio segundo
    int value = analogRead(sensorHumedad);// Lee el valor analogico del sensor
    delay(500);// Espera medio segundo
    digitalWrite(sensorHumedadV, LOW);// Apaga alimentacion del sensor de humedad

    int valorcalibrado = 1024 - value;// El sensor devuelve valor alto si esta seco y bajo si esta humedo, asi lo invertimos para facilitar lectura
    int porcentaje = map(valorcalibrado, minv, maxv, 0, 100);// Convierte valor en porcentaje

    return porcentaje;// Devuelve valor del porcentaje
}


void humedadSerial(){
    Serial.print("Humidity Level (0-100%): ");
    Serial.print(humedad());// Llama a la funcion humedad desde el monitor serial
    Serial.println("%");
    
    // Llamamos a la funcion de riego y le pasamos el porcentaje de humedad
    riego(humedad());
    Serial.println("Comando terminado");
}


void loop() {
    if(Serial.available()) {
      char c = Serial.read();
      if (c == 'r') {
        humedadSerial();
      }
    }
}