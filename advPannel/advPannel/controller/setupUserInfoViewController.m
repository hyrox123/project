//
//  userInfoViewController.m
//  YSTParentClient
//
//  Created by apple on 15/3/12.
//  Copyright (c) 2015年 jason. All rights reserved.
//

#import "setupUserInfoViewController.h"
#import "ProtoType.h"
#import "HttpService.h"
#import "utilityFunction.h"
#import "SDTransparentPieProgressView.h"
#import "MBProgressHUD.h"
#import <QuartzCore/QuartzCore.h>

@interface setupUserInfoViewController ()<UITableViewDelegate, UITableViewDataSource, UITextFieldDelegate, UIImagePickerControllerDelegate, UINavigationControllerDelegate>
@property(nonatomic) UITableView *infoList;
@property(nonatomic) UIImageView *portraitView;
@property(nonatomic) UIButton *btnValideTime, *btnForbidden, *btnConfirm;
@property(nonatomic) NSString *portraitUrl;
@property(nonatomic) NSArray *menuItemArray, *validTimeArray;
@property(nonatomic) int selectIndex;

-(void)onBtnModify;
-(void)onClickImg;
@end

@implementation setupUserInfoViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    CGRect clientRect = [UIScreen mainScreen].bounds;
    
    UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 30, 20)];
    titleLabel.font = [UIFont boldSystemFontOfSize:20];
    titleLabel.textColor = [UIColor whiteColor];
    titleLabel.text = @"个人信息";
    
    self.navigationItem.titleView = titleLabel;
    self.view.backgroundColor =  [UIColor colorWithRed:0.9647f green:0.9647f blue:0.9647f alpha:1.0f];
    
    _portraitView = [[UIImageView alloc] initWithFrame:CGRectMake(clientRect.size.width/2-30, 20, 60, 60)];
    _infoList = [[UITableView alloc] initWithFrame:CGRectMake(10, 100, clientRect.size.width-20, 250)];
   
    [self.view addSubview:_portraitView];
    [self.view addSubview:_infoList];
    
    _portraitView.userInteractionEnabled = YES;
    UITapGestureRecognizer *singleTapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onClickImg)];
    [_portraitView addGestureRecognizer:singleTapGesture];
    _portraitView.layer.masksToBounds = YES;
    _portraitView.layer.cornerRadius = 4;
    
    _infoList.delegate = self;
    _infoList.dataSource = self;
    [utilityFunction setExtraCellLineHidden:_infoList];
    
    _btnConfirm = [UIButton buttonWithType:UIButtonTypeCustom];
    _btnConfirm.frame = CGRectMake(5, 375, clientRect.size.width-10, 35);
    
    _btnConfirm.layer.cornerRadius = 17.5;
    _btnConfirm.layer.masksToBounds = YES;
    _btnConfirm.showsTouchWhenHighlighted = YES;
    _btnConfirm.titleLabel.font = [UIFont systemFontOfSize:20];
    _btnConfirm.titleLabel.textColor = [UIColor whiteColor];
    _btnConfirm.backgroundColor = [UIColor colorWithRed:0.2235f green:0.6235f blue:0.8745f alpha:1.0f];
    [_btnConfirm setTitle: @"修 改" forState: UIControlStateNormal];
    [_btnConfirm addTarget:self action:@selector(onBtnModify) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:_btnConfirm];
    
    NSData *picData = [[NSUserDefaults standardUserDefaults] objectForKey:@"userPortrait"];
    
    if (picData) {
        _portraitView.image = [UIImage imageWithData:picData];
    }
    else
    {
        _portraitView.image = [UIImage imageNamed:@"defaultLogo"];
    }
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return 5;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CustomCellIdentifier = @"CustomCellIdentifier";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CustomCellIdentifier];
    
    if (cell == nil) {
        cell = [[UITableViewCell alloc]
                initWithStyle:UITableViewCellStyleDefault
                reuseIdentifier:CustomCellIdentifier];
    }
    
    for (UIView *subview in cell.contentView.subviews) {
        [subview removeFromSuperview];
    }
    
    UILabel *descLabel = [[UILabel alloc] initWithFrame:CGRectMake(15, 10, 75, 30)];
    UITextField *textField = [[UITextField alloc] initWithFrame:CGRectMake(85, 10, 150, 30)];
    textField.returnKeyType = UIReturnKeyDone;
    textField.clearButtonMode = UITextFieldViewModeWhileEditing;
    
    textField.delegate = self;
    descLabel.font = [UIFont systemFontOfSize:18];
    textField.font = [UIFont systemFontOfSize:16];
    
    
    if ([indexPath row] == 0)
    {
        descLabel.text = @"昵称";
        textField.placeholder = @"请输入昵称";
        textField.tag = 0x1001;
        textField.text = [HttpService getInstance].userBaseInfo.nickName;
    }
    else if([indexPath row] == 1)
    {
        descLabel.text = @"手机";
        textField.placeholder = @"请输入手机号";
        textField.tag = 0x1002;
        textField.text = [HttpService getInstance].userBaseInfo.phone;
    }
    else if([indexPath row] == 2)
    {
        descLabel.text = @"QQ";
        textField.placeholder = @"请输入QQ号";
        textField.tag = 0x1003;
        textField.text = [HttpService getInstance].userBaseInfo.qq;
    }
    else if([indexPath row] == 3)
    {
        descLabel.text = @"支付宝";
        textField.placeholder = @"请输入支付宝账号";
        textField.tag = 0x1004;
        textField.text = [HttpService getInstance].userBaseInfo.alipayId;
    }
    else if([indexPath row] == 4)
    {
        descLabel.text = @"推荐人";
        textField.placeholder = @"请输入推荐人账号";
        textField.tag = 0x1005;
        textField.text = [HttpService getInstance].userBaseInfo.referee;
        
        if (textField.text.length > 0) {
            textField.enabled = NO;
        }
    }
    
    [cell.contentView addSubview:descLabel];
    [cell.contentView addSubview:textField];
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
    return cell;
}

