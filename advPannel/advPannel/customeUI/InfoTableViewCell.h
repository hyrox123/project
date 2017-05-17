//
//  InfoTableViewCell.h
//  YSTParentClient
//
//  Created by apple on 14-10-13.
//  Copyright (c) 2014年 jason. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface InfoTableViewCell : UITableViewCell

@property(nonatomic) UIImageView *logoView, *notifyView;
@property(nonatomic) UILabel *nameLabel;
@property(nonatomic) UILabel *decLabel;
@property(nonatomic) UILabel *updateTimeLabel;

@end
