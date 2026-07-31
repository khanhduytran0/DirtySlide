//
//  PrivateHeaders.h
//  DirtySlide
//
//  Created by Duy Tran on 31/7/26.
//
@import Foundation;

@interface _LSOpenConfiguration : NSObject
@property(nonatomic, copy) NSDictionary *frontBoardOptions;
@end

extern NSString* FBSOpenApplicationOptionKeyActivateAsClassic;
extern NSString* FBSOpenApplicationOptionKeyPayloadURL;

@interface LSApplicationWorkspace : NSObject
+ (instancetype)defaultWorkspace;
- (BOOL)openApplicationWithBundleID:(NSString *)arg1 ;
- (BOOL)openURL:(id)url;
- (BOOL)isApplicationAvailableToOpenURL:(id)arg1 error:(id*)arg2;
- (void)openApplicationWithBundleIdentifier:(NSString *)bundleID
                              configuration:(_LSOpenConfiguration *)configuration
                          completionHandler:(void (^)(BOOL, NSError *))completion;
- (void)openApplicationWithBundleIdentifier:(NSString *)bundleID
                         usingConfiguration:(_LSOpenConfiguration *)configuration
                          completionHandler:(void (^)(BOOL, NSError *))completion;
@end
