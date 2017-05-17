//
//  userInfoViewController.m
//  advPannel
//
//  Created by apple on 15/10/11.
//  Copyright © 2015年 jason. All rights reserved.
//

#import "userInfoViewController.h"
#import "userInfoView.h"
#import "HttpService.h"
#import "utilityFunction.h"
#import "ProtoType.h"
#import "setupUserInfoViewController.h"

@interface userInfoViewController ()<UITableViewDelegate, UITableViewDataSource>
@property(nonatomic) userInfoView *canvasV;
@end

@implementation userInfoViewController

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [_canvasV.functionListTable reloadData];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    CGRect frame = [ UIScreen mainScreen ].bounds;
    
    UIBarButtonItem *backItem = [[UIBarButtonItem alloc] init];
    backItem.title = @"返回";
    self.navigationItem.backBarButtonItem = backItem;
    
    UILabel *titleLable = [[UILabel alloc] initWithFrame:CGRectMake(frame.size.width/2-130, 0, 130, 20)];
    titleLable.font = [UIFont boldSystemFontOfSize:20];
    titleLable.textColor = [UIColor whiteColor];
    titleLable.textAlignment = NSTextAlignmentCenter;
    titleLable.text = @"个人中心";
    
    self.navigationItem.titleView = titleLable;
    
    _canvasV = [[userInfoView alloc] initWithFrame:[UIScreen mainScreen].bounds];
    [self.view addSubview:_canvasV];
    
    _canvasV.functionListTable.delegate = self;
    _canvasV.functionListTable.dataSource = self;
    [utilityFunction setExtraCellLineHidden:_canvasV.functionListTable];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 2;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    
    if (section == 0)
    {
        return 1;
    }
    else
    {
        return 4;
    }
}

#pragma mark Table Delegate Methods
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    if (indexPath.section == 0)
    {
        return 70;
    }
    else
    {
        return 50;
    }
}


- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section;
{
    return 0.1f;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CustomCellIdentifier = @"CustomCellIdentifier";
    
    UITableViewCell *cell = [[UITableViewCell alloc]
                               initWithStyle:UITableViewCellStyleDefault
                               reuseIdentifier:CustomCellIdentifier];
    cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    
    if ([indexPath section] == 0)
    {
        UIImageView *portraitV = [[UIImageView alloc] initWithFrame:CGRectMake(13, 8, 50, 50)];
        [cell.contentView addSubview:portraitV];
        
        portraitV.layer.masksToBounds = YES;
        portraitV.layer.cornerRadius = 4;
        
        NSData *picData = [[NSUserDefaults standardUserDefaults] objectForKey:@"userPortrait"];
        
        if (picData) {
            portraitV.image = [UIImage imageWithData:picData];
        }
        else
        {
            portraitV.image =[UIImage imageNamed:@"defaultLogo"];
        }
        
        UILabel *desclabel = [[UILabel alloc] initWithFrame:CGRectMake(80, 12, 80, 15)];
        desclabel.font = [UIFont systemFontOfSize:12];
        desclabel.textColor = [UIColor lightGrayColor];
        desclabel.text = @"我的昵称";
        [cell.contentView addSubview:desclabel];
        
        UILabel *nicklabel = [[UILabel alloc] initWithFrame:CGRectMake(135, 10, 150, 18)];
        [cell.contentView addSubview:nicklabel];
        
        nicklabel.font = [UIFont boldSystemFontOfSize:18];
        nicklabel.textColor = [UIColor colorWithRed:0.3529f green:0.7569f blue:0.7490f alpha:1.0f];
                
        if ([HttpService getInstance].userBaseInfo.nickName && [HttpService getInstance].userBaseInfo.nickName.length > 0) {
            nicklabel.text = [HttpService getInstance].userBaseInfo.nickName;
        }
        else
        {
            nicklabel.text = @"暂无昵称";
        }
        
        CGSize nicklabelW = [nicklabel.text sizeWithAttributes:[NSDictionary dictionaryWithObjectsAndKeys:[UIFont boldSystemFontOfSize:18],NSFontAttributeName, nil]];
        
        UILabel *userTypelabel = [[UILabel alloc] initWithFrame:CGRectMake(135+nicklabelW.width, 12, 80, 15)];
        [cell.contentView addSubview:userTypelabel];
        userTypelabel.font = [UIFont systemFontOfSize:12];
        userTypelabel.textColor = [UIColor orangeColor];
        
        UIImageView *scoreV = [[UIImageView alloc] initWithFrame:CGRectMake(80, 34, 20, 20)];
        scoreV.image = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"icon_coin@2x" ofType:@"png"]];
        [cell.contentView addSubview:scoreV];
        
        UILabel *scorelabel = [[UILabel alloc] initWithFrame:CGRectMake(105, 38, 100, 15)];
        scorelabel.font = [UIFont systemFontOfSize:12];
        scorelabel.textColor = [UIColor lightGrayColor];
        scorelabel.text = [NSString stringWithFormat:@"%d", [HttpService getInstance].userBaseInfo.score];
        [cell.contentView addSubview:scorelabel];
    }
    else
    {
        cell.textLabel.font = [UIFont systemFontOfSize:16];
        
        switch (indexPath.row ) {
                
            case 0:
            {
                cell.imageView.image = [UIImage imageNamed:@"icon_gr_coin"];
                cell.textLabel.text = @"我的金币";
            }
                break;
                
            case 1:
            {
                cell.imageView.image = [UIImage imageNamed:@"icon_gr_friends"];
                cell.textLabel.text = @"我的小伙伴";
                
            }
                break;
                
            case 2:
            {
                cell.imageView.image = [UIImage imageNamed:@"icon_gr_message"];
                cell.textLabel.text = @"我的消息";
                
            }
                break;
                
            case 3:
            {
                cell.imageView.image = [UIImage imageNamed:@"icon_gr_lg"];
                cell.textLabel.text = @"我的推广二维码";
            }
                break;
                
            default:
                break;
        }
    }
    
    return cell;
}

//选中Cell响应事件
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    
    if (indexPath.section == 0) {
        setupUserInfoViewController *setupPrivateCtrl = [[setupUserInfoViewController alloc] init];
        setupPrivateCtrl.hidesBottomBarWhenPushed = YES;
        
        [self.navigationController pushViewController:setupPrivateCtrl animated:YES];
    }
    
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */

@end
