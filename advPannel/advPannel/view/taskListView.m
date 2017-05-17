//
//  taskListView.m
//  advPannel
//
//  Created by apple on 15/10/11.
//  Copyright © 2015年 jason. All rights reserved.
//

#import "taskListView.h"

@interface taskListView()
-(void)onClick;
@end

@implementation taskListView

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
        
        UIView *pannelBlue = [[UIView alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth(frame), 130)];
        [self addSubview:pannelBlue];
        
        pannelBlue.backgroundColor =  [UIColor colorWithRed:0.2235f green:0.6235f blue:0.8745f alpha:1.0f];

        _userLogo = [[DKCircleButton alloc] initWithFrame:CGRectMake((CGRectGetWidth(frame)-60)/2, 35, 60, 60)];
        [pannelBlue addSubview:_userLogo];
        
        [self.userLogo addTarget:self action:@selector(onClick) forControlEvents:UIControlEventTouchUpInside];
        
        UIButton *guideBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        [pannelBlue addSubview:guideBtn];

        guideBtn.frame = CGRectMake((CGRectGetWidth(frame)-60)/2-80, 55, 35, 35);
        guideBtn.showsTouchWhenHighlighted = YES;
        guideBtn.layer.cornerRadius = 17.5;
        [guideBtn setImage:[UIImage imageNamed:@"icon_gantan"] forState:UIControlStateNormal];
        
   //   [guideBtn addTarget:self action:@selector() forControlEvents:UIControlEventTouchUpInside];

        UIButton *friendBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        [pannelBlue addSubview:friendBtn];
        
        friendBtn.frame = CGRectMake((CGRectGetWidth(frame)-60)/2+110, 55, 35, 35);
        friendBtn.showsTouchWhenHighlighted = YES;
        friendBtn.layer.cornerRadius = 17.5;
        [friendBtn setImage:[UIImage imageNamed:@"icon_minus"] forState:UIControlStateNormal];

        _nickNameLabel = [[UILabel alloc] initWithFrame:CGRectMake((CGRectGetWidth(frame)-100)/2, 100, 100, 20)];
        [pannelBlue addSubview:_nickNameLabel];
        
        _nickNameLabel.font = [UIFont boldSystemFontOfSize:16];
        _nickNameLabel.textColor = [UIColor whiteColor];
        _nickNameLabel.textAlignment = NSTextAlignmentCenter;
        
        _cityLabel = [[UILabel alloc] initWithFrame:CGRectMake(CGRectGetWidth(frame)-150, 20, 140, 20)];
        [pannelBlue addSubview:_cityLabel];
        
        _cityLabel.font = [UIFont boldSystemFontOfSize:12];
        _cityLabel.textColor = [UIColor whiteColor];
        _cityLabel.textAlignment = NSTextAlignmentRight;

        UIView *pannelWhite = [[UIView alloc] initWithFrame:CGRectMake(0, 130, CGRectGetWidth(frame), 50)];
        [self addSubview:pannelWhite];
        
        pannelWhite.backgroundColor =  [UIColor whiteColor];
        pannelWhite.layer.borderWidth = 0.5f;
        pannelWhite.layer.borderColor = [UIColor lightGrayColor].CGColor;
        
        UIView *verticalMark1 = [[UIView alloc] initWithFrame:CGRectMake(CGRectGetWidth(frame)/3, 0, 0.5, 50)];
        [pannelWhite addSubview:verticalMark1];
        verticalMark1.backgroundColor = [UIColor lightGrayColor];
        
        UIView *verticalMark2 = [[UIView alloc] initWithFrame:CGRectMake(CGRectGetWidth(frame)*2/3, 0, 0.5, 50)];
        [pannelWhite addSubview:verticalMark2];
        verticalMark2.backgroundColor = [UIColor lightGrayColor];
        
        int grideW = CGRectGetWidth(frame)/3;
        
        UIImageView *userIcon = [[UIImageView alloc] initWithFrame:CGRectMake(grideW/2-20, 5, 20, 20)];
        [pannelWhite addSubview:userIcon];
        
        userIcon.image = [UIImage imageNamed:@"icon_id"];
        
        UILabel *userDescLabel = [[UILabel alloc] initWithFrame:CGRectMake(grideW/2+5, 5, 100, 20)];
        [pannelWhite addSubview:userDescLabel];
        
        userDescLabel.font = [UIFont systemFontOfSize:15];
        userDescLabel.textColor = [UIColor lightGrayColor];
        userDescLabel.text = @"ID";
        
        _userNameLabel = [[UILabel alloc] initWithFrame:CGRectMake(grideW/2-30, 25, 60, 20)];
        [pannelWhite addSubview:_userNameLabel];
        
        _userNameLabel.font = [UIFont systemFontOfSize:15];
        _userNameLabel.textColor = [UIColor lightGrayColor];
        _userNameLabel.textAlignment = NSTextAlignmentCenter;
       
        UIImageView *userScore = [[UIImageView alloc] initWithFrame:CGRectMake(grideW/2-20+grideW, 5, 20, 20)];
        [pannelWhite addSubview:userScore];
        
        userScore.image = [UIImage imageNamed:@"icon_coin"];
        
        UILabel *scoreDescLabel = [[UILabel alloc] initWithFrame:CGRectMake(grideW/2+grideW+5, 5, 80, 20)];
        [pannelWhite addSubview:scoreDescLabel];
        
        scoreDescLabel.font = [UIFont systemFontOfSize:15];
        scoreDescLabel.textColor = [UIColor lightGrayColor];
        scoreDescLabel.text = @"金币";
        
        _scoreLabel = [[UILabel alloc] initWithFrame:CGRectMake(grideW, 25, grideW, 20)];
        [pannelWhite addSubview:_scoreLabel];
        
        _scoreLabel.font = [UIFont systemFontOfSize:12];
        _scoreLabel.textColor = [UIColor lightGrayColor];
        _scoreLabel.textAlignment = NSTextAlignmentCenter;
        
        UIImageView *userFriend = [[UIImageView alloc] initWithFrame:CGRectMake(grideW/2-25+grideW*2, 5, 20, 20)];
        [pannelWhite addSubview:userFriend];
        
        userFriend.image = [UIImage imageNamed:@"icon_friend"];
        
        UILabel *friendDescLabel = [[UILabel alloc] initWithFrame:CGRectMake(grideW/2+grideW*2, 5, 80, 20)];
        [pannelWhite addSubview:friendDescLabel];
        
        friendDescLabel.font = [UIFont systemFontOfSize:15];
        friendDescLabel.textColor = [UIColor lightGrayColor];
        friendDescLabel.text = @"小伙伴";
        
        UILabel *friendSum = [[UILabel alloc] initWithFrame:CGRectMake(grideW/2+grideW*2, 25, 40, 20)];
        [pannelWhite addSubview:friendSum];
        
        friendSum.font = [UIFont systemFontOfSize:15];
        friendSum.textColor = [UIColor lightGrayColor];
        friendSum.textAlignment = NSTextAlignmentCenter;
        friendSum.text = @"0";
        
        _taskListTable = [[UITableView alloc] initWithFrame:CGRectMake(0, 185, CGRectGetWidth(frame), CGRectGetHeight(frame)-235)];
        _taskListTable.layer.borderWidth = 0.5f;
        _taskListTable.layer.borderColor = [UIColor lightGrayColor].CGColor;
        [self addSubview:_taskListTable];
    }
    
    return self;
}

-(void)onClick
{
    if ([self.delegate respondsToSelector:@selector(onClickLogo)]) {
        [self.delegate onClickLogo];
    }
}


@end
