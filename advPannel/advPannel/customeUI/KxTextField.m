//
//  KxTextField.m
//  YSTParentClient
//
//  Created by apple on 15/6/10.
//  Copyright (c) 2015年 jason. All rights reserved.
//

#import "KxTextField.h"

@implementation KxTextField

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

- (CGRect)textRectForBounds:(CGRect)bounds {
    return CGRectInset( bounds , 10 , 0 );
}

- (CGRect)editingRectForBounds:(CGRect)bounds {
    return CGRectInset( bounds , 10 , 0 );
}

@end
