//
//  resetPswViewController.m
//  YSTParentClient
//
//  Created by apple on 15/3/13.
//  Copyright (c) 2015年 jason. All rights reserved.
//

#import "resetPswViewController.h"
#import "HttpService.h"
#import "MBProgressHUD.h"
#import "utilityFunction.h"
#import "UITextField+Shake.h"

@interface resetPswViewController ()<UITextFieldDelegate>
@property(nonatomic) UIImageView *progressImgView, *phoneImgView, *smsImgView, *pswImgView1, *pswImgView2;
@property(nonatomic) UITextField *phoneField, *smsCodeField, *pswField,  *rePswField;
@property(nonatomic) UIView *horizontalMark1, *horizontalMark2, *horizontalMark3;
@property(nonatomic) UIButton *btnSMS, *btnNext, *btnConfirm;
@property(nonatomic) int registerStep;

-(void)onBtnSMS;
-(void)onBtnNext;
-(void)onBtnRestPsw;
-(void)refreshLayout:(int)style;
@end

@implementation resetPswViewController

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
    titleLable.text = @"找回密码";
    
    self.navigationItem.titleView = titleLable;
    
    _progressImgView = [[UIImageView alloc] initWithFrame:CGRectMake(5, 10, frame.size.width-10, 40)];
    [self.view addSubview:_progressImgView];
    
    _horizontalMark1 = [[UIView alloc] initWithFrame:CGRectMake(5, 60, frame.size.width-10, 0.5)];
    [self.view addSubview:_horizontalMark1];
    _horizontalMark1.backgroundColor = [UIColor lightGrayColor];
    
    _phoneImgView = [[UIImageView alloc] initWithFrame:CGRectMake(15, 72, 30, 30)];
    [self.view addSubview:_phoneImgView];
    _phoneImgView.image = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"shoujihao@2x" ofType:@"png"]];
    
    _smsImgView = [[UIImageView alloc] initWithFrame:CGRectMake(15, 72, 30, 30)];
    [self.view addSubview:_smsImgView];
    _smsImgView.image = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"yanzhengma@2x" ofType:@"png"]];
    
    _pswImgView1 = [[UIImageView alloc] initWithFrame:CGRectMake(15, 72, 30, 30)];
    [self.view addSubview:_pswImgView1];
    _pswImgView1.image = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"mima@2x" ofType:@"png"]];
    
    _pswImgView2 = [[UIImageView alloc] initWithFrame:CGRectMake(15, 127, 30, 30)];
    [self.view addSubview:_pswImgView2];
    _pswImgView2.image = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"mima@2x" ofType:@"png"]];
    
    _phoneField = [[UITextField alloc] initWithFrame:CGRectMake(70, 75, 180, 30)];
    [self.view addSubview:_phoneField];
    
    _phoneField.backgroundColor = [UIColor clearColor];
    _phoneField.placeholder = @"输入手机号";
    _phoneField.returnKeyType = UIReturnKeyDone;
    _phoneField.autocapitalizationType = UITextAutocapitalizationTypeNone;
    _phoneField.clearButtonMode = UITextFieldViewModeWhileEditing;
    _phoneField.delegate = self;
    
    _smsCodeField = [[UITextField alloc] initWithFrame:CGRectMake(70, 75, 180, 30)];
    [self.view addSubview:_smsCodeField];
    
    _smsCodeField.backgroundColor = [UIColor clearColor];
    _smsCodeField.placeholder = @"填写验证码";
    _smsCodeField.returnKeyType = UIReturnKeyDone;
    _smsCodeField.autocapitalizationType = UITextAutocapitalizationTypeNone;
    _smsCodeField.clearButtonMode = UITextFieldViewModeWhileEditing;
    _smsCodeField.delegate = self;
    
    _pswField = [[UITextField alloc] initWithFrame:CGRectMake(70, 75, 180, 30)];
    [self.view addSubview:_pswField];
    
    _pswField.backgroundColor = [UIColor clearColor];
    _pswField.placeholder = @"输入密码";
    _pswField.returnKeyType = UIReturnKeyDone;
    _pswField.autocapitalizationType = UITextAutocapitalizationTypeNone;
    _pswField.clearButtonMode = UITextFieldViewModeWhileEditing;
    _pswField.secureTextEntry = YES;
    _pswField.delegate = self;
    
    _horizontalMark2 = [[UIView alloc] initWithFrame:CGRectMake(5, 115, frame.size.width-10, 0.5)];
    [self.view addSubview:_horizontalMark2];
    _horizontalMark2.backgroundColor = [UIColor lightGrayColor];
    
    _rePswField = [[UITextField alloc] initWithFrame:CGRectMake(70, 130, 180, 30)];
    [self.view addSubview:_rePswField];
    
    _rePswField.backgroundColor = [UIColor clearColor];
    _rePswField.placeholder = @"重复输入密码";
    _rePswField.returnKeyType = UIReturnKeyDone;
    _rePswField.autocapitalizationType = UITextAutocapitalizationTypeNone;
    _rePswField.secureTextEntry = YES;
    _rePswField.clearButtonMode = UITextFieldViewModeWhileEditing;
    _rePswField.delegate = self;
    
    _horizontalMark3 = [[UIView alloc] initWithFrame:CGRectMake(5, 170, frame.size.width-10, 0.5)];
    [self.view addSubview:_horizontalMark3];
    _horizontalMark3.backgroundColor = [UIColor lightGrayColor];
    
    _btnSMS = [UIButton buttonWithType:UIButtonTypeCustom];
    [self.view addSubview:_btnSMS];
    
    _btnSMS.frame = CGRectMake(5, 145, frame.size.width-10, 35);
    _btnSMS.showsTouchWhenHighlighted = YES;
    _btnSMS.titleLabel.font = [UIFont systemFontOfSize:20];
    _btnSMS.titleLabel.textColor = [UIColor whiteColor];
    _btnSMS.backgroundColor = [UIColor colorWithRed:0.2235f green:0.6235f blue:0.8745f alpha:1.0f];
    _btnSMS.layer.cornerRadius = 17.5;
    
    [_btnSMS setTitle: @"获取验证码" forState: UIControlStateNormal];
    [_btnSMS addTarget:self action:@selector(onBtnSMS) forControlEvents:UIControlEventTouchUpInside];
    
    _btnNext = [UIButton buttonWithType:UIButtonTypeCustom];
    [self.view addSubview:_btnNext];
    
    _btnNext.frame = CGRectMake(5, 145, frame.size.width-10, 35);
    _btnNext.showsTouchWhenHighlighted = YES;
    _btnNext.titleLabel.font = [UIFont systemFontOfSize:20];
    _btnNext.titleLabel.textColor = [UIColor whiteColor];
    _btnNext.backgroundColor = [UIColor colorWithRed:0.2235f green:0.6235f blue:0.8745f alpha:1.0f];
    _btnNext.layer.cornerRadius = 17.5;
    
    [_btnNext setTitle: @"下一步" forState: UIControlStateNormal];
    [_btnNext addTarget:self action:@selector(onBtnNext) forControlEvents:UIControlEventTouchUpInside];
    
    _btnConfirm = [UIButton buttonWithType:UIButtonTypeCustom];
    [self.view addSubview:_btnConfirm];
    
    _btnConfirm.frame = CGRectMake(5, 200, frame.size.width-10, 35);
    _btnConfirm.showsTouchWhenHighlighted = YES;
    _btnConfirm.layer.cornerRadius = 17.5;
    _btnConfirm.titleLabel.font = [UIFont systemFontOfSize:20];
    _btnConfirm.titleLabel.textColor = [UIColor whiteColor];
    _btnConfirm.backgroundColor = [UIColor colorWithRed:0.2235f green:0.6235f blue:0.8745f alpha:1.0f];
    _btnConfirm.layer.cornerRadius = 17.5;
    
    [_btnConfirm setTitle: @"确  定" forState: UIControlStateNormal];
    [_btnConfirm addTarget:self action:@selector(onBtnRestPsw) forControlEvents:UIControlEventTouchUpInside];
    
    _registerStep = 0;
    [self refreshLayout:_registerStep];
}

