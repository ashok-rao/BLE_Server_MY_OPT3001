#include "mbed.h"
#include "rtos.h"
#include "stdio.h"
#include "OPT3001.h"
#include "ble\BLE.h"
#include "ble\Gap.h"

#define SERVICE_UUID			<Service_UUID here>//0xA000
#define CHARACTERISTIC_UUID		<Characteristic UUID here>//0xA001

//BLE ble;
BLE &ble = BLE::Instance();
DigitalOut led1(LED1);
DigitalOut led2(LED2);

const static char DEVICE_NAME[] = "Your device name here"//"OPT3001";
const static uint16_t uuid16_list[] = {SERVICE_UUID};
float lux_data = 0.0;

//Uncomment the line below to define a characteristic.
//ReadOnlyGattCharacteristic<float> SensorReading(CHARACTERISTIC_UUID, &lux_data, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    BLE::Instance(BLE::DEFAULT_INSTANCE).gap().startAdvertising(); // restart advertising
}

void bleInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
    BLE &ble   = params->ble;
    ble_error_t error = params->error;
	
	const Gap::AdvertisementCallbackParams_t *params_advCallback;

	printf("Inside BLE..starting payload creation..\n");
	
    ble.gap().onDisconnection(disconnectionCallback);
	

	GattCharacteristic *charTable[] = {&SensorReading};
    GattService sensorService(SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
    ble.addService(sensorService);

		
    /* Setup advertising. */
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
		
		if (error == BLE_ERROR_NONE) {
        led2 = 1;
    }
		
    ble.gap().setAdvertisingInterval(1000); /* 1000ms */
    error = ble.gap().startAdvertising();
		printf("ble.gap().startAdvertising() => %u\r\n", error);
	
	/****************************************
	
	TODO : Print the address of the server. The below is partially working.
	
	printf("Hello..My addr is: [%02x %02x %02x %02x %02x %02x] \r\n", params_advCallback->peerAddr[5], params_advCallback->peerAddr[4], params_advCallback->peerAddr[3], params_advCallback->peerAddr[2], params_advCallback->peerAddr[1], params_advCallback->peerAddr[0]);
	****************************************/
	
}
/*
void periodicCallback(void) {
    led1 = !led1;
}
*/


/************************ Thread #1 for light sensor ************************/

void light_sensor(void) {
	read_DeviceID();
	
	while(true) {
		lux_data = read_sensor();
		printf("Lux = %0.2f\n", lux_data);
		Thread::wait(1000);
		ble.updateCharacteristicValue(SensorReading.getValueHandle(), (uint8_t *)&lux_data, sizeof(float));	
	}
}

/************************ Thread #2 for BLE activities ************************/

void Bluetooth_LE_server(void) {
	
		
		ble.init(bleInitComplete);
//    ble.onDisconnection(disconnectionCallback);
//	
//		GattCharacteristic *charTable[] = {&SensorReading};
//    GattService sensorService(SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
//    ble.addService(sensorService);
//    
//    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
//    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
//    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *) DEVICE_NAME, sizeof(DEVICE_NAME));
//		ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
//    
//    ble.gap().setAdvertisingInterval(1000);
//    ble.gap().startAdvertising();
//		
//	while(true) {
//		ble.waitForEvent();
//	}
	while(1) {
		ble.processEvents();
	}
	//This statement might be un-reachable...??
	Thread::wait(1000);
}

/************************ Thread #3 main() ************************/

int main() {
	//printf("Inside main\n");

		Thread thread1;
		Thread thread2;
	
		thread1.start(light_sensor);
		thread2.start(Bluetooth_LE_server);
	
/*
	while(true) {
		ble.waitForEvent();
	}
*/	
	while(true) {
		led1 = !led1;
		Thread::wait(500);
	}
}
