#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int rot=13;
int gelb=12;
int gruen=11;
void setup_1()
{
Serial.begin(9600);
SPI.begin();
mfrc522.PCD_Init();
lcd.init();
lcd.backlight();
pinMode(rot, OUTPUT);
pinMode(gelb, OUTPUT);
pinMode(gruen, OUTPUT);
}
void loop_1()
{
while ( ! mfrc522.PICC_IsNewCardPresent())
{
lcd.setCursor(0, 0);
lcd.print("Karte bitte       ");
digitalWrite(gelb, HIGH);
digitalWrite(gruen, LOW);
digitalWrite(rot, LOW);
}
if ( mfrc522.PICC_ReadCardSerial())
{
return;
}
while ( mfrc522.PICC_IsNewCardPresent())
{
  lcd.setCursor(0, 0);
  lcd.print("Zutritt erlaubt       ");
  digitalWrite(gruen, HIGH);
  digitalWrite(rot, LOW);
  digitalWrite(gelb, LOW);
  delay(5000);
}
long code=0;
for (byte i = 0; i < mfrc522.uid.size; i++)
{
  code=((code+mfrc522.uid.uidByte[i])*10);
}
Serial.print("Die Kartennummer lautet");
Serial.println(code);
}
