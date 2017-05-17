//
//  configureView.m
//  advPannel
//
//  Created by apple on 15/10/15.
//  Copyright © 2015年 jason. All rights reserved.
//

#import "configureView.h"

@implementation configureView

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
        _functionListTable = [[UITableView alloc] initWithFrame:CGRectMake(0, -10, CGRectGetWidth(frame), CGRectGetHeight(frame)-60) style:UITableViewStyleGrouped];
        _functionListTable.layer.borderWidth = 0.5f;
        _functionListTable.layer.borderColor = [UIColor lightGrayColor].CGColor;
        _functionListTable.backgroundColor = [UIColor colorWithRed:0.9647f green:0.9647f blue:0.9647f alpha:1.0f];
        [self addSubview:_functionListTable];
    }
    
    return self;
}


@end
