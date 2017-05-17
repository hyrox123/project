//
//  basePannelController.m
//  YSTParentClient
//
//  Created by apple on 14-10-11.
//  Copyright (c) 2014年 jason. All rights reserved.
//

#import "basePannelController.h"
#import "taskListViewController.h"
#import "exchangeViewController.h"
#import "userInfoViewController.h"
#import "configureViewController.h"
#import "HttpService.h"
#import "ProtoType.h"
#import "reportView.h"
#import <CoreLocation/CoreLocation.h>


@interface basePannelController ()<CLLocationManagerDelegate>
@property(nonatomic) CLLocationManager *locationMgr;
@end

@implementation basePannelController


- (CLLocationManager*)locationMgr
{
    if (!_locationMgr) {
        _locationMgr = [[CLLocationManager alloc] init];
        _locationMgr.delegate = self;
        _locationMgr.desiredAccuracy = kCLLocationAccuracyBest;
        
        if([_locationMgr respondsToSelector:@selector(requestAlwaysAuthorization)]) {
            [_locationMgr requestAlwaysAuthorization];
        }
    }
    
    return _locationMgr;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    taskListViewController *taskCtrl = [taskListViewController new];
    exchangeViewController *exchangeCtrl = [exchangeViewController new];
    userInfoViewController *userInfoCtrl = [userInfoViewController new];
    configureViewController *configureCtrl = [configureViewController new];
    
    UIImage *normalImg = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"Button_zaitao_@2x" ofType:@"png"]];
    UIImage *selectedImg = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"Button_zaitao_2@2x" ofType:@"png"]];
    
    normalImg = [normalImg imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    selectedImg = [selectedImg imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    
    UITabBarItem *taskCtrlItem = [[UITabBarItem alloc] initWithTitle:@"任务" image:normalImg selectedImage:selectedImg];
    
    taskCtrl.tabBarItem = taskCtrlItem;
    taskCtrl.tabBarItem.imageInsets = UIEdgeInsetsMake(5, 0, -5, 0);
    
    normalImg = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"Button_duihuan_@2x" ofType:@"png"]];
    selectedImg = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"Button_duihuan_2@2x" ofType:@"png"]];
    
    normalImg = [normalImg imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    selectedImg = [selectedImg imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    
    UITabBarItem *exchangeCtrlItem = [[UITabBarItem alloc] initWithTitle:@"兑换" image:normalImg selectedImage:selectedImg];
    
    exchangeCtrl.tabBarItem = exchangeCtrlItem;
    exchangeCtrl.tabBarItem.imageInsets = UIEdgeInsetsMake(5, 0, -5, 0);
    
    normalImg = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"Button_user_@2x" ofType:@"png"]];
    selectedImg = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"Button_user_2@2x" ofType:@"png"]];
    
    normalImg = [normalImg imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    selectedImg = [selectedImg imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    
    UITabBarItem *userInfoCtrlItem = [[UITabBarItem alloc] initWithTitle:@"个人中心" image:normalImg selectedImage:selectedImg];
    
    userInfoCtrl.tabBarItem = userInfoCtrlItem;
    userInfoCtrl.tabBarItem.imageInsets = UIEdgeInsetsMake(5, 0, -5, 0);
    
    normalImg = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"Button_more_@2x" ofType:@"png"]];
    selectedImg = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"Button_more_2@2x" ofType:@"png"]];
    
    normalImg = [normalImg imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    selectedImg = [selectedImg imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    
    UITabBarItem *configureCtrlItem = [[UITabBarItem alloc] initWithTitle:@"更多" image:normalImg selectedImage:selectedImg];
    
    configureCtrl.tabBarItem = configureCtrlItem;
    configureCtrl.tabBarItem.imageInsets = UIEdgeInsetsMake(5, 0, -5, 0);
    
    UINavigationController *taskCtrlNav = [[UINavigationController alloc]initWithRootViewController:taskCtrl];
    UINavigationController *exchangeCtrlNav = [[UINavigationController alloc]initWithRootViewController:exchangeCtrl];
    UINavigationController *userInfoCrltNav = [[UINavigationController alloc]initWithRootViewController:userInfoCtrl];
    UINavigationController *configureCtrlNav = [[UINavigationController alloc]initWithRootViewController:configureCtrl];
    
    NSArray *viewControllers = [NSArray arrayWithObjects:taskCtrlNav, exchangeCtrlNav, userInfoCrltNav, configureCtrlNav, nil];
    [self setViewControllers:viewControllers];
    [self.locationMgr startUpdatingLocation];
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1.5 * NSEC_PER_SEC)), dispatch_get_main_queue(),^{
        [self checkAppUpdate];
    });
}

-(void)checkAppUpdate
{
    NSString *currentVersion = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
    NSString *lastVersion = [HttpService getInstance].appVersion;
    
    if (lastVersion && lastVersion.length == 5) {
        if ([currentVersion compare:[HttpService getInstance].appVersion] == NSOrderedAscending) {
            [reportView showReportView:self.view];
        }
    }
}

-(void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations
{
    CLLocation *currentLocation = [locations lastObject];
    
    // 获取当前所在的城市名
    CLGeocoder *geocoder = [[CLGeocoder alloc] init];
    
    //根据经纬度反向地理编译出地址信息
    
    [geocoder reverseGeocodeLocation:currentLocation completionHandler:^(NSArray *array, NSError *error)
     {
         if (array.count > 0)
         {
             CLPlacemark *placemark = [array objectAtIndex:0];
             
             //将获得的所有信息显示到label上
             
             NSLog(@"%@",placemark.name);
             
             //获取城市
             NSString *city = placemark.locality;
             
             if (!city) {
                 //四大直辖市的城市信息无法通过locality获得，只能通过获取省份的方法来获得（如果city为空，则可知为直辖市）
                 city = placemark.administrativeArea;
             }
             
             UserBaseInfo *baseInfo = [UserBaseInfo new];
             baseInfo.city = city;
             
             [[HttpService getInstance] modifyUserBaseInfo:baseInfo andBlock:^(int retValue) {
             }];

             [[NSNotificationCenter defaultCenter]
              postNotificationName:@"updateCity"
              object:city
              userInfo:nil];
             
             [manager stopUpdatingLocation];
         }
         else
         {
             
         }
     }];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc
{
    //    [self removeObserver:self forKeyPath:@"tabBar.hidden"];
    NSLog(@"basePannelController dealloc");
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
