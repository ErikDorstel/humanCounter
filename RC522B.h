#include <SPI.h>
#include <MFRC522.h>

#define rfidbMISO 12
#define rfidbMOSI 13
#define rfidbSCLK 14
#define rfidbCS   15
#define rfidbIRQ  35
#define rfidbRST  33

uint64_t nextReadB=0;
uint32_t waitToNextReadB=2000;

MFRC522 mfrc522B(rfidbCS,rfidbRST);

void initRC522B() {
  SPI.begin(rfidbSCLK,rfidbMISO,rfidbMOSI,rfidbCS);
  mfrc522B.PCD_Init(rfidbCS,rfidbRST);
  if (debug) { Serial.println("RFID Reader B started."); } }

void rc522WorkerB() {
  if (millis()<nextReadB) { return; }

  SPI.begin(rfidbSCLK,rfidbMISO,rfidbMOSI,rfidbCS);
  mfrc522B.PCD_Init(rfidbCS,rfidbRST);
  if (!mfrc522B.PICC_IsNewCardPresent()) { SPI.end(); return; }
  if (!mfrc522B.PICC_ReadCardSerial()) { SPI.end(); return; }

  String tagTypeB=mfrc522B.PICC_GetTypeName(mfrc522B.PICC_GetType(mfrc522B.uid.sak));

  String tagUIDB=""; for (uint8_t i=0;i<mfrc522B.uid.size;i++) {
    if (mfrc522B.uid.uidByte[i]<16) { tagUIDB+="0"; }
    tagUIDB+=String(mfrc522B.uid.uidByte[i],HEX); }

  mfrc522B.PICC_HaltA(); mfrc522B.PCD_StopCrypto1(); SPI.end();

  nextReadB=millis()+waitToNextReadB;

  static uint64_t adminBtimer;
  if (checkAdmin(tagUIDB)) { adminBtimer=millis()+adminDelay; yellowOn(adminDelay); redOn(adminDelay); }
  else if (adminBtimer>=millis()) { newTag(tagUIDB); adminBtimer=0; greenOn(); yellowOn(); redOff(); }
  else if (adminBtimer<millis() && tagExist(tagUIDB)>=0) { setBarrier('B',tagUIDB); }
  else { redOn(); }

  if (debug) {
    Serial.println("Tag detected Reader B");
    Serial.print("  Type: "); Serial.println(tagTypeB);
    Serial.print("  UID: "); Serial.println(tagUIDB); } }
