//
//  HttpService.m
//  YSTParentClient
//
//  Created by apple on 14-11-11.
//  Copyright (c) 2014年 jason. All rights reserved.
//

#import "HttpService.h"
#import "utilityFunction.h"
#import "ProtoType.h"

static HttpService *instance = nil;

@interface HttpService()
-(NSDictionary*)readJsonContext:(NSData*)data;
@end

@implementation HttpService

+(HttpService*)getInstance
{
    if (!instance) {
        instance = [HttpService new];
    }
    
    return instance;
}

-(UserBaseInfo*)userBaseInfo
{
    if (!_userBaseInfo) {
        _userBaseInfo = [UserBaseInfo new];
    }
    
    return _userBaseInfo;
}

-(NSData*)httpRequest:(NSString*)uri
{
    NSURL* url = [NSURL URLWithString:uri];
    NSURLRequest *request = [[NSURLRequest alloc]initWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:10];
    
    NSError *error = nil;
    NSData* result = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:&error];
    
    if (error != nil) {
        NSLog(@"%@",[error description]);
    }
    else
    {
        NSLog(@"%@", [[NSString alloc] initWithData:result encoding:NSUTF8StringEncoding]);
    }
    
    return result;
}

-(void)userRegister:(NSString*)userId psw:(NSString*)password andBlock:(statusBlock)block
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        NSString *uri = [NSString stringWithFormat:@"http://%@:%d/zc/app_userRegister.do?username=%@&password=%@&idfa=%@", CLU_SERVER_IP, CLU_SERVER_PORT, userId, password, [utilityFunction getIDFA]];
        
        int retValue = -1;
        
        NSMutableURLRequest * request = [[NSMutableURLRequest alloc] init];
        [request setHTTPMethod:@"POST"];
        [request setURL:[NSURL URLWithString:uri]];
        
        
        NSMutableData *body = [NSMutableData data];
        [body appendData:[[NSString stringWithFormat:@"&phoneversion=%@", [[UIDevice currentDevice] systemVersion]] dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:[[NSString stringWithFormat:@"&phonetype=%@", [[UIDevice currentDevice] model]] dataUsingEncoding:NSUTF8StringEncoding]];
        [request setHTTPBody:body];
        
        NSString *postLength = [NSString stringWithFormat:@"%lu", (unsigned long)[body length]];
        [request setValue:postLength forHTTPHeaderField:@"Content-Length"];
        
        NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
        
        if (data != nil) {
            
            NSDictionary *jsonDic = [self readJsonContext:data];
            
            if (jsonDic != nil) {
                NSString *code = [jsonDic objectForKey:@"code"];
                retValue = [code intValue];
            }
        }
        
        dispatch_async(dispatch_get_main_queue(), ^{block(retValue);});
    });
}

-(void)userLogin:(NSString*)userName psw:(NSString*)password andBlock:(statusBlock)block
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        NSString *uri = [NSString stringWithFormat:@"http://%@:%d/zc/app_userLogin.do?username=%@&password=%@&apptype=1", CLU_SERVER_IP, CLU_SERVER_PORT, userName, password];
        
        int retValue = -1;
        NSData *data = [self httpRequest:uri];
        
        if (data != nil) {
            
            NSDictionary *jsonDic = [self readJsonContext:data];
            
            if (jsonDic != nil) {
                
                NSString *code = [jsonDic objectForKey:@"code"];
                
                retValue = [code intValue];
                
                if (retValue == 200) {
                    _userName = userName;
                    _userPassword = password;
                    _userId = [NSString stringWithFormat:@"%d", [[jsonDic objectForKey:@"userid"] intValue]];
                    _token = [jsonDic objectForKey:@"token"];
                    _timestamp = [jsonDic objectForKey:@"timestamp"];
                    _appVersion = [jsonDic objectForKey:@"appversion"];
                    _appUrl = [jsonDic objectForKey:@"appurl"];
                    _appUpdateDesc = [jsonDic objectForKey:@"appdes"];

                    
                    [[NSUserDefaults standardUserDefaults] setObject:userName forKey:@"userName"];
                    [[NSUserDefaults standardUserDefaults] setObject:password forKey:@"userPsw"];
                }
            }
        }
        
        dispatch_async(dispatch_get_main_queue(), ^{block(retValue);});
    });
}

