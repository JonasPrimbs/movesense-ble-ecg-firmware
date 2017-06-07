//
//  WBCentralStatistics.h
//  Whiteboard
//
//  Created by Nikita Zhuk on 11/02/16.
//  Copyright (c) 2015 Suunto. All rights reserved.
//

#import <Foundation/Foundation.h>


NS_ASSUME_NONNULL_BEGIN

/**
 WBCentralStatistics manages collection and output of various statistics of WBCentral operations. It is controlled by the environment variables of the app.
 
 Set the following environment variables to enable BLE statistic logging:
 - Key: BLE_STATS_WRITES, Value: 0/1. Enables logging of byte counts of complete data chunk which were requested to be written to BLE characteristic.
 - Key: BLE_STATS_RECEIVES, Value: 0/1. Enables logging of byte counts of complete data chunk which were received from remote device BLE characteristic.
 - Key: BLE_STATS_CHUNK_WRITES, Value: n (unsigned integer). Enables output of full BLE statistic report each n writes
 - Key: BLE_STATS_CHUNK_RECEIVES, Value: n (unsigned integer). Enables output of full BLE statistic report each n receives
 */

@interface WBCentralStatistics : NSObject

/**
 @param data The complete data chunk which was requested to be written to BLE characteristic.
 This data will be encoded and sent in chunks. Each chunk will be logged via @c logWrite:
 */
- (void)logWriteOperationWithData:(NSData *)data;

/**
 @param data The complete data chunk which was received from remote device BLE characteristic.
 This data is collected from chunks and decoded. Each chunk was previously logged via @c logReceive:
 */
- (void)logReceiveOperationWithData:(NSData *)data;

/**
 @param byteCount Number of bytes written to BLE characteristic
 */
- (void)logWrite:(NSUInteger)byteCount;

/**
 @param byteCount Number of bytes received from BLE characteristic's notification
 */
- (void)logReceive:(NSUInteger)byteCount;

@end

NS_ASSUME_NONNULL_END
