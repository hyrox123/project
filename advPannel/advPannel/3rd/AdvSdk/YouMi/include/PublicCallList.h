//
//  PublicCallList.h
//  YouMiSDK
//
//  Created by Layne on 12-01-05.
//  Copyright (c) 2012年 YouMi Mobile Co. Ltd. All rights reserved.
//


#import <Foundation/Foundation.h>
#import "PublicCallConfig.h"
#import "PublicCallListAppModel.h"
#import "PublicCallPointsManager.h"
#import "publicHeader.h"

@interface PublicCallList : NSObject

// !!!: show offer wall
//      param rewarded:  you should set YES
//      param didShowBlockIn: callback when offer wall show
//      param didDismissBlockIn: callback when offer wall close
//
// !!!: 显示积分墙
//      param rewarded: 是不是有积分模式
//      param didShowBlockIn: 成功显示积分墙时执行的block
//      param didDismissBlockIn: 关闭积分墙后执行的block
//      returns 是否显示积分墙成功，积分墙显示不成功并不会调用didShowBlockIn和didDismissBlockIn
//
+ (BOOL)PublicCallShowOffers:(BOOL)PublicRewarded PublicCallDidShowBlock:(void (^)(void))PublicCallDidShowBlock PublicDidDismissBlock:(void (^)(void))PublicDidDismissBlockIn;


#pragma mark - 如果用户想自定义界面，会用到下面的接口.developer will call follow functions who want to self define offerwall UI
// !!!: get app list
//      param rewarded: you should set YES
//      param page: request for whick page
//      param count: number of page for each page
//      param recievedBlock: callback function, theapps is PublicCallListAppModel array.
//
// !!!: 获取积分墙的App广告列表【源数据的接口】
//      param rewarded: 是不是有积分模式
//      param page: 请求的数据第几页
//      param count: 每页有多少个广告（比如设置的requestPage为1，adCountPage为3.那么数据就可以看成是每页3个应用的形式在服务器上，每次请求服务器就把对应页的3个应用返回）
//      param recievedBlock: 接收广告列表的block, 其中NSArray的内容是 @[PublicCallListAppModel, PublicCallListAppModel...]; 获取列表失败, 在NSError里有记录.
//
+ (void)PublicRequestOffersOpenData:(BOOL)PublicCallRewarded PublicCallPage:(NSInteger)PublicCallRequestPage PublicCallCount:(NSInteger)PublicCallAdCountPage PublicCallRevievedBlock:(void (^)(NSArray*theApps, NSError *error))PublicCallRecievedBlock;


// !!!: skip to install app when user clip them
//      param PublicCallApp: PublicCallApp is one of element from theApps.
//
// !!!: 安装积分墙中的APP【源数据的接口】
//      param PublicCallApp: 通过requestOffersOpenData获得的APP
//
+ (void)PublicCallUserInstallApp:(PublicCallListAppModel *)PublicCallApp;




#pragma mark - 跟复杂的自定义界面（更多任务类型）。the complexer self defin(more tasks)

// !!!:get app list
//      param rewarded: you should set YES
//      param page: request for whick page
//      param count: number of page for each page
//      param recievedBlock: callback function, theNormalApps is PublicCallListAppModel array,also moreTaskApp is PublicCallListAppModel
//
// !!!:获取积分墙的App广告列表-----有提审任务跟追加任务【源数据的接口】
//      param rewarded: 是不是有积分模式
//      param page: 请求的数据第几页
//      param count: 每页有多少个广告（比如设置的requestPage为1，adCountPage为3.那么数据就可以看成是每页3个应用的形式在服务器上，每次请求服务器就把对应页的3个应用返回）
//      param recievedBlock: 接收广告列表的block, 其中theNormalApps的内容是 @[PublicCallListAppModel, PublicCallListAppModel...]; 获取列表失败, 在NSError里有记录，moreTaskApp是追加任务跟theNormalApps是同类型的内容。theNormalApps中的元素PublicCallListAppModel的PublicCallIsNeedReview为YES的时候是需要提审的，即用户完成了任务步骤后需要回来调用下面的PublicCallReview接口。调用下面的接口PublicCallBeginMoreTask来开始深度任务
//
+ (void)PublicRequestOffersOpenDataMoreAD:(BOOL)PublicCallRewarded PublicCallPage:(NSInteger)PublicCallRequestPage PublicCallCount:(NSInteger)PublicCallAdCountPage PublicCallRevievedBlock:(void (^)(NSArray *theNormalApps,NSArray *moreTaskApp, NSError *error))PublicCallRecievedBlock;


// !!!:提审【源数据的接口】
/* 
 检查PublicCallListAppModel的PublicCallIsNeedReview成员变量的值来判断是否需要提审
 status:0  提审成功，等待审核给分
 status:402 还没开始任务
 status:404 还没完成
*/
+ (void)PublicCallReview:(PublicCallListAppModel *)PublicCallApp PublicCallRecievedBlock:(void(^)(int status))callBack;

// !!!:开始追加任务【源数据的接口】
/*
 isSuccess:1  成功开始进行深度任务
 isSuccess:0  应用没安装，不能进行深度任务
 */
+ (void)PublicCallBeginMoreTask:(PublicCallListAppModel *)PublicCallApp PublicCallRecievedBlock:(void(^)(BOOL isSuccess))callBack;

/*
 *关闭积分墙
 */
+ (void)PublicCallClose;
@end


