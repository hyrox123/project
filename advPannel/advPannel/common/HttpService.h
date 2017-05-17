//
//  HttpService.h
//  YSTParentClient
//
//  Created by apple on 14-11-11.
//  Copyright (c) 2014年 jason. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//#define CLU_SERVER_IP  @"182.92.167.214"
//#define CLU_SERVER_PORT 8088

#define CLU_SERVER_IP  @"222.186.3.104"
#define CLU_SERVER_PORT 8080

@class NewsItem;
@class UserBaseInfo;
@class ExchangeItem;

@protocol rcNotificationDelegate <NSObject>
@optional
-(void)onRecivNotification:(NSString*)message;
@end

typedef void(^statusBlock)(int);
typedef void(^userBaseInfoBlock)(UserBaseInfo*);
typedef void(^arrayBlock)(NSMutableArray*);
typedef void(^stringBlock)(NSString*);
typedef void(^dictBlock)(NSDictionary*);
typedef void(^pageInfoBlock)(NSMutableArray*, NSDictionary*);
typedef void(^statusStringBlock)(int, NSString*);

@interface HttpService : NSObject
@property(nonatomic) UserBaseInfo *userBaseInfo;
@property(nonatomic) NSString *userName, *userPassword, *userId, *timestamp, *token, *appUrl, *appVersion, *appUpdateDesc;

+(HttpService*)getInstance;
-(void)userRegister:(NSString*)userId psw:(NSString*)password andBlock:(statusBlock)block;
-(void)userLogin:(NSString*)userId psw:(NSString*)password andBlock:(statusBlock)block;
-(void)resetPsw:(NSString*)oldPsw psw:(NSString*)newPsw andBlock:(statusBlock)block;
-(void)modifyPsw:(NSString*)oldPsw psw:(NSString*)newPsw andBlock:(statusBlock)block;
-(void)queryUserBaseInfo:(userBaseInfoBlock)block;
-(void)modifyUserBaseInfo:(UserBaseInfo*)userBaseInfo andBlock:(statusBlock)block;
-(void)sumbitExchange:(NSString*)account item:(ExchangeItem*)item andBlock:(statusBlock)block;
@end