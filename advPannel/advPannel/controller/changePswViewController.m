//
//  changePswViewController.m
//  YSTParentClient
//
//  Created by apple on 15/4/15.
//  Copyright (c) 2015年 jason. All rights reserved.
//

#import "changePswViewController.h"
#import "MBProgressHUD.h"
#import "HttpService.h"
#import "utilityFunction.h"

@interface changePswViewController ()<UITextFieldDelegate>
@property(nonatomic) UIImageView *pswImgView1, *pswImgView2, *pswImgView3;
@property(nonatomic) UITextField *pswField1, *pswField2, *pswField3;
@property(nonatomic) UIView *horizontalMark1, *horizontalMark2, *horizontalMark3;
@property(nonatomic) UIButton *btnConfirm;
@property(nonatomic) BOOL succeed;

-(void)onBtnChangePsw;
@end

@implementation changePswViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    [self.navigationController setNavigationBarHidden:NO animated:NO];
    
    CGRect frame = [ UIScreen mainScreen ].bounds;
    
    self.view.backgroundColor = [UIColor whiteColor];
    
    UILabel *titleLable = [[UILabel alloc] initWithFrame:CGRectMake(frame.size.width/2-130, 0, 130, 20)];
    titleLable.font = [UIFont systemFontOfSize:20];
    titleLable.textColor = [UIColor whiteColor];
    titleLable.textAlignment = NSTextAlignmentCenter;
    titleLable.text = @"修改密码";
    
    self.navigationItem.titleView = titleLable;
    
    _horizontalMark1 = [[UIView alloc] initWithFrame:CGRectMake(15, 75, frame.size.width-30, 0.5)];
    [self.view addSubview:_horizontalMark1];
    _horizontalMark1.backgroundColor = [UIColor lightGrayColor];
    
    _pswImgView1 = [[UIImageView alloc] initWithFrame:CGRectMake(20, 32, 30, 30)];
    [self.view addSubview:_pswImgView1];
    _pswImgView1.image = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"den_suo_@2x" ofType:@"png"]];
    
    _pswImgView2 = [[UIImageView alloc] initWithFrame:CGRectMake(20, 92, 30, 30)];
    [self.view addSubview:_pswImgView2];
    _pswImgView2.image = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"den_suo_@2x" ofType:@"png"]];
    
    _pswImgView3 = [[UIImageView alloc] initWithFrame:CGRectMake(20, 152, 30, 30)];
    [self.view addSubview:_pswImgView3];
    _pswImgView3.image = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"den_suo_@2x" ofType:@"png"]];
    
    _pswField1 = [[UITextField alloc] initWithFrame:CGRectMake(70, 35, 180, 30)];
    [self.view addSubview:_pswField1];
    
    _pswField1.backgroundColor = [UIColor clearColor];
    _pswField1.placeholder = @"输入旧密码";
    _pswField1.returnKeyType = UIReturnKeyDone;
    _pswField1.autocapitalizationType = UITextAutocapitalizationTypeNone;
    _pswField1.clearButtonMode = UITextFieldViewModeWhileEditing;
    _pswField1.secureTextEntry = YES;
    _pswField1.delegate = self;
    
    _horizontalMark2 = [[UIView alloc] initWithFrame:CGRectMake(15, 135, frame.size.width-30, 0.5)];
    [self.view addSubview:_horizontalMark2];
    _horizontalMark2.backgroundColor = [UIColor lightGrayColor];
    
    _pswField2 = [[UITextField alloc] initWithFrame:CGRectMake(70, 95, 180, 30)];
    [self.view addSubview:_pswField2];
    
    _pswField2.backgroundColor = [UIColor clearColor];
    _pswField2.placeholder = @"输入新密码";
    _pswField2.returnKeyType = UIReturnKeyDone;
    _pswField2.autocapitalizationType = UITextAutocapitalizationTypeNone;
    _pswField2.secureTextEntry = YES;
    _pswField2.clearButtonMode = UITextFieldViewModeWhileEditing;
    _pswField2.delegate = self;
    
    _pswField3 = [[UITextField alloc] initWithFrame:CGRectMake(70, 155, 180, 30)];
    [self.view addSubview:_pswField3];
    
    _pswField3.backgroundColor = [UIColor clearColor];
    _pswField3.placeholder = @"重复输入密码";
    _pswField3.returnKeyType = UIReturnKeyDone;
    _pswField3.autocapitalizationType = UITextAutocapitalizationTypeNone;
    _pswField3.secureTextEntry = YES;
    _pswField3.clearButtonMode = UITextFieldViewModeWhileEditing;
    _pswField3.delegate = self;
    
    _horizontalMark3 = [[UIView alloc] initWithFrame:CGRectMake(15, 195, frame.size.width-30, 0.5)];
    [self.view addSubview:_horizontalMark3];
    _horizontalMark3.backgroundColor = [UIColor lightGrayColor];
    
    _btnConfirm = [UIButton buttonWithType:UIButtonTypeCustom];
    [self.view addSubview:_btnConfirm];
    
    _btnConfirm.frame = CGRectMake(10, 220, frame.size.width-20, 35);
    _btnConfirm.showsTouchWhenHighlighted = YES;
    _btnConfirm.layer.cornerRadius = 17.5;
    _btnConfirm.titleLabel.font = [UIFont systemFontOfSize:20];
    _btnConfirm.titleLabel.textColor = [UIColor whiteColor];
    _btnConfirm.backgroundColor = [UIColor colorWithRed:0.2235f green:0.6235f blue:0.8745f alpha:1.0f];
    [_btnConfirm setTitle: @"确  定" forState: UIControlStateNormal];
    [_btnConfirm addTarget:self action:@selector(onBtnChangePsw) forControlEvents:UIControlEventTouchUpInside];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return YES;
}

- (void)onBtnChangePsw
{
    if (_pswField1.text.length == 0 || _pswField2.text.length == 0 || _pswField3.text.length == 0 )
    {
        UIAlertView *alertV = [[UIAlertView alloc] initWithTitle:@"提示" message:@"输入项不能为空" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        [alertV show];
    }
    else if(![_pswField2.text isEqualToString:_pswField3.text])
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
        
        [[HttpService getInstance] modifyPsw:_pswField1.text psw:_pswField2.text andBlock:^(int retValue) {
            
            _btnConfirm.enabled = YES;
            [MBProgressHUD hideHUDForView:self.view animated:YES];
            
            NSString *tipMessage = @"";
            
            if (retValue == 200)
            {
                _succeed = YES;
                tipMessage = @"设置密码成功";
                
                [[NSUserDefaults standardUserDefaults] setObject:_pswField2.text forKey:@"userPsw"];
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

-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (_succeed) {
        [self.navigationController popViewControllerAnimated:YES];
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc
{
    NSLog(@"changePswViewController dealloc");
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
