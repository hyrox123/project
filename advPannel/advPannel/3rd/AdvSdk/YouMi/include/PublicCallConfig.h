//
//  PublicCallConfig.h
//  YouMiSDK
//
//  Created by Layne on 12-5-2.
//  Copyright (c) 2012年 YouMi Mobile Co. Ltd. All rights reserved.
//
//  sdk version 5.3.0


#import <UIKit/UIKit.h>
#import "publicHeader.h"

@interface PublicCallConfig : NSObject
// !!!: set appid And appsecret
//      Go ahead http://www.youmi.net/ ,Registering an app to get appid and appsecret
//
// !!!: 应用ID
//      前往有米主页:http://www.youmi.net/ 注册一个开发者帐户，同时注册一个应用，获取对应应用的ID与秘钥
//
+ (void)PublicCallLaunchWithAppID:(NSString *)PublicCallAppid PublicCallAppSecret:(NSString *)PublicCallAecret;


// !!!:setting window for sdk
//      Setting window for sdk in [AppDelegate application:didFinishLaunchingWithOptions:] by [PublicCallConfig PublicCallSetFullScreenWindow:self.window];
//
// !!!:设置广告显示需要的window [当积分墙不能正常显示出来的时候设置]
//      可以在[AppDelegate application:didFinishLaunchingWithOptions:]里[self.window makeKeyAndVisible];后设置[PublicCallConfig PublicCallSetFullScreenWindow:self.window];
+ (void)PublicCallSetFullScreenWindow:(UIWindow *)PublicCallWindow;

// !!!:setting UserID
//      Using for webSever to identify the users who should get points.
//      for more detail:http://wiki.youmi.net/Youmi_ios_offers_order_callback_protocol_en
// !!!:设置UserID
//      可用于服务器对接获取积分
//      详情请看：http://wiki.youmi.net/%E5%AF%B9%E5%BC%80%E5%8F%91%E8%80%85%E7%9A%84%E7%A7%AF%E5%88%86%E5%8F%8D%E9%A6%88%E6%8E%A5%E5%8F%A3
+ (void)PublicCallSetUserID:(NSString *)PublicCallUserID;
+ (NSString *)PublicCallUserID;
@end
