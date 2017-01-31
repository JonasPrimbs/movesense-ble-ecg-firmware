//
//  WBSerialFrame.h
//  sds-wb-ios
//
//  Created by Nikita Zhuk on 3.2.2016.
//
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface WBSerialFrame : NSObject

/**
 Encodes the given @param data with SerialFrame encoding and includes CRC of the data.
 @return Encoded data or nil, if encoding fails.
 */
+ (nullable NSData *)encodedDataFromData:(NSData *)data;

/**
 Decodes the given @param data from SerialFrame encoding and checks CRC of the data.
 @return Decoded data or nil, if CRC check fails. 
 */
+ (nullable NSData *)decodedDataFromData:(NSData *)data;

@end

NS_ASSUME_NONNULL_END
