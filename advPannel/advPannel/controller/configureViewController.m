//
//  configureViewController.m
//  advPannel
//
//  Created by apple on 15/10/11.
//  Copyright © 2015年 jason. All rights reserved.
//

#import "configureViewController.h"
#import "configureView.h"
#import "HttpService.h"
#import "utilityFunction.h"
#import "changePswViewController.h"
#import "reportView.h"

@interface configureViewController ()<UITableViewDelegate, UITableViewDataSource>

@end

@implementation configureViewController

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
    titleLable.text = @"更 多";
    
    self.navigationItem.titleView = titleLable;
    
    configureView *canvasV = [[configureView alloc] initWithFrame:[UIScreen mainScreen].bounds];
    [self.view addSubview:canvasV];
    
    canvasV.functionListTable.delegate = self;
    canvasV.functionListTable.dataSource = self;
    [utilityFunction setExtraCellLineHidden:canvasV.functionListTable];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return 4;
}

#pragma mark Table Delegate Methods
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 50.0f;
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section;
{
    return 0.1f;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CustomCellIdentifier = @"CustomCellIdentifier";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CustomCellIdentifier];
    
    if (cell == nil) {
        cell = [[UITableViewCell alloc]
                initWithStyle:UITableViewCellStyleDefault
                reuseIdentifier:CustomCellIdentifier];
        cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    }
    
    cell.textLabel.font = [UIFont systemFontOfSize:16];
    
    switch (indexPath.row ) {
            
        case 0:
        {
            cell.imageView.image = [UIImage imageNamed:@"icon_update"];
            cell.textLabel.text = @"检查更新";
        }
            break;
            
        case 1:
        {
            cell.imageView.image = [UIImage imageNamed:@"icon_yhxy"];
            cell.textLabel.text = @"用户协议";
            
        }
            break;
            
        case 2:
        {
            cell.imageView.image = [UIImage imageNamed:@"icon_setup"];
            cell.textLabel.text = @"修改密码";
            
        }
            break;
            
        case 3:
        {
            cell.imageView.image = [UIImage imageNamed:@"icon_us"];
            cell.textLabel.text = @"关于我们";
        }
            break;
            
        default:
            break;
    }

    return cell;
}

//选中Cell响应事件
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    
    switch (indexPath.row) {
            
        case 0:
        {
            [self checkAppUpdate];
        }
            break;
            
        case 1:
            break;
            
        case 2:
        {
            changePswViewController *changePswCtrl = [changePswViewController new];
            changePswCtrl.hidesBottomBarWhenPushed = YES;
            [self.navigationController pushViewController:changePswCtrl animated:YES];
        }
            break;
            
        case 3:
        {
            NSString *currentVersion = [NSString stringWithFormat:@"当前版本:%@",  [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"]];
            UIAlertView *alertV = [[UIAlertView alloc] initWithTitle:@"提示" message:currentVersion delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
            [alertV show];
        }
            break;
            
        default:
            break;
    }
    
    
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}

-(void)checkAppUpdate
{
    NSString *currentVersion = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
    NSString *lastVersion = [HttpService getInstance].appVersion;
    
    if (lastVersion.length == 5) {
        if ([currentVersion compare:[HttpService getInstance].appVersion] == NSOrderedAscending) {
            [reportView showReportView:self.view];
        }
        else
        {
            UIAlertView *alertView = [[UIAlertView alloc]initWithTitle:@"提示" message:@"当前为最新版本!" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil];
            
            [alertView show];
        }
    }
    else
    {
        UIAlertView *alertView = [[UIAlertView alloc]initWithTitle:@"提示" message:@"服务器上没有发现新版本!" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil];
        
        [alertView show];
    }
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
