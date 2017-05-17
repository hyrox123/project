//
//  reportView.h
//  YSTParentClient
//
//  Created by apple on 15/5/15.
//  Copyright (c) 2015年 jason. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef void(^reportBlock)(NSString*);

@interface reportView : UIView
+(void)showReportView:(UIView*)parent;
+(void)hideReportView:(UIView*)parent;
+(void)setSubmitEvent:(UIView*)parent andBlock:(reportBlock)block;
@end
