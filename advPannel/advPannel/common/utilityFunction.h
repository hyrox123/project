//
//  utilityFunction.h
//  YSTParentClient
//
//  Created by apple on 14-10-15.
//  Copyright (c) 2014年 jason. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface utilityFunction : NSObject
+(utilityFunction*)getInstance;
+(NSString*)getIDFA;
+(NSString*)GBK2UTF8:(char*)data;
+(UIImage*)createImage:(int)r green:(int)g blue:(int)b alpha:(int)a width:(int)w height:(int)h;
+(UIImage*)createImage:(Byte*)rgbbuffer width:(int)w height:(int)h;
+(UIImage*)thumbnailWithImageWithoutScale:(UIImage *)image size:(CGSize)asize;
+(UIImage*)getImageByUrl:(NSString*)url;
+(UIImage*)strethImageWith:(NSString*)imageName;
+(NSString*)md5:(NSString *)str;
+(NSString*)getErrorString:(int)code;
+(NSTimeInterval)compareTime:(NSString*)str1 time2:(NSString*)str2 complex:(BOOL)opt;
+(NSMutableDictionary*)getChineseDate:(NSString*)str complex:(BOOL)opt;
+(NSString*)getTraditionalDate:(NSString*)str complex:(BOOL)opt;
+(NSString*)getTimeNow:(BOOL)opt;
+(int)compareDay:(NSString*)str1 time2:(NSString*)str2 complex:(BOOL)opt;
+(bool)timeExpire:(NSString*)source;
+(void)setExtraCellLineHidden: (UITableView *)tableView;
+(UIImage*)saveView2Img:(UIView*)view;
+(UIImage*)getGrayImage:(UIImage*)sourceImage;
+(CGSize)downloadImageSizeWithURL:(id)imageURL;
@end
