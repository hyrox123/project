//
//  exchangeView.m
//  advPannel
//
//  Created by apple on 15/10/15.
//  Copyright © 2015年 jason. All rights reserved.
//

#import "exchangeView.h"

@implementation exchangeView

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
        
        _bannerView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth(frame), 100)];
        [self addSubview: _bannerView];
        
        _functionListTable = [[UITableView alloc] initWithFrame:CGRectMake(0, 105, CGRectGetWidth(frame), 260)];
        _functionListTable.layer.borderWidth = 0.5f;
        _functionListTable.layer.borderColor = [UIColor lightGrayColor].CGColor;
        [self addSubview:_functionListTable];
    }
    
    return self;
}


@end
