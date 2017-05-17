//
//  taskListViewController.m
//  advPannel
//
//  Created by apple on 15/10/11.
//  Copyright © 2015年 jason. All rights reserved.
//

#import "taskListViewController.h"
#import "tasklistView.h"
#import "HttpService.h"
#import "utilityFunction.h"
#import "InfoTableViewCell.h"
#import "ProtoType.h"

//点入
#import "CompileSubject.h"

//万普
#import "WPLib/AppConnect.h"

//有米
#import "PublicCallConfig.h"
#import "PublicCallList.h"
#import "PublicCallPointsManager.h"

//趣米
#import "QumiOperationApp.h"

//指盟
#import "TBDirectorCommand.h"

//点乐
#import "JOJOWangSDK.h"

//多盟
#import "DMOfferWallManager.h"

//巨掌
#import "AdjuzSDK.h"

@interface taskListViewController ()<UITableViewDelegate, UITableViewDataSource,
onClickUserLogoDelegate, YQLDelegate, QMRecommendAppDelegate,JOJOWangSDKDelegate,DMOfferWallManagerDelegate>
@property(nonatomic) NSMutableArray *functionArray;
@property(nonatomic) taskListView *canvasV;
@property(nonatomic) QumiOperationApp *qumiViewController;
@property(nonatomic) DMOfferWallManager *duoMengViewController;
@property(nonatomic) AdjuzSDK *jzViewController;
@end

@implementation taskListViewController

- (NSMutableArray*)functionArray
{
    if (!_functionArray) {
        _functionArray = [NSMutableArray new];
        
        SubjectInfo *DRSubject = [SubjectInfo new];
        DRSubject.name = @"点入广告";
        DRSubject.desc = @"点入广告系统";
        DRSubject.pic = @"icon_dr";
        
        SubjectInfo *YMSubject = [SubjectInfo new];
        YMSubject.name = @"有米广告";
        YMSubject.desc = @"有米广告系统";
        YMSubject.pic = @"icon_ym";
        
        SubjectInfo *QMSubject = [SubjectInfo new];
        QMSubject.name = @"趣米广告";
        QMSubject.desc = @"趣米广告系统";
        QMSubject.pic = @"icon_qm";
        
        SubjectInfo *WPSubject = [SubjectInfo new];
        WPSubject.name = @"万普广告";
        WPSubject.desc = @"万普广告系统";
        WPSubject.pic = @"icon_wp";
        
        SubjectInfo *ZMSubject = [SubjectInfo new];
        ZMSubject.name = @"指盟广告";
        ZMSubject.desc = @"指盟广告系统";
        ZMSubject.pic = @"icon_zm";
        
        SubjectInfo *DLSubject = [SubjectInfo new];
        DLSubject.name = @"点乐广告";
        DLSubject.desc = @"点乐广告系统";
        DLSubject.pic = @"icon_dl";
        
        SubjectInfo *DMSubject = [SubjectInfo new];
        DMSubject.name = @"多盟广告";
        DMSubject.desc = @"多盟广告系统";
        DMSubject.pic = @"icon_dm";
        
        SubjectInfo *JZSubject = [SubjectInfo new];
        JZSubject.name = @"巨掌广告";
        JZSubject.desc = @"巨掌广告系统";
        JZSubject.pic = @"icon_jz";

        
        [_functionArray addObject:DRSubject];
        [_functionArray addObject:YMSubject];
        [_functionArray addObject:WPSubject];
        [_functionArray addObject:QMSubject];
        [_functionArray addObject:ZMSubject];
        [_functionArray addObject:DMSubject];
        [_functionArray addObject:JZSubject];
    }
    
    return _functionArray;
}

- (void)configureAdv
{
    //趣米
    _qumiViewController = [[QumiOperationApp alloc] initwithPointUserID:[HttpService getInstance].userId] ;
    _qumiViewController.delegate = self;
    _qumiViewController.isHiddenStatusBar = NO;
    [_qumiViewController autoGetPoints:NO];
    
    //多盟
    _duoMengViewController = [[DMOfferWallManager alloc] initWithPublisherID:@"96ZJ3KEgzfuLHwTOko"
                                                                   andUserID:[HttpService getInstance].userId];
    _duoMengViewController.delegate = self;
    
    //如果需要禁用应用内下载，请将此值设置为YES。
    _duoMengViewController.disableStoreKit = NO;
    
    //巨掌
    _jzViewController = [[AdjuzSDK alloc] initAppKey:@"678DC0B7F959817D" andAppUserId:[HttpService getInstance].userId];
}

- (void)viewWillAppear:(BOOL)animated
{
    [[HttpService getInstance] queryUserBaseInfo:^(UserBaseInfo *userBaseInfo) {
        _canvasV.scoreLabel.text = [NSString stringWithFormat:@"%d", [HttpService getInstance].userBaseInfo.score];
    }];
}

