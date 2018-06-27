## Migration to 1.6.1

**NOTE:** Both of the new features listed below require the use of latest *./samples/\_build* folder from the release.

### Reset reason string

If you want to receive information from the sensor about what caused the last reset, you can add following code to your sensor application:

```
// Read last reset reason and log it using DebugLogger
  char buf[60];
  if (faultcom_GetLastFaultStr(false, buf, sizeof(buf))) {
      DebugLogger::info("Last reset reason: %s", buf);
	  // ... Do something else with the reason ...
  }
```

The function fills the buffer and returns *true* if there was an error in last reset (e.g. ASSERT) or just returns *false* if not. 
It is possible to read the reason for example in the startup and store it (add *buf[40]* as your *Provider class* member), 
but one can save a bit of RAM by just calling the function when the reason is needed 
(e.g. when returning the info from your own service).

### UTC stays over reset ###

A new feature in 1.6.1 is the keeping of the UTC clock over resets (if the sensor is set to FULL_POWER_OFF, the time is lost). 
The accuracy of the clock suffers when reset occurs, and the error incurred is typically around +- 1 second. If you rely on more accurate
UTC time, make sure that you detect the reset (when your code runs *startModule()*) instead of relying on clock resetting to Jan 1st 2015.


## Migration to 1.5.0

### asyncSubscribe workaround

Whiteboard is currently not caching parameters for the paths. If you are using:  

```
wb::Result result = asyncSubscribe(WB_RES::LOCAL::MEAS_ACC_SAMPLERATE::ID, AsyncRequestOptions(&remoteRequestId, 0, true), ACC_SAMPLERATE);
```

then you will receive all data from Accelerometer (also for higher freqences). To avoid this situation new method of subscribcion can be implemented:  

```
class ...{
   ...
   whiteboard::ResourceId	mMeasAccResourceId;
}

//-------------

wb::Result result = getResource("Meas/Acc/104", mMeasAccResourceId);
if (!wb::RETURN_OKC(result))
{
    return whiteboard::HTTP_CODE_BAD_REQUEST;
}
result = asyncSubscribe(mMeasAccResourceId, AsyncRequestOptions(&remoteRequestId, 0, true));
```

When resource is not longer used, resources should be released:

```
wb::Result result = asyncUnsubscribe(mMeasAccResourceId, NULL);
if (!wb::RETURN_OKC(result))
{
	//Do something
}
isRunning = false;
releaseResource(mMeasAccResourceId);
```

## Migration to 1.3.0 - 1.4.0

These versions contain the same updated bootloader and SoftDevice as version 1.2.0 - if you're updating from a version earlier than 1.2.0, please refer to section "Migration to 1.2.0" below as the same guidance applies to these versions as well.

## Migration to 1.2.0 

### Updating bootloader and SoftDevice

Movesense device software version 1.2.0 includes a new SoftDevice Bluetooth stack version 4.0.5 (by Nordic Semiconductor) as well as new bootloader. When updating the sensor software using DFU from 1.1 version (or older), **the bootloader must be updated first!**  

You can either:  

- use the "*bootloader update DFU package*" (found in migration/bootloader_2.zip), **OR**  
- use a DFU package that includes both the new bootloader and SoftDevice (package is automatically built when compiling sensor applications with movesense-device-lib >= 1.2).

**NOTE: Once updated, the bootloader or SoftDevice cannot be downgraded**  
 
### Updating nrfutil

If you do not use portable-python from movesense-device-lib, but ex. your system one. You should update nrfutil to version 3.4.0 which support the new SoftDevice.
  
`pip install nrfutil==3.4.0`

### New LED API

After 1.2 release the optimal way of use the LED has changed.

a) Old commands:

`wbcmd --port COM13 --path 'Component/Led' --op put --opdatatype bool --opdata false`

`asyncPut(WB_RES::LOCAL::COMPONENT_LED(), AsyncRequestOptions::Empty, true);`

b) New commands: 

`wbcmd --port COM13 --path 'Component/Leds/0' --op put --opdatatype LedState --opdata '{"IsOn":false}'    // '0' is ledIndex`

`WB_RES::LedState ledState = {true};`

`asyncPut(WB_RES::LOCAL::COMPONENT_LEDS_LEDINDEX(), NULL, 0, ledState);    // '0' is ledIndex`

Also some new LED options are available:

a) via wbcmd:

`wbcmd --port COM13 --path 'Component/Leds'`

`wbcmd --port COM13 --path 'Component/Leds/0'    // '0' is ledIndex`

b) via internal request: 

`asyncGet(WB_RES::LOCAL::COMPONENT_LEDS());`

`asyncGet(WB_RES::LOCAL::COMPONENT_LEDS_LEDINDEX(), AsyncRequestOptions::Empty, 0);    // '0' is ledIndex`

## Migration to 1.0.1

- Logbook/EEPROM service
    -If you are using the Logbook and EEPROM parallel (The first EEPROM for Logbook and second for The EEPROM Service)
      Make sure that you will add the limit to your App.cpp file:
```LOGBOOK_MEMORY_AREA(offset, size);```

      Please, check the plain_app sample.

## Migration to 1.0.0

- Make sure that you use the latest *"samples/_build"* helper

- Update the CMakeLists.txt of your project to one from samples

- If you use the datalogger or the logbook, set module as enabled in App.cpp:  
```OPTIONAL_CORE_MODULE(DataLogger, true)```  
```OPTIONAL_CORE_MODULE(Logbook, true)```

