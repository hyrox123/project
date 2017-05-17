//
//  exchangeViewController.m
//  advPannel
//
//  Created by apple on 15/10/11.
//  Copyright © 2015年 jason. All rights reserved.
//

#import "exchangeViewController.h"
#import "exchangeView.h"
#import "HttpService.h"
#import "utilityFunction.h"
#import "InfoTableViewCell.h"
#import "ProtoType.h"
#import "scoreExchangeViewController.h"

@interface exchangeViewController ()<UITableViewDelegate, UITableViewDataSource>
@property(nonatomic) NSMutableArray *functionArray, *zfbItemsArray, *qqItemsArray, *phoneItemsArray;
@end

@implementation exchangeViewController

- (NSMutableArray*)functionArray
{
    if (!_functionArray) {
        _functionArray = [NSMutableArray new];
        
        SubjectInfo *ZFBSubject = [SubjectInfo new];
        ZFBSubject.name = @"支付宝提现";
        ZFBSubject.desc = @"直接提现,方便安全的选择";
        ZFBSubject.pic = @"icon_zfb";
        
        SubjectInfo *QBSubject = [SubjectInfo new];
        QBSubject.name = @"Q币兑换";
        QBSubject.desc = @"快速充值,万能选择";
        QBSubject.pic = @"icon_qq";
        
        SubjectInfo *HFSubject = [SubjectInfo new];
        HFSubject.name = @"话费充值";
        HFSubject.desc = @"您的话费我买单,省时省力";
        HFSubject.pic = @"icon_hf";
        
        SubjectInfo *LQBSubject = [SubjectInfo new];
        LQBSubject.name = @"零钱币兑换";
        LQBSubject.desc = @"零钱购物,全场免费,创造奇迹";
        LQBSubject.pic = @"icon_lq";
        
        [_functionArray addObject:ZFBSubject];
        [_functionArray addObject:QBSubject];
        [_functionArray addObject:HFSubject];
        [_functionArray addObject:LQBSubject];
    }
    
    return _functionArray;
}

- (NSMutableArray*)zfbItemsArray
{
    if (!_zfbItemsArray) {
        _zfbItemsArray = [NSMutableArray new];
        
        ExchangeItem *zfbItem1 = [ExchangeItem new];
        
        zfbItem1.itemDesc = [NSString stringWithFormat:@"支付宝%.1f元", 1000*(1-[HttpService getInstance].userBaseInfo.exchangeRatio)/[HttpService getInstance].userBaseInfo.exchangeBase];
        zfbItem1.value = @"1000";
        zfbItem1.type = @"1";
        
        ExchangeItem *zfbItem2 = [ExchangeItem new];
        zfbItem2.itemDesc = [NSString stringWithFormat:@"支付宝%.1f元", 3000*(1-[HttpService getInstance].userBaseInfo.exchangeRatio)/[HttpService getInstance].userBaseInfo.exchangeBase];
        zfbItem2.value = @"3000";
        zfbItem2.type = @"1";
        
        ExchangeItem *zfbItem3 = [ExchangeItem new];
        zfbItem3.itemDesc = [NSString stringWithFormat:@"支付宝%.1f元", 5000*(1-[HttpService getInstance].userBaseInfo.exchangeRatio)/[HttpService getInstance].userBaseInfo.exchangeBase];
        zfbItem3.value = @"5000";
        zfbItem3.type = @"1";
        
        ExchangeItem *zfbItem4 = [ExchangeItem new];
        zfbItem4.itemDesc = [NSString stringWithFormat:@"支付宝%.1f元", 10000*(1-[HttpService getInstance].userBaseInfo.exchangeRatio)/[HttpService getInstance].userBaseInfo.exchangeBase];
        zfbItem4.value = @"10000";
        zfbItem4.type = @"1";
        
        [_zfbItemsArray addObject:zfbItem1];
        [_zfbItemsArray addObject:zfbItem2];
        [_zfbItemsArray addObject:zfbItem3];
        [_zfbItemsArray addObject:zfbItem4];
    }
    
    return _zfbItemsArray;
}

- (NSMutableArray*)qqItemsArray
{
    if (!_qqItemsArray) {
        _qqItemsArray = [NSMutableArray new];
        
        ExchangeItem *qqItem1 = [ExchangeItem new];
        qqItem1.itemDesc = @"Q币10个";
        qqItem1.value = [NSString stringWithFormat:@"%.0f", [HttpService getInstance].userBaseInfo.exchangeBase*(1+[HttpService getInstance].userBaseInfo.exchangeRatio)*10];
        qqItem1.type = @"2";
        
        ExchangeItem *qqItem2 = [ExchangeItem new];
        qqItem2.itemDesc = @"Q币30个";
        qqItem2.value = [NSString stringWithFormat:@"%.0f", [HttpService getInstance].userBaseInfo.exchangeBase*(1+[HttpService getInstance].userBaseInfo.exchangeRatio)*30];
        qqItem2.type = @"2";
        
        ExchangeItem *qqItem3 = [ExchangeItem new];
        qqItem3.itemDesc = @"Q币50个";
        qqItem3.value = [NSString stringWithFormat:@"%.0f", [HttpService getInstance].userBaseInfo.exchangeBase*(1+[HttpService getInstance].userBaseInfo.exchangeRatio)*50];
        qqItem3.type = @"2";
        
        ExchangeItem *qqItem4 = [ExchangeItem new];
        qqItem4.itemDesc = @"Q币100个";
        qqItem4.value = [NSString stringWithFormat:@"%.0f", [HttpService getInstance].userBaseInfo.exchangeBase*(1+[HttpService getInstance].userBaseInfo.exchangeRatio)*100];
        qqItem4.type = @"2";
        
        [_qqItemsArray addObject:qqItem1];
        [_qqItemsArray addObject:qqItem2];
        [_qqItemsArray addObject:qqItem3];
        [_qqItemsArray addObject:qqItem4];
    }
    
    return _qqItemsArray;
}

