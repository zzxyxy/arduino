//#include "EspMQTTClient.h"
// #include "ArduinoJson.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

/*
EspMQTTClient client(
  "zxyxy_iot",
  "mPHhrbF3NZq2wXUJ5GrMAvSX7PMf8Mwc9eH",
  "172.20.1.1",  // MQTT Broker server ip
  "TestClient"      // Client name that uniquely identify your device
);
*/

#define SCAN_TIME 30     // seconds
BLEScan *pBLEScan;


class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
//    StaticJsonDocument<50> xx;
    String x;
//    xx["adv"] = advertisedDevice.toString();
//    serializeJson(xx, x);
//    client.publish("test", x);
  }
};


void scan(std::string &doc)
{
  BLEScanResults foundDevices = pBLEScan->start(SCAN_TIME);
  int count = foundDevices.getCount();
//  doc["Count"] = count;
  for (int i = 0; i < count; i++)
  {
//    doc.add(i);

    BLEAdvertisedDevice d = foundDevices.getDevice(i);
//    JsonObject obj1=doc.createNestedObject();
    
    doc=d.getAddress().toString();
    Serial.println(d.getName().c_str());

/*
    obj1["rssi"]=d.getRSSI();
    if (d.haveName()) {
      obj1["name"]=d.getName();
    }

    if (d.haveAppearance())
    {
      obj1["Appearance"] = d.getAppearance();
    }
/*
    if (d.haveManufacturerData())
    {
      std::string md = d.getManufacturerData();
      uint8_t *mdp = (uint8_t *)d.getManufacturerData().data();
      char *pHex = BLEUtils::buildHexData(nullptr, mdp, md.length());
      ss << ",\"ManufacturerData\":\"" << pHex << "\"";
      free(pHex);
    }

    if (d.haveServiceUUID())
    {
      ss << ",\"ServiceUUID\":\"" << d.getServiceUUID().toString() << "\"";
    }

    if (d.haveTXPower())
    {
      ss << ",\"TxPower\":" << (int)d.getTXPower();
    }
*/
  }
}



void setup() 
{
  Serial.begin(9600);
  BLEDevice::init("");
  
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(0x50);
  pBLEScan->setWindow(0x30);   
}

String cw;

void onConnectionEstablished() {
//  client.subscribe("my", [] (const String &payload)  {
//    Serial.println(payload);
//  });

}

void loop() {
//  client.loop();
//  StaticJsonDocument<100> res;
  std::string res;
  scan(res);
  Serial.println(res.c_str());
  
  delay(2000);
  String out;
//  serializeJson(res, out);
//  client.publish("test", out);
//  client.publish("test", cw);

}