-(void)resetPsw:(NSString*)phone psw:(NSString*)newPsw andBlock:(statusBlock)block;
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        NSString *uri = [NSString stringWithFormat:@"http://%@:%d/jyt/app_findPSW.do?username=%@&password=%@", CLU_SERVER_IP, CLU_SERVER_PORT, phone, newPsw];
        
        int retValue = -1;
        NSData *data = [self httpRequest:uri];
        NSDictionary *jsonDic = [self readJsonContext:data];
        
        if (jsonDic != nil) {
            NSString *code = [jsonDic objectForKey:@"code"];
            retValue = [code intValue];
        }
        
        dispatch_async(dispatch_get_main_queue(), ^{
            block(retValue);
        });
    });
}

-(void)modifyPsw:(NSString*)oldPsw psw:(NSString*)newPsw andBlock:(statusBlock)block;
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        NSString *uri = [NSString stringWithFormat:@"http://%@:%d/zc/app_changePSW.do?userid=%@&token=%@&timstamp=%@&password=%@&newpsw=%@", CLU_SERVER_IP, CLU_SERVER_PORT, _userId, _token, _timestamp, oldPsw, newPsw];
        
        int retValue = -1;
        
        NSData *data = [self httpRequest:uri];
        
        if (data != nil) {
            
            NSDictionary *jsonDic = [self readJsonContext:data];
            
            if (jsonDic != nil) {
                NSString *code = [jsonDic objectForKey:@"code"];
                retValue = [code intValue];
            }
        }
        
        dispatch_async(dispatch_get_main_queue(), ^{
            block(retValue);
        });
    });
}

-(void)queryUserBaseInfo:(userBaseInfoBlock)block;
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        NSString *uri = [NSString stringWithFormat:@"http://%@:%d/zc/app_findUserInfo.do?userid=%@&token=%@", CLU_SERVER_IP, CLU_SERVER_PORT, _userId, _token];
        
        int retValue = -1;
        NSData *data = [self httpRequest:uri];
        NSDictionary *jsonDic = [self readJsonContext:data];
        
        if (jsonDic != nil) {
            NSString *code = [jsonDic objectForKey:@"code"];
            retValue = [code intValue];
        }
        
        if (retValue == 200) {
            self.userBaseInfo.userName = [jsonDic objectForKey:@"username"];
            self.userBaseInfo.nickName = [jsonDic objectForKey:@"nickname"];
            self.userBaseInfo.referee = [jsonDic objectForKey:@"referee"];
            self.userBaseInfo.alipayId = [jsonDic objectForKey:@"alipay"];
            self.userBaseInfo.phone = [jsonDic objectForKey:@"phone"];
            self.userBaseInfo.qq = [jsonDic objectForKey:@"qq"];
            self.userBaseInfo.score = [[jsonDic objectForKey:@"integral"] intValue];
            
            self.userBaseInfo.exchangeRatio = [[jsonDic objectForKey:@"comratio"] intValue]/(float)100;
            self.userBaseInfo.exchangeBase = [[jsonDic objectForKey:@"ratio"] intValue];
        }
        
        dispatch_async(dispatch_get_main_queue(), ^{
            block(self.userBaseInfo);
        });
    });
}

