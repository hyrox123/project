//
//  ProtoType.h
//  YSTParentClient
//
//  Created by apple on 15/3/27.
//  Copyright (c) 2015年 jason. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>


@interface SubjectInfo : NSObject
@property(nonatomic) NSString *name;
@property(nonatomic) NSString *pic;
@property(nonatomic) NSString *desc;
@end

@interface UserBaseInfo : NSObject
@property(nonatomic) NSString *userName;
@property(nonatomic) NSString *nickName;
@property(nonatomic) NSString *referee;
@property(nonatomic) NSString *alipayId;
@property(nonatomic) NSString *phone;
@property(nonatomic) NSString *qq;
@property(nonatomic) NSString *city;
@property(nonatomic) int score, exchangeBase;
@property(nonatomic) float exchangeRatio;
@end

@interface ExchangeItem : NSObject
@property(nonatomic) NSString *itemDesc;
@property(nonatomic) NSString *value;
@property(nonatomic) NSString *type;
@end

@interface ProtoType : NSObject
@end
