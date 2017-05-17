//
//  loginViewController.m
//  YSTParentClient
//
//  Created by apple on 14-10-8.
//  Copyright (c) 2014年 jason. All rights reserved.
//

#import "loginViewController.h"
#import "basePannelController.h"
#import "utilityFunction.h"
#import "loginView.h"
#import "registerViewController.h"
#import "resetPswViewController.h"
#import "HttpService.h"
#import "MBProgressHUD.h"
#import "UITextField+Shake.h"
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
#import "QumiConfigTool.h"

//指盟
#import "TBSpiritInstrument.h"

//点乐
#import "JOJOWangSDK.h"

@interface loginViewController ()<loginViewDelegate>
@property(nonatomic) loginView *officialLoginView;

- (void)onPressLoginBtn:(NSString*)userName passWd:(NSString*)userPsw;
@end

@implementation loginViewController

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
}

- (void)viewDidAppear:(BOOL)animated
{
    _officialLoginView.userNameField.text = [[NSUserDefaults standardUserDefaults] objectForKey:@"userName"];
    _officialLoginView.pswField.text = [[NSUserDefaults standardUserDefaults] objectForKey:@"userPsw"];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    CGRect clientRect = [UIScreen mainScreen].bounds;
    
    [self.navigationController setNavigationBarHidden:NO animated:YES];
    UIBarButtonItem *backItem = [[UIBarButtonItem alloc] init];
    backItem.title = @"返回";
    self.navigationItem.backBarButtonItem = backItem;
    self.navigationItem.hidesBackButton = YES;
    
    UILabel *titleLable = [[UILabel alloc] initWithFrame:CGRectMake(clientRect.size.width/2-130, 0, 130, 20)];
    titleLable.font = [UIFont boldSystemFontOfSize:20];
    titleLable.textColor = [UIColor whiteColor];
    titleLable.textAlignment = NSTextAlignmentCenter;
    titleLable.text = @"登 录";
    
    self.navigationItem.titleView = titleLable;
    
    _officialLoginView = [[loginView alloc] initWithFrame:clientRect];
    [self.view addSubview:_officialLoginView];
    
    _officialLoginView.delegate = self;
    [_officialLoginView.userNameField setBorderStyle:UITextBorderStyleNone];
    

#if 1
    
    NSString *crashDump = [[NSUserDefaults standardUserDefaults] objectForKey:@"crashDump"];
    
    if (crashDump) {
        UIAlertView *alertV = [[UIAlertView alloc] initWithTitle:@"上次遇到程序崩溃" message:crashDump  delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        [alertV show];
        
        [[NSUserDefaults standardUserDefaults] setObject:nil forKey:@"crashDump"];
    }

#else
    
    NSString *signalDump = [[NSUserDefaults standardUserDefaults] objectForKey:@"signalDump"];
    NSString *exceptionDump = [[NSUserDefaults standardUserDefaults] objectForKey:@"exceptionDump"];
    NSString *crashDump = @"";
    
    if (signalDump) {
        crashDump = [crashDump stringByAppendingFormat:@"singalDump:\n%@\n\n", signalDump];
        [[NSUserDefaults standardUserDefaults] setObject:nil forKey:@"signalDump"];
    }
    
    if (exceptionDump) {
        crashDump = [crashDump stringByAppendingFormat:@"exceptionDump:\n%@\n\n", exceptionDump];
        [[NSUserDefaults standardUserDefaults] setObject:nil forKey:@"exceptionDump"];
    }
    
    if (crashDump.length > 0) {
        UIAlertView *alertV = [[UIAlertView alloc] initWithTitle:@"上次遇到程序崩溃" message:crashDump  delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        [alertV show];
    }
    
#endif
}

- (void)initAdvSdk:(NSString*)userId
{
    //点入
    DR_INIT(@"0000922B200000E1", NO, userId)

    //有米
    [PublicCallConfig PublicCallLaunchWithAppID:@"d85ada3d7e5ac114" PublicCallAppSecret:@"a253a90ae71ca233"];
    [PublicCallConfig PublicCallSetUserID:userId];
    [PublicCallPointsManager PublicCallEnable];

    //万普
    [AppConnect getConnect:@"8f5dd153265afff3ea0ebe985124bab0" pid:@"appstore" userID:userId];
 
    //趣米
    [QumiConfigTool startWithAPPID:@"34494e420c92229d" secretKey:@"7654d5857050202d" appChannel:100055];

    //指盟
    [TBSpiritInstrument launchFactoryd:@"ca749530a17dd0cbe0946e713a0bece1" ukrt:userId];

    //点乐
  //[JOJOWangSDK requestJOJOWangSession:@"3b9e2dd530c3662670fcd3c0d7355202" withUserID:userId];
}

- (void)onPressLoginBtn:(NSString*)userName passWd:(NSString*)userPsw
{

    if (userName.length == 0) {
        
        [_officialLoginView.userNameField shake:10
                                      withDelta:5.0f
                                          speed:0.04f
                                 shakeDirection:ShakeDirectionHorizontal];
        return;
    }
    
    if (userPsw.length == 0) {
        
        [_officialLoginView.pswField shake:10
                                 withDelta:5.0f
                                     speed:0.04f
                            shakeDirection:ShakeDirectionHorizontal];
        return;
    }
    
    
#if 1
    
    _officialLoginView.userNameField.enabled = NO;
    _officialLoginView.pswField.enabled = NO;
    _officialLoginView.confirmBtn.enabled = NO;
    _officialLoginView.delegate = nil;
   
    MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:self.view animated:YES];
    hud.mode = MBProgressHUDModeIndeterminate;
    hud.labelText = @"登录中...";
    
    [[HttpService getInstance] userLogin:userName psw:userPsw andBlock:^(int retValue)
     {
         if (retValue != 200) {
             
             _officialLoginView.userNameField.enabled = YES;
             _officialLoginView.pswField.enabled = YES;
             _officialLoginView.confirmBtn.enabled = YES;
             _officialLoginView.delegate = self;
             
             [MBProgressHUD hideHUDForView:self.view animated:YES];
             
             UIAlertView *alertV = [[UIAlertView alloc] initWithTitle:@"提示" message: [utilityFunction getErrorString:retValue] delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
             [alertV show];
         }
         else
         {
             [self initAdvSdk:[HttpService getInstance].userId];
             
             basePannelController *baseCtrl = [basePannelController new];
             baseCtrl.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
             baseCtrl.modalPresentationStyle = UIModalPresentationCurrentContext;
             [self presentViewController:baseCtrl animated:YES completion:nil];
         }
     }];
    
#else

    basePannelController *baseCtrl = [basePannelController new];
    baseCtrl.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
    baseCtrl.modalPresentationStyle = UIModalPresentationCurrentContext;
    [self presentViewController:baseCtrl animated:YES completion:nil];
    
#endif
}

- (void)onPressRegisterLable
{
    CATransition *transition = [CATransition animation];
    transition.duration = 0.4f;
    transition.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
    transition.type = kCATransitionPush;
    transition.subtype = kCATransitionFromRight;
    transition.delegate = self;
    [self.navigationController.view.layer addAnimation:transition forKey:nil];
    [self.navigationController pushViewController:[registerViewController new] animated:YES];
}

- (void)onPressLostPswLable
{
    UIAlertView *alertV = [[UIAlertView alloc] initWithTitle:@"提示" message:@"重置密码暂未开放" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
    [alertV show];

#if 0
    CATransition *transition = [CATransition animation];
    transition.duration = 0.4f;
    transition.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
    transition.type = kCATransitionPush;
    transition.subtype = kCATransitionFromLeft;
    transition.delegate = self;
    [self.navigationController.view.layer addAnimation:transition forKey:nil];
    [self.navigationController pushViewController:[resetPswViewController new] animated:YES];
#endif
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc
{
    NSLog(@"loginViewController dealloc");
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