-(void)modifyUserBaseInfo:(UserBaseInfo*)userBaseInfo andBlock:(statusBlock)block
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        NSString *uri = [NSString stringWithFormat:@"http://%@:%d/zc/app_updateUserInfo?userid=%@&username=%@&token=%@", CLU_SERVER_IP, CLU_SERVER_PORT, _userId, _userName, _token];
        
        NSMutableURLRequest * request = [[NSMutableURLRequest alloc] init];
        [request setHTTPMethod:@"POST"];
        [request setURL:[NSURL URLWithString:uri]];
        
        NSMutableData *body = [NSMutableData data];
        
        if (userBaseInfo.nickName != nil && userBaseInfo.nickName.length > 0) {
            [body appendData:[[NSString stringWithFormat:@"&nickname=%@", userBaseInfo.nickName] dataUsingEncoding:NSUTF8StringEncoding]];
        }
        
        if (userBaseInfo.phone != nil && userBaseInfo.phone.length > 0) {
            [body appendData:[[NSString stringWithFormat:@"&phone=%@", userBaseInfo.phone] dataUsingEncoding:NSUTF8StringEncoding]];
        }
        
        if (userBaseInfo.alipayId != nil && userBaseInfo.alipayId.length > 0) {
            [body appendData:[[NSString stringWithFormat:@"&alipay=%@", userBaseInfo.alipayId] dataUsingEncoding:NSUTF8StringEncoding]];
        }
        
        if (userBaseInfo.qq != nil && userBaseInfo.qq.length > 0) {
            [body appendData:[[NSString stringWithFormat:@"&qq=%@", userBaseInfo.qq] dataUsingEncoding:NSUTF8StringEncoding]];
        }
        
        if (userBaseInfo.referee != nil && userBaseInfo.referee.length > 0) {
            [body appendData:[[NSString stringWithFormat:@"&referee=%@", userBaseInfo.referee] dataUsingEncoding:NSUTF8StringEncoding]];
        }
        
        if (userBaseInfo.city != nil && userBaseInfo.city.length > 0) {
            [body appendData:[[NSString stringWithFormat:@"&usercity=%@", userBaseInfo.city] dataUsingEncoding:NSUTF8StringEncoding]];
        }

        [request setHTTPBody:body];
        
        NSString *postLength = [NSString stringWithFormat:@"%lu", (unsigned long)[body length]];
        [request setValue:postLength forHTTPHeaderField:@"Content-Length"];
        
        NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
        
        int retValue = -1;
        
        if (data != nil) {
            
            NSDictionary *jsonDic = [self readJsonContext:data];
            
            if (jsonDic != nil) {
                NSString *code = [jsonDic objectForKey:@"code"];
                retValue = [code intValue];
            }
        }
        
        if (retValue == 200) {
            
            if (userBaseInfo.nickName != nil && userBaseInfo.nickName.length > 0) {
                _userBaseInfo.nickName = userBaseInfo.nickName;
            }
            
            if (userBaseInfo.phone != nil && userBaseInfo.phone.length > 0) {
                _userBaseInfo.phone = userBaseInfo.phone;
            }
            
            if (userBaseInfo.alipayId != nil && userBaseInfo.alipayId.length > 0) {
                _userBaseInfo.alipayId = userBaseInfo.alipayId;
            }
            
            if (userBaseInfo.qq != nil && userBaseInfo.qq.length > 0) {
                _userBaseInfo.qq = userBaseInfo.qq;
            }
            
            if (userBaseInfo.referee != nil && userBaseInfo.referee.length > 0) {
                _userBaseInfo.referee = userBaseInfo.referee;
            }
        }
        
        dispatch_async(dispatch_get_main_queue(), ^{
            block(retValue);
        });
    });
}

-(void)sumbitExchange:(NSString*)account item:(ExchangeItem*)item andBlock:(statusBlock)block
{
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        NSString *uri = [NSString stringWithFormat:@"http://%@:%d/zc/app_pointExchange.do?userid=%@&token=%@", CLU_SERVER_IP, CLU_SERVER_PORT, _userId, _token];
        
        NSMutableURLRequest * request = [[NSMutableURLRequest alloc] init];
        [request setHTTPMethod:@"POST"];
        [request setURL:[NSURL URLWithString:uri]];
        
        NSMutableData *body = [NSMutableData data];
        
        [body appendData:[[NSString stringWithFormat:@"&type=%@", item.type] dataUsingEncoding:NSUTF8StringEncoding]];
        
        [body appendData:[[NSString stringWithFormat:@"&to=%@", account] dataUsingEncoding:NSUTF8StringEncoding]];
        
        [body appendData:[[NSString stringWithFormat:@"&desc=%@", item.itemDesc] dataUsingEncoding:NSUTF8StringEncoding]];
        
        [body appendData:[[NSString stringWithFormat:@"&integral=%@", item.value] dataUsingEncoding:NSUTF8StringEncoding]];
        
        [request setHTTPBody:body];
        
        NSString *postLength = [NSString stringWithFormat:@"%lu", (unsigned long)[body length]];
        [request setValue:postLength forHTTPHeaderField:@"Content-Length"];
        
        NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
        
        int retValue = -1;
        
        if (data != nil) {
            
            NSDictionary *jsonDic = [self readJsonContext:data];
            
            if (jsonDic != nil) {
                NSString *code = [jsonDic objectForKey:@"code"];
                retValue = [code intValue];
            }
        }
                
        dispatch_async(dispatch_get_main_queue(), ^{
            block(retValue);
        });
    });
}

-(NSDictionary*)readJsonContext:(NSData*)data
{
    if (data == nil) {
        return nil;
    }
    
    NSString *jsonString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    NSError *error = nil;
    NSDictionary *jsonDic = [NSJSONSerialization JSONObjectWithData:[jsonString dataUsingEncoding:NSUTF8StringEncoding] options:NSJSONReadingMutableLeaves error:&error];
    
    if (error != nil) {
        NSLog(@"%@",[error description]);
        return nil;
    }
    
    return jsonDic;
}

@end