-(void)onBtnSMS
{
    if ([_phoneField.text length] != 11)
    {
        [_phoneField shake:10
                 withDelta:5.0f
                     speed:0.04f
            shakeDirection:ShakeDirectionHorizontal];
    }
    else
    {
        _btnSMS.enabled = NO;
        
        MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:self.view animated:YES];
        hud.mode = MBProgressHUDModeIndeterminate;
        hud.labelText = @"发送中...";
        
    }
}

-(void)onBtnNext
{
    if ([_smsCodeField.text length] == 0)
    {
        [_smsCodeField shake:10
                 withDelta:5.0f
                     speed:0.04f
            shakeDirection:ShakeDirectionHorizontal];
    }
    else
    {
        _btnNext.enabled = NO;
        
        MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:self.view animated:YES];
        hud.mode = MBProgressHUDModeIndeterminate;
        hud.labelText = @"验证中...";
        
    }
}

-(void)onBtnRestPsw
{
    if (_pswField.text.length == 0 || _rePswField.text.length == 0 )
    {
        UIAlertView *alertV = [[UIAlertView alloc] initWithTitle:@"提示" message:@"输入项不能为空" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        [alertV show];
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
        hud.labelText = @"处理中...";
        
        [[HttpService getInstance] resetPsw:_phoneField.text psw:_pswField.text andBlock:^(int retValue) {
            
            _registerStep = 3;
            _btnConfirm.enabled = YES;
            [MBProgressHUD hideHUDForView:self.view animated:YES];
            
            NSString *tipMessage = @"";
            
            if (retValue == 200)
            {
                tipMessage = @"重置密码成功";
                
                [[NSUserDefaults standardUserDefaults] setObject:_phoneField.text forKey:@"userName"];
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
    if (_registerStep != 3)
    {
        [self refreshLayout:_registerStep];
    }
    else
    {
        [self.navigationController popViewControllerAnimated:YES];
    }
}

-(void)refreshLayout:(int)style
{
    CATransition *animation = [CATransition animation];
    animation.type = kCATransitionMoveIn;
    animation.duration = 0.4;
    
    [_phoneImgView.layer addAnimation:animation forKey:nil];
    [_smsImgView.layer addAnimation:animation forKey:nil];
    [_pswImgView1.layer addAnimation:animation forKey:nil];
    [_pswImgView2.layer addAnimation:animation forKey:nil];
    [_phoneField.layer addAnimation:animation forKey:nil];
    [_smsCodeField.layer addAnimation:animation forKey:nil];
    [_pswField.layer addAnimation:animation forKey:nil];
    [_horizontalMark3.layer addAnimation:animation forKey:nil];
    [_btnSMS.layer addAnimation:animation forKey:nil];
    [_btnNext.layer addAnimation:animation forKey:nil];
    [_btnConfirm.layer addAnimation:animation forKey:nil];
    [_progressImgView.layer addAnimation:animation forKey:nil];
    
    if (style == 0)
    {
        _phoneImgView.hidden = NO;
        _smsImgView.hidden = YES;
        _pswImgView1.hidden = YES;
        _pswImgView2.hidden = YES;
        _phoneField.hidden = NO;
        _smsCodeField.hidden = YES;
        _pswField.hidden = YES;
        _rePswField.hidden = YES;
        _horizontalMark3.hidden = YES;
        _btnSMS.hidden = NO;
        _btnNext.hidden = YES;
        _btnConfirm.hidden = YES;
        _progressImgView.image = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"registerp1@2x" ofType:@"png"]];
    }
    else if(style == 1)
    {
        _phoneImgView.hidden = YES;
        _smsImgView.hidden = NO;
        _pswImgView1.hidden = YES;
        _pswImgView2.hidden = YES;
        _phoneField.hidden = YES;
        _smsCodeField.hidden = NO;
        _pswField.hidden = YES;
        _rePswField.hidden = YES;
        _horizontalMark3.hidden = YES;
        _btnSMS.hidden = YES;
        _btnNext.hidden = NO;
        _btnConfirm.hidden = YES;
        _progressImgView.image = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"registerp2@2x" ofType:@"png"]];
    }
    else
    {
        _phoneImgView.hidden = YES;
        _smsImgView.hidden = YES;
        _pswImgView1.hidden = NO;
        _pswImgView2.hidden = NO;
        _phoneField.hidden = YES;
        _smsCodeField.hidden = YES;
        _pswField.hidden = NO;
        _rePswField.hidden = NO;
        _horizontalMark3.hidden = NO;
        _btnSMS.hidden = YES;
        _btnNext.hidden = YES;
        _btnConfirm.hidden = NO;
        _progressImgView.image = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"registerp3@2x" ofType:@"png"]];
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc
{
    NSLog(@"resetPswViewController dealloc");
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
