# Movesense Sensor Data Storage #

This document describes the data storage features of the Movesense sensor. 

## Technical capabilities ##

The Movesense sensor contains 128kB EEPROM memory for storing data. To assist software developer to utilize this memory the *Movesense Core Library* contains two services */DataLogger* and */Logbook* that make it effortless to store and retrieve the data from the sensor. The EEPROM memory does not have a filesystem, but it is instead organized as a circular buffer *ChunkStorage*, which means that as the memory gets full, the old data gets overwritten. To distinguish between recordings the *ChunkStorage* uses a *SessionID* that is incremented every time that the *ChunkStorage* is restarted (at the  moment this only happens every time that the sensor is reset).


## /DataLogger service ##

To help logging data from Whiteboard *ResourceProviders* the Movesense Core Library contains a *DataLogger* service. This is a simple service that can be configured to subscribe to any whiteboard resources (even the ones you create in the application!) and record all the  data (notifications) they provide. /DataLogger has two resources: */DataLogger/Config* and */DataLogger/State*. Config contains the current configuration of the logger (what data to log), and state describes the state of the logger (see: DataLoggerState in *datalogger.yaml*) 


The DataLogger service stores the data using the *ChunkStorage* class. The data is stored in binary format that is deduced automatically from the yaml files as a part of the application build process. The generated code has some limitations, such as: 

 - only 1st element of an array is stored
 - no string support
 - optional data still takes space even if it has *null* value

The *ChunkStorage* stores data one entry at the time until the internal buffer for the resource is full and then synchronizes the data to the EEPROM memory. If the memory gets full the ChuckStorage starts over from the beginning and begins overwriting the old data.


### DataLogger usage ###

To use datalogger to log data, one needs to 

1. Configure the DataLogger. This is done by **PUT**:ing *DataLoggerConfig* -object to the */DataLogger/Config* -resource. For example to log accelerometer & gyro data, use configuration:

```json
{
	"dataEntries" : {
		"dataEntry" : [{
				"path" : "/Device/Measurement/LinearAcceleration"
			}, {
				"path" : "/Device/Measurement/AngularVelocity"
			}
		]
	}
}
```

2. To start logging, PUT value *DATALOGGER_LOGGING* (=3) to */DataLogger/State* resource

3. To stop logging, PUT value *DATALOGGER_READY* (=2) to */DataLogger/State* resource



## /Logbook service ##

Logbook service is used to access the data stored in the EEPROM memory. The yaml file contains lots of different things, but Movesense sensor only supports a small subset of that API (see below). The Logbook service can enumerate the contents of the EEPROM memory (*/Logbook/Entries* resource) and provide the data of the made recordings. 

The recordings are returned in Suunto Oy:s SBEM-format, which can be converted to JSON using the Movesense Mobile Library's *sbemToJson* functionality (not fully completed when writing this). To convert the recording to JSON both *descriptors* and *data* are needed. The *Descriptors* for given DataLogger configuration only change when Movesense Core Library or the application change, so you can cache them in your mobile application until you update the sensor application. 

### /Logbook usage ###

To get recording from the Movesense sensors EEPROM storage, you need to:

1. Do **GET** on  */Logbook/Entries*. This returns a list of LogEntry objects. If the status was HTTP_OK, the list is complete. If the result code is HTTP_CONTINUE, you must GET again with the parameter StartAfterId set to the Id of the last entry you received and you'll get the next entries.

2. Choose the Log that you are interested in and notice the Id of it.

3. Fetch the descriptors with **GET** to */Logbook/byId/<Id>/Descriptors*. This returns a bytestream with the similar HTTP_CONTINUE handling as above. However you **must** keep re-requesting the **GET** until you get error or HTTP_OK, or the Logbook service will stay "in the middle of the stream" (we hope to remove this limitation in the future).

4. Fetch the data with **GET** to */Logbook/byId/<Id>/Data*. This returns also a bytestream (just like the */Logbook/Descriptors* above). 

5. Convert the data using the converter tools or classes. (To Be Continued...)

