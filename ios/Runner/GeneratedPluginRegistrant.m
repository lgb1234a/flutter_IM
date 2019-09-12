//
//  Generated file. Do not edit.
//

#import "GeneratedPluginRegistrant.h"
#import <nim_sdk_util/NimSdkUtilPlugin.h>
#import <shared_preferences/SharedPreferencesPlugin.h>
#import <wx_sdk/WxSdkPlugin.h>

@implementation GeneratedPluginRegistrant

+ (void)registerWithRegistry:(NSObject<FlutterPluginRegistry>*)registry {
  [NimSdkUtilPlugin registerWithRegistrar:[registry registrarForPlugin:@"NimSdkUtilPlugin"]];
  [FLTSharedPreferencesPlugin registerWithRegistrar:[registry registrarForPlugin:@"FLTSharedPreferencesPlugin"]];
  [WxSdkPlugin registerWithRegistrar:[registry registrarForPlugin:@"WxSdkPlugin"]];
}

@end
