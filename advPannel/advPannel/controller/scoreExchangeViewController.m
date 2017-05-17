//
//  scoreExchangeViewController.m
//  advPannel
//
//  Created by apple on 15/10/19.
//  Copyright © 2015年 jason. All rights reserved.
//

#import "scoreExchangeViewController.h"
#import "ProtoType.h"
#import "HttpService.h"
#import "UITextField+Shake.h"
#import "MBProgressHUD.h"

@interface scoreExchangeViewController ()<UITextFieldDelegate,UIPickerViewDelegate,UIPickerViewDataSource>
@property(nonatomic) UIPickerView *pickerV;
@property(nonatomic) int selectedIndex;
@end

@implementation scoreExchangeViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.view.backgroundColor = [UIColor colorWithRed:0.9647f green:0.9647f blue:0.9647f alpha:1.0f];
    
    CGRect clientRect = [UIScreen mainScreen].bounds;
    
    UILabel *titleLable = [[UILabel alloc] initWithFrame:CGRectMake(clientRect.size.width/2-130, 0, 130, 20)];
    titleLable.font = [UIFont boldSystemFontOfSize:20];
    titleLable.textColor = [UIColor whiteColor];
    titleLable.textAlignment = NSTextAlignmentCenter;
    titleLable.text = _viewTitle;
    
    self.navigationItem.titleView = titleLable;
    
    UIView *pannel = [[UIView alloc] initWithFrame:CGRectMake(0, 50, CGRectGetWidth(clientRect), 100)];
    [self.view addSubview:pannel];
    
    pannel.backgroundColor = [UIColor whiteColor];
    pannel.layer.borderWidth = 0.5;
    pannel.layer.borderColor = [UIColor lightGrayColor].CGColor;
    
    UIView *spe = [[UIView alloc] initWithFrame:CGRectMake(100, 50, CGRectGetWidth(clientRect)-100, 0.5)];
    [pannel addSubview:spe];
    
    spe.backgroundColor = [UIColor lightGrayColor];
    
    UILabel *itemLabel1 = [[UILabel alloc] initWithFrame:CGRectMake(10, 15, 80, 20)];
    [pannel addSubview:itemLabel1];
    
    itemLabel1.font = [UIFont systemFontOfSize:14];
    itemLabel1.textAlignment = NSTextAlignmentRight;
    itemLabel1.text = _dstItemName;
    
    UITextField *itemField1 = [[UITextField alloc] initWithFrame:CGRectMake(105, 17, 200, 20)];
    [pannel addSubview:itemField1];
    
    itemField1.tag = 0x1000;
    itemField1.backgroundColor = [UIColor clearColor];
    itemField1.placeholder = _dstDesc;
    itemField1.returnKeyType = UIReturnKeyDone;
    itemField1.autocapitalizationType = UITextAutocapitalizationTypeNone;
    itemField1.clearButtonMode = UITextFieldViewModeWhileEditing;
    itemField1.delegate = self;
    
    UILabel *itemLabel2 = [[UILabel alloc] initWithFrame:CGRectMake(10, 65, 80, 20)];
    [pannel addSubview:itemLabel2];
    
    itemLabel2.font = [UIFont systemFontOfSize:14];
    itemLabel2.textAlignment = NSTextAlignmentRight;
    itemLabel2.text = @"花费金币:";
    
    UITextField *itemField2 = [[UITextField alloc] initWithFrame:CGRectMake(105, 67, 200, 20)];
    [pannel addSubview:itemField2];
    
    itemField2.tag = 0x1001;
    itemField2.backgroundColor = [UIColor clearColor];
    itemField2.returnKeyType = UIReturnKeyDone;
    itemField2.autocapitalizationType = UITextAutocapitalizationTypeNone;
    itemField2.clearButtonMode = UITextFieldViewModeWhileEditing;
    itemField2.enabled = NO;
    itemField2.delegate = self;
    
    UILabel *noticeLabel = [[UILabel alloc] init];
    [self.view addSubview:noticeLabel];
    
    noticeLabel.font = [UIFont systemFontOfSize:12];
    noticeLabel.textColor = [UIColor lightGrayColor];
    noticeLabel.text = @"所有兑换将于次日的24点到账,请耐心等待。本软件所有活动与苹果官方无关";
    noticeLabel.numberOfLines = 0;
    
    CGSize textSize = [noticeLabel.text boundingRectWithSize:CGSizeMake(clientRect.size.width-20, MAXFLOAT) options:NSStringDrawingUsesLineFragmentOrigin attributes:@{NSFontAttributeName:[UIFont systemFontOfSize:12]} context:nil].size;
    
    CGRect textRect = CGRectMake(10, 170, textSize.width, textSize.height);
    
    noticeLabel.frame = textRect;
    
    UIButton *confirmBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    [self.view addSubview:confirmBtn];
    
    confirmBtn.frame = CGRectMake(5, 215, CGRectGetWidth(clientRect)-10, 35);
    confirmBtn.layer.cornerRadius = 17.5;
    confirmBtn.showsTouchWhenHighlighted = YES;
    confirmBtn.titleLabel.font = [UIFont systemFontOfSize:20];
    confirmBtn.titleLabel.textColor = [UIColor whiteColor];
    confirmBtn.backgroundColor = [UIColor colorWithRed:0.2235f green:0.6235f blue:0.8745f alpha:1.0f];
    [confirmBtn setTitle: @"兑 换" forState: UIControlStateNormal];
    [confirmBtn addTarget:self action:@selector(onBtnExchange) forControlEvents:UIControlEventTouchUpInside];
    
    _pickerV = [[UIPickerView alloc] initWithFrame:CGRectMake(10, 290, CGRectGetWidth(clientRect)-20, 150)];
    [self.view addSubview:_pickerV];
    
    _pickerV.delegate = self;
    _pickerV.dataSource = self;
    
    [_pickerV selectRow:0 inComponent:0 animated:NO];
    
    ExchangeItem *item = _dstArray[0];
    _selectedIndex = 0;
    
    if ([item.type isEqualToString:@"1"]) {
        itemField1.text = [HttpService getInstance].userBaseInfo.alipayId;
    }
    else if([item.type isEqualToString:@"2"])
    {
        itemField1.text = [HttpService getInstance].userBaseInfo.qq;
    }
    else
    {
        itemField1.text = [HttpService getInstance].userBaseInfo.phone;
    }
    
    itemField2.text = item.value;
}

