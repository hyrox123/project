//
//  ExceptionHandler.h
//  advPannel
//
//  Created by apple on 15/10/29.
//  Copyright © 2015年 jason. All rights reserved.
//

#import <Foundation/Foundation.h>


extern NSString *const UncaughtExceptionHandlerSignalKey;
extern NSString *const SingalExceptionHandlerAddressesKey;
extern NSString *const ExceptionHandlerAddressesKey;

@interface ExceptionHandler : NSObject
+ (void)installExceptionHandler;
+ (NSArray *)backtrace;
@end
