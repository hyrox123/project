//
//  AppDelegate.m
//  advPannel
//
//  Created by apple on 15/10/11.
//  Copyright © 2015年 jason. All rights reserved.
//

#import "AppDelegate.h"
#import "loginViewController.h"
#import "ExceptionHandler.h"

@interface AppDelegate ()
@property(nonatomic) UINavigationController *navCtrl;
@property(atomic) BOOL appRuninBk;
@end

@implementation AppDelegate


void UncaughtExceptionHandler(NSException *exception) {
    
    NSString *crashReason = [NSString stringWithFormat:@"\n**********application occur exception**********\n \
                             exception type : %@ \n crash reason : %@ \n call stack info : %@", [exception name], [exception reason], [exception callStackSymbols]];
    
    [[NSUserDefaults standardUserDefaults] setObject:crashReason forKey:@"crashDump"];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    
#if 1
    NSSetUncaughtExceptionHandler (&UncaughtExceptionHandler);
#else
    [ExceptionHandler installExceptionHandler];
#endif
    
    [[UINavigationBar appearance] setBackgroundImage:[UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"shu@2x" ofType:@"png"]] forBarMetrics:UIBarMetricsDefault];
    [[UINavigationBar appearance] setTintColor:[UIColor whiteColor]];
    
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.rootViewController = [UIViewController new];
    [self.window makeKeyAndVisible];
    
    self.navCtrl = [[UINavigationController alloc] initWithRootViewController:[loginViewController new]];
    self.window.rootViewController = self.navCtrl;
    self.window.layer.contents = nil;
    self.window.backgroundColor = [UIColor whiteColor];
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    
#if 0
    _appRuninBk = YES;
    __block UIBackgroundTaskIdentifier background_task;
    
    //注册一个后台任务，告诉系统我们需要向系统借一些事件
    background_task = [application beginBackgroundTaskWithExpirationHandler:^ {
        
        //不管有没有完成，结束background_task任务
        [application endBackgroundTask: background_task];
        background_task = UIBackgroundTaskInvalid;
    }];
    
    //异步
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        while(_appRuninBk)
        {
            [NSThread sleepForTimeInterval:1]; //wait for 1 sec
        }
        
        [application endBackgroundTask: background_task];
        background_task = UIBackgroundTaskInvalid;
    });
#endif
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
    _appRuninBk = NO;
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
