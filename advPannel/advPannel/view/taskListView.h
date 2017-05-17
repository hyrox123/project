//
//  taskListView.h
//  advPannel
//
//  Created by apple on 15/10/11.
//  Copyright © 2015年 jason. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DKCircleButton.h"

@protocol onClickUserLogoDelegate <NSObject>
@optional
-(void)onClickLogo;
@end

@interface taskListView : UIView
@property(nonatomic) DKCircleButton *userLogo;
@property(nonatomic) UILabel *userNameLabel, *nickNameLabel, *scoreLabel, *cityLabel;
@property(nonatomic) UITableView *taskListTable;
@property(nonatomic, weak) id<onClickUserLogoDelegate> delegate;
@end