- (void)viewDidAppear:(BOOL)animated
{
    NSData *picData = [[NSUserDefaults standardUserDefaults] objectForKey:@"userPortrait"];
    
    if (picData) {
        [_canvasV.userLogo setImage:[UIImage imageWithData:picData] animated:YES];
    }
    else
    {
        [_canvasV.userLogo setImage:[UIImage imageNamed:@"defaultLogo"] animated:YES];
    }
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    UIBarButtonItem *backItem = [[UIBarButtonItem alloc] init];
    backItem.title = @"返回";
    self.navigationItem.backBarButtonItem = backItem;
    self.navigationItem.hidesBackButton = YES;
    [self.navigationController setNavigationBarHidden:YES animated:YES];
    
    _canvasV = [[taskListView alloc] initWithFrame:[UIScreen mainScreen].bounds];
    [self.view addSubview:_canvasV];
    
    _canvasV.delegate = self;
    
    if ([HttpService getInstance].userBaseInfo.nickName && [HttpService getInstance].userBaseInfo.nickName.length > 0) {
        _canvasV.nickNameLabel.text = [HttpService getInstance].userBaseInfo.nickName;
    }
    else
    {
        _canvasV.nickNameLabel.text =  @"暂无昵称";
    }
    
    _canvasV.userNameLabel.text = [HttpService getInstance].userName;
    
    _canvasV.taskListTable.delegate = self;
    _canvasV.taskListTable.dataSource = self;
    [utilityFunction setExtraCellLineHidden:_canvasV.taskListTable];
    
    [self configureAdv];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(updateCity:)
                                                 name:@"updateCity"
                                               object:nil];
}

- (void)onClickLogo
{
    
}

-(void)updateCity:(NSNotification*)notification {
    _canvasV.cityLabel.text = notification.object;
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
    
    switch (indexPath.row) {
            
        case 0:
        {
            DR_SHOW(DR_OFFERWALL, self, self)
        }
            break;
            
        case 1:
        {
            [PublicCallList PublicCallShowOffers:YES PublicCallDidShowBlock:^{
                NSLog(@"积分墙已经展示");
            } PublicDidDismissBlock:^{
                NSLog(@"积分墙退出");
            }];
        }
            break;
            
        case 2:
        {
            [AppConnect showList:self];
        }
            break;

            
        case 3:
        {
            [_qumiViewController presentQmRecommendApp:self];
        }
            break;
            
        case 4:
        {
            [TBDirectorCommand  driversrepeated:self correctViewPosition:YES];
        }
            break;
            
        case 5:
        {
            [_duoMengViewController presentOfferWallWithViewController:self type:eDMOfferWallTypeList];
        }
            break;
            
        case 6:
        {
            [_jzViewController openListOfferWall:self];
        }
            break;
            
        default:
            break;
    }
    
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}


//#################################点入广告###########################################
/*********************/
/*广告创建成功         */
/*********************/
- (void)dianruDidOpen:(UIViewController *)vc{
}

/*********************/
/*加载html失败         */
/*********************/
- (void)dianruDidLoadFail:(UIViewController *)vc {
    if (vc) {
        [vc dismissViewControllerAnimated:YES
                               completion:nil];
    }
}

/*********************/
/*点击关闭广告         */
/*不代表广告从内存中释放 */
/*********************/
- (void)dianruDidClose:(UIViewController *)vc{
    
}

/*********************/
/*从内存中销毁         */
/*********************/
- (void)dianruDidDestroy:(UIViewController *)vc{
    
}

//###################################################################################


//#################################趣米广告###########################################
-(void)QMSuccessToLoaded:(QumiOperationApp *)qumiAdApp
{
    NSLog(@"积分墙加载成功回调");
}
-(void)QMFailToLoaded:(QumiOperationApp *)qumiAdApp withError:(NSError *)error
{
    NSLog(@"加载数据失败回调%@",error);
}
-(void)QMDismiss:(QumiOperationApp *)qumiAdApp
{
    NSLog(@"积分墙关闭");
}
- (void)QMGetPointSuccess:(NSString *)getPointState getPoints:(NSInteger)points{
    NSLog(@"积分状态：%@,%zd",getPointState,points);
    
    //领取成功追加
    [_qumiViewController appendPoints:points];
}
//请求领取积分失败方法的回调
- (void)QMGetPointFailed:(NSError *)error{
    NSLog(@"领取积分失败");
}
//请求检查剩余积分成功后，回调该方法，获得总积分数和返回的积分数。
- (void)QMCheckPointsSuccess:(NSInteger)remainPoints{
    NSLog(@"查剩余积分成功后");
}
//请求检查剩余积分失败后，回调该方法，返回检查积分失败信息
-(void)QMCheckPointsFailWithError:(NSError *)error{
    NSLog(@"查剩余积分失败后");
}

#pragma mark QMRecommendApp ConsumePoints Mehtods
//消费请求成功之后，回调该方法，返回消费情况(消费成功，或者当前的积分不足)，以及当前的总积分数
- (void)QMConsumePointsSuccess:(NSString *)ConsumeState remainPoints:(NSInteger)points
{
    NSLog(@"消费请求成功");
}

//消费请求失败之后，回调该方法，返回失败信息。
- (void)QMConsumePointsFailWithError:(NSError *)error
{
    NSLog(@"消费请求失败");
}

#pragma mark QMRecommendApp AppendPoints Mehtods
//追加积分成功后，回调该方法，返回追加积分的情况(追加积分成功)，以及当前的总积分数
- (void)QMAppendPointsSuccess:(NSString *)appendState remainPoints:(NSInteger)points
{
    NSLog(@"追加积分成功后");
}

//追加积分失败后，回调该方法，返回失败信息
- (void)QMAppendPointsFailWithError:(NSError *)error
{
    NSLog(@"获取积分失败：%@",error);
}

//###################################################################################


//#################################点乐广告###########################################


//###################################################################################

//#################################多盟广告###########################################


//###################################################################################


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
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
