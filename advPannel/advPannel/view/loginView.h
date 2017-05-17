//
//  loginView.h
//  YSTParentClient
//
//  Created by apple on 14-10-10.
//  Copyright (c) 2014年 jason. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol loginViewDelegate <NSObject>
- (void)onPressLoginBtn:(NSString*)userNmae passWd:(NSString*)userPsw;
- (void)onPressRegisterLable;
- (void)onPressLostPswLable;
@end

@interface loginView : UIView

@property (nonatomic) UITextField *userNameField;
@property (nonatomic) UITextField *pswField;
@property (nonatomic) UIButton *confirmBtn;
@property (nonatomic, weak) id<loginViewDelegate> delegate;

@end
