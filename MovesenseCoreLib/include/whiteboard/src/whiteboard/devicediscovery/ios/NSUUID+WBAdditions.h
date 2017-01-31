//
//  NSUUID+WBAdditions.h
//  Whiteboard
//
//  Created by Nikita Zhuk on 06/07/15.
//  Copyright (c) 2015 Suunto. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSUUID (WBAdditions)

+ (nullable NSUUID*)UUIDFromUTF8String:(const char* __nonnull)nullTerminatedCString __attribute__((nonnull));

@end
