//
//  scoreExchangeViewController.h
//  advPannel
//
//  Created by apple on 15/10/19.
//  Copyright © 2015年 jason. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface scoreExchangeViewController : UIViewController
@property(nonatomic) NSString *viewTitle;
@property(nonatomic) NSString *dstItemName, *dstDesc;
@property(nonatomic,weak) NSMutableArray *dstArray;
@end
