//
//  tutorialView.h
//  YSTParentClient
//
//  Created by apple on 15/3/26.
//  Copyright (c) 2015年 jason. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol tutorialViewDelegate <NSObject>
-(void)onConfirm;
@end

@interface tutorialView : UIView
@property(nonatomic,weak) id<tutorialViewDelegate> delegate;

-(id)initWithFrame:(CGRect)frame cover:(NSString*)coverImg btnImg:(NSString*)btnImg;
@end
