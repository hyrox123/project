//  messageTipView.m
//  YSTParentClient
//
//  Created by apple on 15/3/24.
//  Copyright (c) 2015年 jason. All rights reserved.
//

#import "messageTipView.h"

@interface messageTipView()
@property(nonatomic) UIImageView *tipImageView;
@property(nonatomic) UILabel *tipMessageLable;
@end

@implementation messageTipView

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
        _tipImageView = [[UIImageView alloc] initWithFrame:CGRectMake((frame.size.width-100)/2.0,(frame.size.height-65)/2.0-50-frame.origin.y, 100, 100)];
    
        _tipMessageLable = [[UILabel alloc] initWithFrame:CGRectMake((frame.size.width-100)/2.0, (frame.size.height-65)/2.0+50-frame.origin.y, 100, 20)];
        
        _tipMessageLable.font = [UIFont systemFontOfSize:12];
        _tipMessageLable.textColor = [UIColor grayColor];
        _tipMessageLable.textAlignment = NSTextAlignmentCenter;
        
        [self addSubview:_tipImageView];
        [self addSubview:_tipMessageLable];
    }
    
    return self;
}

+ (void)showTipView:(UIView*)parent style:(int)style tip:(NSString*)messageText
{
    Class messageTipClass = [messageTipView class];
    NSEnumerator *subviewsEnum = [parent.subviews reverseObjectEnumerator];
    for (UIView *subview in subviewsEnum) {
        if ([subview isKindOfClass:messageTipClass]) {
            return;
        }
    }
    
    messageTipView *tipV = [[messageTipView alloc] initWithFrame:parent.frame];
    [parent addSubview:tipV];
    
    tipV.tipMessageLable.text = messageText;
    
    if (style == 0)
    {
        tipV.tipImageView.image = [UIImage imageNamed:@"beijing_.png"];
    }
    else
    {
        tipV.tipImageView.image = [UIImage imageNamed:@"beijing_2.png"];
    }
}

+ (void)removeTipView:(UIView*)parent
{
    messageTipView *tipV = nil;
    Class messageTipClass = [messageTipView class];
    NSEnumerator *subviewsEnum = [parent.subviews reverseObjectEnumerator];
    for (UIView *subview in subviewsEnum) {
        if ([subview isKindOfClass:messageTipClass]) {
            tipV = (messageTipView *)subview;
            break;
        }
    }
    
    if (tipV) {
        [tipV removeFromSuperview];
    }
}

@end
