/* 
   Fundacion Kinal
   Centro educativo tecnico laboral Kinal
   Quinto perito
   Quinto electronica
   Codigo Tecnico: EB5AV 
   Curso: Taller de electronica digital y reparacion de computadoras I
   Proyecto: RFID
   Dev: Profe. Alejandro Lopez Navas
   Fecha: 26 de julio.
*/

#include <MFRC522.h>
#include <SPI.h>

#define RST_pin 9
#define SS_pin 10
#define verde 3
#define rojo 4
#define buzz 2

MFRC522 mfrc522(SS_pin, RST_pin);
MFRC522::MIFARE_Key key;

byte tag1[4] = {0x73, 0x85, 0x19, 0xF8};  // Cambia este valor al RFID que deseas permitir

byte tagActual[4];

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(verde, OUTPUT);
  pinMode(rojo, OUTPUT);
  digitalWrite(rojo, HIGH);
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);

  digitalWrite(verde, LOW);
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? "0" : " ");
    Serial.print(buffer[i], HEX);
    tagActual[i] = buffer[i];
  }

  if (compararArray(tagActual, tag1)) {
    Serial.println("Acceso permitido...");
    digitalWrite(verde, HIGH);
    digitalWrite(rojo, LOW);
    tone(buzz, 200);
    delay(3000);
    noTone(buzz);
    digitalWrite(rojo, HIGH);
  } 
  
  else {
    Serial.println("Acceso denegado");
    digitalWrite(rojo, HIGH);
    tone(buzz, 400);
    delay(1000);
    noTone(buzz);
  }
}

boolean compararArray(byte Array1[], byte Array2[]) {
  for (byte i = 0; i < 4; i++) {
    if (Array1[i] != Array2[i])
      return false;
  }
  return true;
}