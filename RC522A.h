#include <SPI.h>
#include <MFRC522.h>

#define rfidaMISO 19
#define rfidaMOSI 23
#define rfidaSCLK 18
#define rfidaCS    5
#define rfidaIRQ  34
#define rfidaRST   4

uint64_t nextReadA=0;
uint32_t waitToNextReadA=2000;

MFRC522 mfrc522A(rfidaCS,rfidaRST);

void initRC522A() {
  SPI.begin(rfidaSCLK,rfidaMISO,rfidaMOSI,rfidaCS);
  mfrc522A.PCD_Init(rfidaCS,rfidaRST);
  if (debug) { Serial.println("RFID Reader A started."); } }

void rc522WorkerA() {
  if (millis()<nextReadA) { return; }

  SPI.begin(rfidaSCLK,rfidaMISO,rfidaMOSI,rfidaCS);
  mfrc522A.PCD_Init(rfidaCS,rfidaRST);
  if (!mfrc522A.PICC_IsNewCardPresent()) { SPI.end(); return; }
  if (!mfrc522A.PICC_ReadCardSerial()) { SPI.end(); return; }

  String tagTypeA=mfrc522A.PICC_GetTypeName(mfrc522A.PICC_GetType(mfrc522A.uid.sak));

  uint64_t tagUIDA=0; for (uint8_t i=0;i<mfrc522A.uid.size;i++) {
    tagUIDA|=(uint64_t)mfrc522A.uid.uidByte[i]<<(8*i); }

  mfrc522A.PICC_HaltA(); mfrc522A.PCD_StopCrypto1(); SPI.end();

  nextReadA=millis()+waitToNextReadA;

  static uint64_t adminAtimer;
  if (checkAdmin(tagUIDA)) { adminAtimer=millis()+adminDelay; yellowOn(adminDelay); redOn(adminDelay); }
  else if (adminAtimer>=millis()) { newTag(tagUIDA); adminAtimer=0; greenOn(); yellowOn(); redOff(); }
  else if (adminAtimer<millis() && tagExist(tagUIDA)>=0) { setBarrier('A',tagUIDA); }
  else { redOn(); }

  if (debug) {
    Serial.println("Tag detected Reader A");
    Serial.print("  Type: "); Serial.println(tagTypeA);
    Serial.print("  UID: "); Serial.println(String(tagUIDA,HEX)); } }
