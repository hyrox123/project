//
//  AdjuzSDK.h
//  AdjuzSDK
//
//  Created by zz on 15-12-4.
//  Copyright (c) 2014年 gg. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

// SDK管理类
@interface AdjuzSDK : NSObject

/**
 *  @brief  获取管理类实例
 *
 *  @param appKey    appKey
 *  @param appUserId appUserId
 *
 *  @return 管理类实例
 */
- (instancetype)initAppKey:(NSString *)appKey andAppUserId:(NSString *)appUserId;

/**
 *  @brief  显示积分墙
 *
 *  @param viewController 模态出积分墙界面的控制器
 */
- (void)openListOfferWall:(UIViewController *)viewController;

/**
 *  @brief  获得积分数
 *
 *  @param isShow 是否弹出alert显示积分数
 *
 *  @return 积分数
 */
- (NSString *)getSorceAndIsShow:(BOOL)isShow ;

/**
 *  @brief  消费积分
 *
 *  @param sorce 消费积分数
 *
 *  @return 成功:YES <br>失败NO
 */
- (BOOL)spendSorce:(int)sorce;

/**
 *  SDK 版本
 */
- (NSString *)sdkVersion;
@end