- ECG sample rate
    - Check supported sample rates by reading the path: */Meas/ECG/Info*  
	   Response example:

     
		```{```    
		```  "response": 200,```  
		```  "responsestring": "HTTP_CODE_OK",```  
		```  "operation": "get",```  
		```  "uri": "/net/174010101010/Meas/ECG/Info",```  
		```  "content": {```  
		```    "CurrentSampleRate": 0,```  
		```    "AvailableSampleRates": [```  
		```      125,```  
		```      128,```  
		```      200,```  
		```      250,```  
		```      256,```  
		```      500,```  
		```      512```  
		```    ],```  
		```    "ArraySize": 16```  
		```  },```  
		```  "querytimems": 30,```  
		```  "querytimens": 30291550```  
		```}```  

    - Use the new path for ECG: */Meas/ECG/{Sample_rate}*  
	  Response example:

		  ```   /net/174010101010/Meas/ECG/125::onSubscribeResult```  
		  ```Subscribed and listening for notifications. Press ESC to stop:```  
		  ```@153 { {```  
		  ```"Samples": [```  
		  ```  0,```  
		  ```  0,```  
		  ```  -1,```  
		  ```  -3,```  
		  ```  0,```  
		  ```  -4,```  
		  ```  -58,```  
		  ```  77,```  
		  ```  -12,```  
		  ```  -162,```  
		  ```  142,```  
		  ```  -605,```  
		  ```  248,```  
		  ```  231,```  
		  ```  -3063,```  
		  ```  2406```  
		  ```]```  
		  ```} }```  
		  ```@279 { {```  
		  ```"Samples": [```  
		  ```  7444,```  
		  ``` -6374,```  
		  ```  -18151,```  
		  ```  7849,```  
		  ```  68088,```  
		  ```  113667,```  
		  ```  111805,```  
		  ``` 84464,```  
		  ```  73809,```  
		  ```  84868,```  
		  ```  89333,```  
		  ```  84991,```  
		  ```  83335,```  
		  ```  81508,```  
		  ```  80044,```  
		  ```  78287```  
		  ```]```  
		  ```} }```  

	
- Own application info can be added by using APPINFO (Name, Version, Company)  
  Code example in App.cpp:

    ```APPINFO_NAME("Sample HelloWorld");```  
    ```APPINFO_VERSION("1.0.0");```  
    ```APPINFO_COMPANY("Movesense");```  

    - Check application info by reading the path */Appinfo*  
    - There is also possibility to check if optional modules are enabled.  
	Response example:  


        ```{```  
    	```"response": 200,```  
    	```  "responsestring": "HTTP_CODE_OK",```  
    	```  "operation": "get",```  
    	```  "uri": "/net/174010101010/AppInfo",```  
    	```  "content": {```  
    	```    "appName": "Sample HelloWorld",```  
    	```    "appVersion": "1.0.0",```  
    	```    "appProvider": "MoveSense",```  
    	```    "modules": {```  
    	```      "data": [```  
    	```        {```  
    	```          "name": "DataLogger",```  
    	```          "status": true```  
    	```        },```  
    	```        {```  
    	```          "name": "Logbook",```  
    	```          "status": true```  
    	```        },```  
    	```        {```  
    	```          "name": "LedService",```  
    	```          "status": true```  
    	```        },```  
    	```        {```  
    	```          "name": "IndicationService",```  
    	```       "status": true```  
    	```     },```  
    	```    {```  
    	```       "name": "BleService",```  
    	```          "status": true```  
    	```        },```  
    	```        {```  
    	```          "name": "EepromService",```  
    	```          "status": true```  
    	```        }```  
    	```      ]```  
    	```    }```  
    	```  },```  
   	    ```  "querytimems": 51,```  
	    ```  "querytimens": 51898791```  
	    ```}```  
	
- The way of calculating BLE DFU MAC address has changed. Instead of incrementing last byte by one you should read the */Info* path where all needed addresses are listed.  
**"BLE"** address is main one used by application.  
**"DFU-BLE"** address is DFU Mode address used during firmware update.  
Respone example:


	```{```  
	```  "response": 200,```  
	```  "responsestring": "HTTP_CODE_OK",```  
	```  "operation": "get",```  
	```  "uri": "/net/174010101010/Info",```  
	```  "content": {```  
	```    "manufacturerName": "Suunto",```  
	```    "brandName": null,```  
	```    "productName": "SmartSensor2",```  
	```    "variant": "_Variant",```  
	```    "design": null,```  
	```    "hwCompatibilityId": "C",```  
	```    "serial": "174010101010",```  
	```    "pcbaSerial": "_PCBASERIAL_PCBASERIAL_PCBASERIAL_",```  
	```    "sw": "0.11.0",```  
	```    "hw": "_HW_HW",```  
	```    "additionalVersionInfo": null,```  
	```    "addressInfo": [```  
	```      {```  
	```        "name": "BLE",```  
	```        "address": "FF-FF-AA-AA-FF-FF"```  
	```      },```  
	```      {```  
	```        "name": "DFU-BLE",```  
	```        "address": "DA-40-B1-94-B8-FC"```  
	```      }```  
	```    ],```  
	```    "apiLevel": "1"```  
	```  },```  
	```  "querytimems": 58,```  
	```  "querytimens": 58683239```  
	```}```  
