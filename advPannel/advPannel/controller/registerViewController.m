//
//  registerViewController.m
//  YSTParentClient
//
//  Created by apple on 15/3/13.
//  Copyright (c) 2015年 jason. All rights reserved.
//

#import "registerViewController.h"
#import "HttpService.h"
#import "MBProgressHUD.h"
#import "utilityFunction.h"
#import "UITextField+Shake.h"

@interface registerViewController ()<UITextFieldDelegate>
@property(nonatomic) UIImageView *nickNameImgView, *pswImgView1, *pswImgView2;
@property(nonatomic) UITextField *pswField, *rePswField, *nickNameField;
@property(nonatomic) UIView *horizontalMark1, *horizontalMark2;
@property(nonatomic) UIButton *btnConfirm;
@property(nonatomic) BOOL success;

-(void)onBtnRegister;
@end

@implementation registerViewController

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    CGRect frame = [ UIScreen mainScreen ].bounds;
    
    self.view.backgroundColor = [UIColor colorWithRed:0.9647f green:0.9647f blue:0.9647f alpha:1.0f];
    
    UILabel *titleLable = [[UILabel alloc] initWithFrame:CGRectMake(frame.size.width/2-130, 0, 130, 20)];
    titleLable.font = [UIFont boldSystemFontOfSize:20];
    titleLable.textColor = [UIColor whiteColor];
    titleLable.textAlignment = NSTextAlignmentCenter;
    titleLable.text = @"注 册";
    
    self.navigationItem.titleView = titleLable;
    
    UIView *pannel = [[UIView alloc] initWithFrame:CGRectMake(10, 30, CGRectGetWidth(frame)-20, 140)];
    [self.view addSubview:pannel];
    
    pannel.layer.borderWidth = 0.5;
    pannel.layer.borderColor = [UIColor lightGrayColor].CGColor;
    pannel.layer.cornerRadius = 8;
    pannel.backgroundColor = [UIColor whiteColor];

    _nickNameImgView = [[UIImageView alloc] initWithFrame:CGRectMake(15, 10, 25, 25)];
    [pannel addSubview:_nickNameImgView];
    _nickNameImgView.image = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"deng_zhanghu_@2x" ofType:@"png"]];
    
    _pswImgView1 = [[UIImageView alloc] initWithFrame:CGRectMake(15, 55, 25, 25)];
    [pannel addSubview:_pswImgView1];
    _pswImgView1.image = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"mima@2x" ofType:@"png"]];
    
    _pswImgView2 = [[UIImageView alloc] initWithFrame:CGRectMake(15, 100, 25, 25)];
    [pannel addSubview:_pswImgView2];
    _pswImgView2.image = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"mima@2x" ofType:@"png"]];
    
    _nickNameField = [[UITextField alloc] initWithFrame:CGRectMake(70, 10, 180, 30)];
    [pannel addSubview:_nickNameField];
    
    _nickNameField.backgroundColor = [UIColor clearColor];
    _nickNameField.placeholder = @"输入用户名";
    _nickNameField.returnKeyType = UIReturnKeyDone;
    _nickNameField.autocapitalizationType = UITextAutocapitalizationTypeNone;
    _nickNameField.clearButtonMode = UITextFieldViewModeWhileEditing;
    _nickNameField.delegate = self;
    
    _pswField = [[UITextField alloc] initWithFrame:CGRectMake(70, 55, 180, 30)];
    [pannel addSubview:_pswField];
    
    _pswField.backgroundColor = [UIColor clearColor];
    _pswField.placeholder = @"输入密码";
    _pswField.returnKeyType = UIReturnKeyDone;
    _pswField.autocapitalizationType = UITextAutocapitalizationTypeNone;
    _pswField.clearButtonMode = UITextFieldViewModeWhileEditing;
    _pswField.secureTextEntry = YES;
    _pswField.delegate = self;
    
    _horizontalMark1 = [[UIView alloc] initWithFrame:CGRectMake(5, 45, frame.size.width-30, 0.5)];
    [pannel addSubview:_horizontalMark1];
    _horizontalMark1.backgroundColor = [UIColor lightGrayColor];
    
    _rePswField = [[UITextField alloc] initWithFrame:CGRectMake(70, 100, 180, 30)];
    [pannel addSubview:_rePswField];
    
    _rePswField.backgroundColor = [UIColor clearColor];
    _rePswField.placeholder = @"重复输入密码";
    _rePswField.returnKeyType = UIReturnKeyDone;
    _rePswField.autocapitalizationType = UITextAutocapitalizationTypeNone;
    _rePswField.secureTextEntry = YES;
    _rePswField.clearButtonMode = UITextFieldViewModeWhileEditing;
    _rePswField.delegate = self;
    
    _horizontalMark2 = [[UIView alloc] initWithFrame:CGRectMake(5, 90, frame.size.width-30, 0.5)];
    [pannel addSubview:_horizontalMark2];
    _horizontalMark2.backgroundColor = [UIColor lightGrayColor];
    
    _btnConfirm = [UIButton buttonWithType:UIButtonTypeCustom];
    [self.view addSubview:_btnConfirm];
    
    _btnConfirm.frame = CGRectMake(5, 190, frame.size.width-10, 35);
    _btnConfirm.showsTouchWhenHighlighted = YES;
    _btnConfirm.titleLabel.font = [UIFont systemFontOfSize:20];
    _btnConfirm.titleLabel.textColor = [UIColor whiteColor];
    _btnConfirm.backgroundColor = [UIColor colorWithRed:0.2235f green:0.6235f blue:0.8745f alpha:1.0f];
    _btnConfirm.layer.cornerRadius = 17.5;
    
    [_btnConfirm setTitle: @"确  定" forState: UIControlStateNormal];
    [_btnConfirm addTarget:self action:@selector(onBtnRegister) forControlEvents:UIControlEventTouchUpInside];
}