#pragma mark Table Delegate Methods
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 50.0;
}

-(void)onBtnModify
{
    UITableViewCell *cell1 = [_infoList cellForRowAtIndexPath:[NSIndexPath indexPathForItem:0 inSection:0]];
    UITableViewCell *cell2 = [_infoList cellForRowAtIndexPath:[NSIndexPath indexPathForItem:1 inSection:0]];
    UITableViewCell *cell3 = [_infoList cellForRowAtIndexPath:[NSIndexPath indexPathForItem:2 inSection:0]];
    UITableViewCell *cell4 = [_infoList cellForRowAtIndexPath:[NSIndexPath indexPathForItem:3 inSection:0]];
    UITableViewCell *cell5 = [_infoList cellForRowAtIndexPath:[NSIndexPath indexPathForItem:4 inSection:0]];
    
    UITextField *textfield1 = (UITextField*)[cell1 viewWithTag:0x1001];
    UITextField *textfield2 = (UITextField*)[cell2 viewWithTag:0x1002];
    UITextField *textfield3 = (UITextField*)[cell3 viewWithTag:0x1003];
    UITextField *textfield4 = (UITextField*)[cell4 viewWithTag:0x1004];
    UITextField *textfield5 = (UITextField*)[cell5 viewWithTag:0x1005];
    
    UserBaseInfo *baseInfo = [UserBaseInfo new];
    
    baseInfo.nickName = textfield1.text;
    baseInfo.phone = textfield2.text;
    baseInfo.qq = textfield3.text;
    baseInfo.alipayId = textfield4.text;
    baseInfo.referee = textfield5.text;
    
    _btnConfirm.enabled = NO;
    MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:self.view animated:YES];
    hud.mode = MBProgressHUDModeIndeterminate;
    hud.labelText = @"处理中...";
    
    [[HttpService getInstance] modifyUserBaseInfo:baseInfo andBlock:^(int retValue) {
        
        _btnConfirm.enabled = YES;
        [MBProgressHUD hideHUDForView:self.view animated:YES];
        
        NSString *messageTip = @"";
        
        if (retValue == 200)
        {
            messageTip = @"修改用户基本信息成功";
        }
        else
        {
            messageTip = @"修改用户基本信息失败";
        }
        
        UIAlertView *alertV = [[UIAlertView alloc] initWithTitle:@"提示" message:messageTip delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        [alertV show];
    }];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return YES;
}

-(void)onClickImg
{
    UIAlertView*alert = [[UIAlertView alloc]initWithTitle:nil
                                                  message:nil
                                                 delegate:self
                                        cancelButtonTitle:@"取消"
                                        otherButtonTitles:@"拍照", @"从手机相册选择",nil];
    
    [alert show];
}

-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == 1)
    {
        [self onBtnClickCamera];
    }
    
    if (buttonIndex == 2)
    {
        [self onBtnClickLocal];
    }
}

-(void)imagePickerController: (UIImagePickerController *)picker
didFinishPickingMediaWithInfo: (NSDictionary *)info
{
    UIImage* srcImage = [info objectForKey: @"UIImagePickerControllerEditedImage"];
    
    CGSize dstSize = {320, 320};
    UIGraphicsBeginImageContext(dstSize);
    [srcImage drawInRect:CGRectMake(0, 0, dstSize.width, dstSize.height)];
    UIImage *dstImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    NSData *picData = UIImageJPEGRepresentation(dstImage, 0.8);
    
    [[NSUserDefaults standardUserDefaults] setObject:picData forKey:@"userPortrait"];
    _portraitView.image = dstImage;
    
    [self dismissViewControllerAnimated:YES completion:nil];
}

-(void) imagePickerControllerDidCancel: (UIImagePickerController *)picker
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

-(void)onBtnClickLocal
{
    UIImagePickerController *imagePickerLocal = [[UIImagePickerController alloc] init];
    imagePickerLocal.delegate = self;
    imagePickerLocal.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    imagePickerLocal.modalTransitionStyle = UIModalTransitionStyleCoverVertical;
    imagePickerLocal.allowsEditing = YES;
    
    [self presentViewController:imagePickerLocal animated:YES completion:nil];
}

-(void)onBtnClickCamera
{
    UIImagePickerController *imagePickerCamera = [[UIImagePickerController alloc] init];
    imagePickerCamera.delegate = self;
    imagePickerCamera.sourceType =  UIImagePickerControllerSourceTypeCamera;
    imagePickerCamera.modalTransitionStyle = UIModalTransitionStyleCoverVertical;
    imagePickerCamera.allowsEditing = YES;
    
    [self presentViewController:imagePickerCamera animated:YES completion:nil];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)dealloc
{
    NSLog(@"userInfoViewController dealoc");
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
