//
//  reportView.m
//  YSTParentClient
//
//  Created by apple on 15/5/15.
//  Copyright (c) 2015年 jason. All rights reserved.
//

#import "reportView.h"
#import "HttpService.h"

@interface reportView()
@property(nonatomic, weak) reportBlock navtiveBlock;
@end

@implementation reportView

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
        self.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.5f];
        
        CGSize textSize  = {0, 0};
        
        if ([HttpService getInstance].appUpdateDesc != nil && [HttpService getInstance].appUpdateDesc.length > 0) {
            
            NSString *updateDesc = [NSString stringWithFormat:@"更新内容:\n%@", [HttpService getInstance].appUpdateDesc];
            textSize = [updateDesc boundingRectWithSize:CGSizeMake(CGRectGetWidth(frame)-60, MAXFLOAT) options:NSStringDrawingUsesLineFragmentOrigin attributes:@{NSFontAttributeName:[UIFont boldSystemFontOfSize:15]} context:nil].size;
        }
        
        CGRect ClientRect;
        ClientRect.size.width = CGRectGetWidth(frame)-40;
        ClientRect.size.height = 120+textSize.height;
        ClientRect.origin.x = 20;
        ClientRect.origin.y = (frame.size.height-ClientRect.size.height)/2;
        
        UIView *pannel = [[UIView alloc] initWithFrame:ClientRect];
        [self addSubview:pannel];

        pannel.backgroundColor = [UIColor whiteColor];
        pannel.layer.masksToBounds = YES;
        pannel.layer.cornerRadius = 6;
        pannel.layer.borderWidth = 0.5;
        pannel.layer.borderColor = [UIColor lightGrayColor].CGColor;
        
        UILabel *title = [[UILabel alloc] initWithFrame:CGRectMake(0, 10, CGRectGetWidth(frame), 22)];
        [pannel addSubview:title];
        
        title.textAlignment = NSTextAlignmentCenter;
        title.font = [UIFont boldSystemFontOfSize:20];
        title.textColor = [UIColor colorWithRed:0.1765f green:0.5765f blue:0.8627f alpha:1];
        title.text = @"发现新的版本";
        
        UIView *horiz = [[UIView alloc] initWithFrame:CGRectMake(10, 40, CGRectGetWidth(frame)-20, 2)];
        [pannel addSubview:horiz];
        
        horiz.backgroundColor = [UIColor colorWithRed:0.1765f green:0.5765f blue:0.8627f alpha:1];
        
        UILabel *version = [[UILabel alloc] initWithFrame:CGRectMake(10, 50, CGRectGetWidth(frame)-20, 20)];
        [pannel addSubview:version];
        
        version.textAlignment = NSTextAlignmentLeft;
        version.font = [UIFont systemFontOfSize:15];
        version.textColor = [UIColor blackColor];
        version.text = [NSString stringWithFormat:@"最新版本: %@", [HttpService getInstance].appVersion];
        
        if ([HttpService getInstance].appUpdateDesc != nil && [HttpService getInstance].appUpdateDesc.length > 0) {
            
            NSString *updateDesc = [NSString stringWithFormat:@"更新内容:\n%@", [HttpService getInstance].appUpdateDesc];
            CGSize  textSize = [updateDesc boundingRectWithSize:CGSizeMake(CGRectGetWidth(frame)-20, MAXFLOAT) options:NSStringDrawingUsesLineFragmentOrigin attributes:@{NSFontAttributeName:[UIFont boldSystemFontOfSize:15]} context:nil].size;
            
            UILabel *describ = [[UILabel alloc] initWithFrame:CGRectMake(10, 70, CGRectGetWidth(frame)-20, textSize.height)];
            [pannel addSubview:describ];
            
            describ.textAlignment = NSTextAlignmentLeft;
            describ.font = [UIFont systemFontOfSize:15];
            describ.textColor = [UIColor blackColor];
            describ.numberOfLines = 0;
            describ.text = updateDesc;
        }
        
        UIButton *confirmBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        [pannel addSubview:confirmBtn];
        
        confirmBtn.frame = CGRectMake(10, CGRectGetHeight(ClientRect)-40, 100, 30);
        confirmBtn.layer.cornerRadius = 3;
        confirmBtn.layer.borderWidth = 0.5;
        confirmBtn.layer.borderColor = [UIColor blueColor].CGColor;
        confirmBtn.showsTouchWhenHighlighted = YES;
        confirmBtn.titleLabel.font = [UIFont boldSystemFontOfSize:15];
        confirmBtn.backgroundColor = [UIColor colorWithRed:0.1765f green:0.5765f blue:0.8627f alpha:1];
        [confirmBtn setTitle: @"立即更新" forState: UIControlStateNormal];
        [confirmBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
        [confirmBtn setTitleColor:[UIColor orangeColor] forState:UIControlStateHighlighted];
        [confirmBtn addTarget:self action:@selector(onBtnConfirm) forControlEvents:UIControlEventTouchUpInside];
        
        UIButton *cancelBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        [pannel addSubview:cancelBtn];
        
        cancelBtn.frame = CGRectMake(CGRectGetWidth(ClientRect)-110, CGRectGetHeight(ClientRect)-40, 100, 30);
        cancelBtn.layer.cornerRadius = 3;
        cancelBtn.layer.borderWidth = 0.5;
        cancelBtn.layer.borderColor = [UIColor blackColor].CGColor;
        cancelBtn.backgroundColor = [UIColor lightGrayColor];
        cancelBtn.showsTouchWhenHighlighted = YES;
        cancelBtn.titleLabel.font = [UIFont boldSystemFontOfSize:15];
        [cancelBtn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
        [cancelBtn setTitle: @"以后再说" forState: UIControlStateNormal];
        [cancelBtn addTarget:self action:@selector(onCancel) forControlEvents:UIControlEventTouchUpInside];
    }
    
    return self;
}

- (void)onCancel
{
    [self removeFromSuperview];
}

- (void)onBtnConfirm
{
    NSURL *url = [NSURL URLWithString:[HttpService getInstance].appUrl];
    [[UIApplication sharedApplication] openURL:url];
    [self removeFromSuperview];
}


+ (void)showReportView:(UIView*)parent
{
    reportView *tipV = [[reportView alloc] initWithFrame:[UIScreen mainScreen].bounds];
    [parent addSubview:tipV];
}

+ (void)hideReportView:(UIView*)parent
{
    reportView *tipV = nil;
    Class messageTipClass = [reportView class];
    NSEnumerator *subviewsEnum = [parent.subviews reverseObjectEnumerator];
    for (UIView *subview in subviewsEnum) {
        if ([subview isKindOfClass:messageTipClass]) {
            tipV = (reportView *)subview;
            break;
        }
    }
    
    if (tipV) {
        [tipV removeFromSuperview];
    }
}

+(void)setSubmitEvent:(UIView*)parent andBlock:(reportBlock)block
{
    reportView *tipV = nil;
    Class messageTipClass = [reportView class];
    NSEnumerator *subviewsEnum = [parent.subviews reverseObjectEnumerator];
    for (UIView *subview in subviewsEnum) {
        if ([subview isKindOfClass:messageTipClass]) {
            tipV = (reportView *)subview;
            break;
        }
    }
    
    if (tipV) {
        tipV.navtiveBlock = block;
    }
}

@end