-(void)onBtnRegister
{
    if (_nickNameField.text.length == 0
        || _pswField.text.length == 0
        || _rePswField.text.length == 0) {
        
        if (_nickNameField.text.length == 0)
        {
            [_nickNameField shake:10
                        withDelta:5.0f
                            speed:0.04f
                   shakeDirection:ShakeDirectionHorizontal];
            return;
        }
        
        if (_pswField.text.length == 0)
        {
            [_pswField shake:10
                        withDelta:5.0f
                            speed:0.04f
                   shakeDirection:ShakeDirectionHorizontal];
            return;
        }

        if (_rePswField.text.length == 0)
        {
            [_rePswField shake:10
                        withDelta:5.0f
                            speed:0.04f
                   shakeDirection:ShakeDirectionHorizontal];
            return;
        }
    }
    else if(![_pswField.text isEqualToString:_rePswField.text])
    {
        UIAlertView *alertV = [[UIAlertView alloc] initWithTitle:@"提示" message:@"两次输入密码不相同" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        [alertV show];
    }
    else
    {
        _btnConfirm.enabled = NO;
        
        MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:self.view animated:YES];
        hud.mode = MBProgressHUDModeIndeterminate;
        hud.labelText = @"注册中...";
        
         [[HttpService getInstance] userRegister:_nickNameField.text psw:_pswField.text andBlock:^(int retValue) {
            
            _success = true;
            _btnConfirm.enabled = YES;
            [MBProgressHUD hideHUDForView:self.view animated:YES];
            
            NSString *tipMessage = @"";
            
            if (retValue == 200)
            {
                tipMessage = @"账号已经注册成功";
                
                [[NSUserDefaults standardUserDefaults] setObject:_nickNameField.text forKey:@"userName"];
                [[NSUserDefaults standardUserDefaults] setObject:_pswField.text forKey:@"userPsw"];
            }
            else
            {
                tipMessage = [utilityFunction getErrorString:retValue];
            }
            
            UIAlertView *alertV = [[UIAlertView alloc] initWithTitle:@"提示" message:tipMessage delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
            [alertV show];
        }];
    }
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return YES;
}

-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (_success)
    {
    [self.navigationController popViewControllerAnimated:YES];
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc
{
    NSLog(@"registerViewController dealloc");
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