- (void)onBtnExchange
{
    UITextField *textfid = (UITextField*)[self.view viewWithTag:0x1000];
    
    if (!textfid.text.length) {
        [textfid shake:10
             withDelta:5.0f
                 speed:0.04f
        shakeDirection:ShakeDirectionHorizontal];
        return;
    }
    
    ExchangeItem *excItem = (ExchangeItem*)_dstArray[_selectedIndex];
    
    if ([excItem.value intValue] > [HttpService getInstance].userBaseInfo.score) {
        UIAlertView *alertV = [[UIAlertView alloc] initWithTitle:@"提示" message:@"您的金币不够兑换" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        [alertV show];
        return;
    }
    
    MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:self.view animated:YES];
    hud.mode = MBProgressHUDModeIndeterminate;
    hud.labelText = @"提交兑换...";
    
    [[HttpService getInstance] sumbitExchange:textfid.text item:excItem andBlock:^(int retValue) {
        
        [MBProgressHUD hideHUDForView:self.view animated:YES];
        
        NSString *resultString = @"";
        
        if (retValue != 200) {
            resultString = @"已经成功提交兑换请求,请耐心等待";
        }
        else
        {
            resultString = @"提交失败";
        }
        
        UIAlertView *alertV = [[UIAlertView alloc] initWithTitle:@"提示" message:resultString delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        [alertV show];
    }];
}

-(NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView{
    return 1;
}

-(NSInteger) pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component{
    return _dstArray.count;
}

-(NSString*) pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component{
    
    ExchangeItem *item = [_dstArray objectAtIndex:row];
    return item.itemDesc;
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    ExchangeItem *item = [_dstArray objectAtIndex:row];
    
    UITextField *textfid = (UITextField*)[self.view viewWithTag:0x1001];
    textfid.text = item.value;
    
    _selectedIndex = (int)row;
}

#if 0
- (UIView *)pickerView:(UIPickerView *)pickerView viewForRow:(NSInteger)row forComponent:(NSInteger)component reusingView:(UIView *)view
{
    ExchangeItem *item = [_dstArray objectAtIndex:row];
    
    UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(12.0f, 0.0f, [pickerView rowSizeForComponent:component].width-12, [pickerView rowSizeForComponent:component].height)];
    
    label.text = item.itemDesc;
    label.backgroundColor = [UIColor clearColor];
    label.textAlignment = NSTextAlignmentCenter;
    
    return label;
}
#endif

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return YES;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
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