- (NSMutableArray*)phoneItemsArray
{
    if (!_phoneItemsArray) {
        _phoneItemsArray = [NSMutableArray new];
        
        ExchangeItem *phoneItem1 = [ExchangeItem new];
        phoneItem1.itemDesc = @"话费10元";
        phoneItem1.value = [NSString stringWithFormat:@"%.0f", [HttpService getInstance].userBaseInfo.exchangeBase*(1+[HttpService getInstance].userBaseInfo.exchangeRatio)*10];
        phoneItem1.type = @"3";
        
        ExchangeItem *phoneItem2 = [ExchangeItem new];
        phoneItem2.itemDesc = @"话费30元";
        phoneItem2.value = [NSString stringWithFormat:@"%.0f", [HttpService getInstance].userBaseInfo.exchangeBase*(1+[HttpService getInstance].userBaseInfo.exchangeRatio)*30];
        phoneItem2.type = @"3";
        
        ExchangeItem *phoneItem3 = [ExchangeItem new];
        phoneItem3.itemDesc = @"话费50元";
        phoneItem3.value = [NSString stringWithFormat:@"%.0f", [HttpService getInstance].userBaseInfo.exchangeBase*(1+[HttpService getInstance].userBaseInfo.exchangeRatio)*50];
        phoneItem3.type = @"3";
        
        ExchangeItem *phoneItem4 = [ExchangeItem new];
        phoneItem4.itemDesc = @"话费100元";
        phoneItem4.value = [NSString stringWithFormat:@"%.0f", [HttpService getInstance].userBaseInfo.exchangeBase*(1+[HttpService getInstance].userBaseInfo.exchangeRatio)*100];
        phoneItem4.type = @"3";
        
        [_phoneItemsArray addObject:phoneItem1];
        [_phoneItemsArray addObject:phoneItem2];
        [_phoneItemsArray addObject:phoneItem3];
        [_phoneItemsArray addObject:phoneItem4];
    }
    
    return _phoneItemsArray;
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
    titleLable.text = @"兑 换";
    
    self.navigationItem.titleView = titleLable;
    
    exchangeView *canvasV = [[exchangeView alloc] initWithFrame:[UIScreen mainScreen].bounds];
    [self.view addSubview:canvasV];
    
    canvasV.bannerView.image = [UIImage imageNamed:@"squareHeader"];
    
    canvasV.functionListTable.delegate = self;
    canvasV.functionListTable.dataSource = self;
    [utilityFunction setExtraCellLineHidden:canvasV.functionListTable];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return  self.functionArray.count;
}

#pragma mark Table Delegate Methods
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 65.0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CustomCellIdentifier = @"CustomCellIdentifier";
    InfoTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CustomCellIdentifier];
    
    if (cell == nil) {
        cell = [[InfoTableViewCell alloc]
                initWithStyle:UITableViewCellStyleDefault
                reuseIdentifier:CustomCellIdentifier];
    }
    
    SubjectInfo *item = self.functionArray[indexPath.row];
    
    cell.logoView.image = [UIImage imageNamed:item.pic];
    cell.nameLabel.text = item.name;
    cell.decLabel.text = item.desc;
    cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    
    return cell;
}

//选中Cell响应事件
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    if (![HttpService getInstance].userBaseInfo.exchangeRatio) {
        UIAlertView *alertV = [[UIAlertView alloc] initWithTitle:@"提示" message:@"后台没有设置兑换比率,暂时不能兑换" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        [alertV show];
        return;
    }

    scoreExchangeViewController *scoreExchangeCtrl = [scoreExchangeViewController new];
    
    switch (indexPath.row) {
            
        case 0:
        {
            scoreExchangeCtrl.viewTitle = @"支付宝提现";
            scoreExchangeCtrl.dstItemName = @"支付宝账号:";
            scoreExchangeCtrl.dstDesc = @"请输入支付宝账号";
            scoreExchangeCtrl.dstArray = self.zfbItemsArray;
        }
            break;
            
        case 1:
        {
            scoreExchangeCtrl.viewTitle = @"Q币兑换";
            scoreExchangeCtrl.dstItemName = @"QQ账号:";
            scoreExchangeCtrl.dstDesc = @"请输入QQ账号";
            scoreExchangeCtrl.dstArray = self.qqItemsArray;
            
        }
            break;
            
        case 2:
        {
            scoreExchangeCtrl.viewTitle = @"话费充值";
            scoreExchangeCtrl.dstItemName = @"手机号:";
            scoreExchangeCtrl.dstDesc = @"请输入手机号";
            scoreExchangeCtrl.dstArray = self.phoneItemsArray;
            
        }
            break;
            
        default:
        {
            return;
        }
            break;
    }
    
    scoreExchangeCtrl.hidesBottomBarWhenPushed = YES;
    [self.navigationController pushViewController:scoreExchangeCtrl animated:YES];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


//*****点入广告*****//

//

/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */

@end
