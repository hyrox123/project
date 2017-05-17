//
//  loginView.m
//  YSTParentClient
//
//  Created by apple on 14-10-10.
//  Copyright (c) 2014年 jason. All rights reserved.
//

#import "loginView.h"
#import "HttpService.h"

@interface loginView()<UITextFieldDelegate>
- (void)onBtnLogin;
- (void)onTapLostPswLable;
@end

@implementation loginView

/*
 // Only override drawRect: if you perform custom drawing.
 // An empty implementation adversely affects performance during animation.
 - (void)drawRect:(CGRect)rect {
 // Drawing code
 }
 */

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    
    if (self)
    {
        self.backgroundColor = [UIColor colorWithRed:0.9647f green:0.9647f blue:0.9647f alpha:1.0f];
        
        UIView *pannel = [[UIView alloc] initWithFrame:CGRectMake(10, 30, CGRectGetWidth(frame)-20, 90)];
        pannel.layer.borderWidth = 0.5;
        pannel.layer.borderColor = [UIColor lightGrayColor].CGColor;
        pannel.layer.cornerRadius = 8;
        pannel.backgroundColor = [UIColor whiteColor];
        
        UIImageView *imgView1 = [[UIImageView alloc] initWithFrame:CGRectMake(10, 10, 25, 25)];
        imgView1.image = [UIImage imageNamed:@"deng_zhanghu_"];
        
        _userNameField = [[UITextField alloc] initWithFrame:CGRectMake(50, 10, 180, 30)];
        _userNameField.backgroundColor = [UIColor clearColor];
        _userNameField.placeholder = @"请输入用户名/手机号";
        _userNameField.returnKeyType = UIReturnKeyDone;
        _userNameField.autocapitalizationType = UITextAutocapitalizationTypeNone;
        _userNameField.clearButtonMode = UITextFieldViewModeWhileEditing;
        _userNameField.delegate = self;
        
        UIView *horizontalMark1 = [[UIView alloc] initWithFrame:CGRectMake(5, 45, CGRectGetWidth(frame)-30, 0.5)];
        horizontalMark1.backgroundColor = [UIColor lightGrayColor];
        
        UIImageView *imgView2 = [[UIImageView alloc] initWithFrame:CGRectMake(10, 55, 25, 25)];
        imgView2.image = [UIImage imageNamed:@"mima.png"];
        
        _pswField = [[UITextField alloc] initWithFrame:CGRectMake(50, 55, 180, 30)];
        _pswField.backgroundColor = [UIColor clearColor];
        _pswField.placeholder = @"请输入密码";
        _pswField.returnKeyType = UIReturnKeyDone;
        _pswField.autocapitalizationType = UITextAutocapitalizationTypeNone;
        _pswField.secureTextEntry = YES;
        _pswField.clearButtonMode = UITextFieldViewModeWhileEditing;
        _pswField.delegate = self;
        
        [pannel addSubview:imgView1];
        [pannel addSubview:_userNameField];
        [pannel addSubview:imgView2];
        [pannel addSubview:horizontalMark1];
        [pannel addSubview:_pswField];
        
        _confirmBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        _confirmBtn.frame = CGRectMake(5, 140, frame.size.width-10, 35);
        _confirmBtn.layer.cornerRadius = 17.5;
        _confirmBtn.showsTouchWhenHighlighted = YES;
        _confirmBtn.titleLabel.font = [UIFont systemFontOfSize:20];
        _confirmBtn.titleLabel.textColor = [UIColor whiteColor];
        _confirmBtn.backgroundColor = [UIColor colorWithRed:0.2235f green:0.6235f blue:0.8745f alpha:1.0f];
        [_confirmBtn setTitle: @"登   录" forState: UIControlStateNormal];
        [_confirmBtn addTarget:self action:@selector(onBtnLogin) forControlEvents:UIControlEventTouchUpInside];
        
        UILabel *registerLable = [[UILabel alloc] initWithFrame:CGRectMake(CGRectGetWidth(frame)/2-70, 185, 70, 20)];
        
        registerLable.font = [UIFont systemFontOfSize:14];
        registerLable.textColor = [UIColor lightGrayColor];
        registerLable.text = @"注册账号";
        registerLable.userInteractionEnabled = YES;
        
        UITapGestureRecognizer *singleTapGesture2 = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onTapRegisterLable)];
        [registerLable addGestureRecognizer:singleTapGesture2];

        UILabel *segLable = [[UILabel alloc] initWithFrame:CGRectMake(CGRectGetWidth(frame)/2, 185, 10, 20)];
        segLable.textColor = [UIColor lightGrayColor];
        segLable.text = @"|";
        
        UILabel *lostPswLable = [[UILabel alloc] initWithFrame:CGRectMake(CGRectGetWidth(frame)/2+20, 185, 70, 20)];
        
        lostPswLable.font = [UIFont systemFontOfSize:14];
        lostPswLable.textColor = [UIColor lightGrayColor];
        lostPswLable.text = @"忘记密码?";
        lostPswLable.userInteractionEnabled = YES;
        
        UITapGestureRecognizer *singleTapGesture1 = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onTapLostPswLable)];
        [lostPswLable addGestureRecognizer:singleTapGesture1];
        
        [self addSubview:pannel];
        [self addSubview:_confirmBtn];
        [self addSubview:registerLable];
        [self addSubview:segLable];
        [self addSubview:lostPswLable];
        
        registerLable.hidden = YES;
        segLable.hidden = YES;
        lostPswLable.hidden = YES;
    }
    
    return self;
}

- (void)onBtnLogin
{
    if ([_delegate respondsToSelector:@selector(onPressLoginBtn:passWd:)] == YES)
    {
        [_delegate onPressLoginBtn:_userNameField.text passWd:self.pswField.text];
    }
}

- (void)onTapLostPswLable
{
    if ([_delegate respondsToSelector:@selector(onPressLostPswLable)] == YES)
    {
        [_delegate onPressLostPswLable];
    }
}

- (void)onTapRegisterLable
{
    if ([_delegate respondsToSelector:@selector(onPressRegisterLable)] == YES)
    {
        [_delegate onPressRegisterLable];
    }
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return YES;
}

@end
