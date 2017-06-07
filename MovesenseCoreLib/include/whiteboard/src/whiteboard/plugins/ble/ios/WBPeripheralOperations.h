//
//  WBPeripheralOperations.h
//  TestCentralApp
//
//  Created by Nikita Zhuk on 01/07/15.
//  Copyright (c) 2015 Suunto Oy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^WBPeripheralWriteOperationCallback)(BOOL result);

/**
   Identifying information about the target of an operation
 */
@interface WBPeripheralOperationTarget : NSObject <NSCopying>

@property(nonatomic, readonly) CBPeripheral* peripheral;
@property(nonatomic, readonly) CBCharacteristic* characteristic;

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithPeripheral:(CBPeripheral*)peripheral characteristic:(CBCharacteristic*)characteristic NS_DESIGNATED_INITIALIZER;

@end

/**
   A class to represent a data write operation to a @c WBPeripheralOperationTarget
 */
@interface WBPeripheralWriteOperation : NSObject

@property(nonatomic, readonly) WBPeripheralOperationTarget* target;
@property(nonatomic, readonly) WBPeripheralWriteOperationCallback completion;

- (instancetype)init NS_UNAVAILABLE;

/**
   Tracks a single write operation

   @param target Target of the write operation
   @param data The data to be sent. Data can be of any size.
   @param mtu The maximum size of data chunks which will be provided by the @c nextChunk method.
    This value should correspond to the preferred MTU of BLE connection - usually from 20 to 512.
   @param completion Completion callback which is attached to this write operation.
 */
- (instancetype)initWithTarget:(WBPeripheralOperationTarget*)target
                          data:(NSData*)data
                           mtu:(NSUInteger)mtu
                    completion:(WBPeripheralWriteOperationCallback)completion NS_DESIGNATED_INITIALIZER;

/**
   @return Empty NSData if all chunks have been already returned.
 */
- (NSData*)nextChunk;

@end

/**
   Tracks a single read operation.
 */
@interface WBPeripheralReadOperation : NSObject

@property(nonatomic, readonly, nonnull) WBPeripheralOperationTarget* target;

/**
   The data collected so far. Can be empty data, is never nil.
 */
@property(nonatomic, readonly, nonnull) NSData* data;


- (instancetype)init NS_UNAVAILABLE;

/**
   @param target Target of the read operation
 */
- (instancetype)initWithTarget:(WBPeripheralOperationTarget*)target NS_DESIGNATED_INITIALIZER;

/**
   Appends given data to this read operation

   @param data Data to append. Can be of any size, must not be nil.
 */
- (void)appendData:(NSData*)data;

/**
   Clears internal data buffer. After calling this method, @c data property will return empty data.
 */
- (void)clearData;

@end

NS_ASSUME_NONNULL_END
