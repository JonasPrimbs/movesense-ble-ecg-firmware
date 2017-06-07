//
//  WBCentral.h
//  Whiteboard
//
//  Created by Nikita Zhuk on 29/06/15.
//  Copyright (c) 2015 Suunto. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "WBPeripheralOperations.h" // For WBPeripheralWriteOperationCallback

NS_ASSUME_NONNULL_BEGIN

typedef void (^WBCentralConnectPeripheralFailureCallback)(NSError *_Nullable error);
typedef void (^WBCentralDidConnectPeripheralCallback)(NSUUID* uuid, BOOL result);
typedef void (^WBCentralDidDisconnectPeripheralCallback)(NSUUID* uuid, BOOL wasRemotelyInitiated);
typedef void (^WBCentralDidReceiveDataCallback)(NSUUID* uuid, NSData* data);
typedef void (^WBCentralDidNotifyConnectPeripheralCallback)(NSUUID* uuid, BOOL result);
typedef void (^WBCentralDidNotifyDisconnectPeripheralCallback)(NSUUID* uuid, BOOL wasRemotelyInitiated);

/**
   This class serves in BLE Central mode - scanning & connecting to Peripherals.
   It also serves in GATT Client mode - subscribes to characteristics provided by Peripherals and writes & reads from those
   characteristics.
   A single instance of WBCentral can be used to communicate with multiple Peripherals.
 */
@interface WBCentral : NSObject

@property(nonatomic, copy) WBCentralDidConnectPeripheralCallback didConnectToPeripheral;
@property(nonatomic, copy) WBCentralDidDisconnectPeripheralCallback didDisconnectFromPeripheral;
@property(nonatomic, copy) WBCentralDidReceiveDataCallback didReceiveDataFromPeripheral;
@property(nonatomic, copy) WBCentralDidNotifyConnectPeripheralCallback didNotifyConnectToPeripheral;
@property(nonatomic, copy) WBCentralDidNotifyDisconnectPeripheralCallback didNotifyDisconnectFromPeripheral;

/**
 @return How many bytes are included in each BLE write packet by this class.
 */
+ (NSUInteger)MTU;

/**
   Create a singleton of this class
 */
+ (instancetype)sharedCentral;

/**
   Starts connection attempt to a peripheral with given @c uuid
   Calls @c didConnectToPeripheral callback when the connection attempt either succeeds or fails.
   Note that there is no timeout - connection may be created after long period of time (even hours) after it has been requested.
   @param uuid UUID of the peripheral
   @param failureCallback If connection will fail because of characteristic subscription error (usually caused by failed pairing), this callback will be called. Nil callbacks are ignored.
   @param resetAutoReconnectStatus If this device has been previously disabled for auto-reconnections and this flag is @c YES, auto-reconnections will be enabled again.
 */
- (void)connectToPeripheralWithUUID:(NSUUID *)uuid
                    failureCallback:(nullable WBCentralConnectPeripheralFailureCallback)failureCallback
           resetAutoReconnectStatus:(BOOL)resetAutoReconnectStatus;

/**
   Starts disconnect attempt from a peripheral with given @c uuid
   If peripheral was connected when this method was called, calls @c didDisconnectFromPeripheral callback when the disconnect either succeeds or fails.
   This method can also be used to cancel a connection attempt previously started with @c connectToPeripheralWithUUID: even if
   connection has not yet been established
   @param uuid UUID of the peripheral
   @return @c YES, if peripheral was found with given @c uuid and the request to cancel/close the connection to that peripheral was sent successfully. @c NO if the peripheral was not found, or it was found but it was already disconnected.
 */
- (BOOL)disconnectFromPeripheralWithUUID:(NSUUID *)uuid;

/**
   Writes the given @c data to peripheral with given @c uuid. Writing is asyncronous and happens in chunks.
   When write completes, the @c completion callback is called with result.
   @param data Data to be written. Can be of any size, cannot be nil.
   @param uuid UUID of the peripheral to write data to.
   @param completion This handler will be called when operation completes with success or failure.
 */
- (void)writeData:(NSData*)data toPeripheralWithUUID:(NSUUID*)uuid completion:(WBPeripheralWriteOperationCallback)completion;

- (NSArray *)retrieveCurrentlyPairedDevices;

- (NSString *)parseDeviceSerialFromLocalName:(NSString*)localName;

/**
   Prevent receiver from automatically reconnecting to device with given @c serial after it has been disconnected.
   If this method is not called, the default behaviour is to reconnect to device, also when the app is suspended in the background.
   @param serial Serial number of the device for which auto-reconnect should be disabled. Duplicate serials will be ignored. Cannot be nil.
 */
- (void)disableAutoReconnectForDeviceWithSerial:(NSString *)serial;

@end

NS_ASSUME_NONNULL_END
