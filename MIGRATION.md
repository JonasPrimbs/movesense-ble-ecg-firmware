**Migration to 1.2.0 (bootloader)**
- The new bootloader contains a new SoftDevice made by Nordic Semiconductor in version 4.0.5. You can find the new bootloader in migration/bootloader_2.zip. Before you will send a new dfu with 1.2.0 based software you HAVE TO update bootloader, without this step your Movesense will be always in dfu mode. Your Movesense automatically will go to the second dfu mode after bootloader update. The bootloader can not be downgraded.


**Migration to 1.0.1**

- Logbook/EEPROM service
    -If you are using the Logbook and EEPROM parallel (The first EEPROM for Logbook and second for The EEPROM Service)
      Make sure that you will add the limit to your App.cpp file:
```LOGBOOK_MEMORY_AREA(offset, size);```

      Please, check the plain_app sample.

**Migration to 1.0.0**

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