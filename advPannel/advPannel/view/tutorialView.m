//
//  tutorialView.m
//  YSTParentClient
//
//  Created by apple on 15/3/26.
//  Copyright (c) 2015年 jason. All rights reserved.
//

#import "tutorialView.h"


@interface tutorialView()
@end

@implementation tutorialView

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

- (id)initWithFrame:(CGRect)frame cover:(NSString*)coverImg btnImg:(NSString*)btnImg
{
    self = [super initWithFrame:frame];
    
    if (self)
    {
        self.backgroundColor = [UIColor colorWithWhite:1.0 alpha:0.01];
        UIImageView *imageV = [[UIImageView alloc] initWithFrame:frame];
        imageV.image = [UIImage imageNamed:coverImg];
        [self addSubview:imageV];
        
        UIButton *btnConfirm = [UIButton buttonWithType:UIButtonTypeCustom];
        btnConfirm.frame = CGRectMake((frame.size.width-120)/2, frame.size.height-110, 120, 35);
        btnConfirm.showsTouchWhenHighlighted = YES;
        [btnConfirm setImage:[UIImage imageNamed:btnImg] forState:UIControlStateNormal];
        [btnConfirm addTarget:self action:@selector(onBtnConfirm) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:btnConfirm];
    }
    
    return self;
}

- (void)onBtnConfirm
{
    if ([self.delegate respondsToSelector:@selector(onConfirm)]) {
        [self.delegate onConfirm];
    }
}

@end
