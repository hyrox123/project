//
//  messageTipView.h
//  YSTParentClient
//
//  Created by apple on 15/3/24.
//  Copyright (c) 2015年 jason. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface messageTipView : UIView
+(void)showTipView:(UIView*)parent style:(int)style tip:(NSString*)messageText;
+(void)removeTipView:(UIView*)parent;
@end
