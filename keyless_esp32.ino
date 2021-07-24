#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define kontak 2

String knownBLEAddresses[] = {""}; //masukan address itag kalian
int device_found;
int scanTime = 1; //In seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
void onResult(BLEAdvertisedDevice advertisedDevice) {
boolean dapat = false;

for (int i = 0; i < (sizeof(knownBLEAddresses) / sizeof(knownBLEAddresses[0])); i++)
{ 
if (strcmp(advertisedDevice.getAddress().toString().c_str(), knownBLEAddresses[i].c_str()) == 0)
{
dapat = true;
}
if (dapat == true){
Serial.println("awokawok");
Serial.println(advertisedDevice.getRSSI());       
if (advertisedDevice.getRSSI()> -80){ // rssi = -60(dekat) || rssi = -90(jauh)
  device_found = true;
}
else {
  device_found = false;
}
advertisedDevice.getScan()->stop();
delay(100);
}
      //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str()); //hilangkan (//) untuk mencari address itag kalian
}
}
};
void setup() {
  Serial.begin(115200); //Enable UART on ESP32
  Serial.println("Scanning..."); // Print Scanning
  pinMode(kontak, OUTPUT); //make BUILTIN_LED pin as output
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); //Init Callback Function
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100); // set Scan interval
  pBLEScan->setWindow(99);  // less or equal setInterval value
}
void loop() 
{
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  if (device_found == true ){
    digitalWrite(kontak, HIGH);
  }
  else {
    digitalWrite(kontak, LOW);
  }
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay (100);
}
