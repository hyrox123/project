//
//  InfoTableViewCell.m
//  YSTParentClient
//
//  Created by apple on 14-10-13.
//  Copyright (c) 2014年 jason. All rights reserved.
//

#import "InfoTableViewCell.h"


@implementation InfoTableViewCell


- (void)awakeFromNib {
    // Initialization code
}

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    
    if (self) {
        
        self.backgroundColor = [UIColor whiteColor];
        CGRect clientRect = [ UIScreen mainScreen ].bounds;
        
        // Initialization code
        self.logoView = [[UIImageView alloc] initWithFrame:CGRectMake(12, 12, 42, 42)];
        self.notifyView = [[UIImageView alloc] initWithFrame:CGRectMake(58, 8, 8, 8)];
        self.nameLabel = [[UILabel alloc] initWithFrame:CGRectMake(75, 15, clientRect.size.width-180, 16)];
        self.decLabel = [[UILabel alloc] initWithFrame:CGRectMake(75, 40, clientRect.size.width-100, 16)];
        self.updateTimeLabel = [[UILabel alloc] initWithFrame:CGRectMake(clientRect.size.width-90, 15, 80, 11)];
        
        [self.nameLabel setFont:[UIFont systemFontOfSize: 16.0]];
        [self.decLabel setFont:[UIFont systemFontOfSize: 13.0]];
        [self.decLabel setTextColor:[UIColor lightGrayColor]];
        [self.updateTimeLabel setFont:[UIFont systemFontOfSize: 14.0]];
        [self.updateTimeLabel setTextColor:[UIColor lightGrayColor]];
        
        self.nameLabel.highlightedTextColor = [UIColor blueColor];
        self.decLabel.highlightedTextColor = [UIColor blueColor];
        self.updateTimeLabel.highlightedTextColor = [UIColor blueColor];
        
        [self.contentView addSubview:self.logoView];
        [self.contentView addSubview:self.notifyView];
        [self.contentView addSubview:self.nameLabel];
        [self.contentView addSubview:self.decLabel];
        [self.contentView addSubview:self.updateTimeLabel];
    }
    
    return self;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];
    
    // Configure the view for the selected state
}

@end
