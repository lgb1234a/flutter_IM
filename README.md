# quote

基于云信IM实现的类微信IM聊天框架（iOS可运行，Android暂未实现）

## 目录结构

```

app/
|---- android  #暂无
|---- images   #flutter侧 图片文件目录
|---- ios      #ios 工作区
    Appdelegate   
    |---- Runner       #iOS 业务代码区
        |---- Contacts    #通讯录（纯flutter UI）
        |---- Hybird      #flutter&native交互bridge
        |---- Login       #为空，目前登录UI为纯flutter页面
        |---- Mine        #我的（纯flutter UI）
        |---- Session     #会话（嵌套的云信IM页面）
|---- lib   #flutter 代码
    |---- Base # 网络请求、util、hybird bridge
    |---- Contacts  # 通讯录UI
    |---- Login  #登录页UI
    |---- Mine   #我的
    |---- Session   #会话页
    main.dart  #flutter 入口以及页面路由
|---- nim_sdk_util  #云信SDK服务的插件
|---- wx_sdk    #微信登录、分享的插件

```

## 框架思路（遇到的几个问题）

### 如何管理页面堆栈

在解决这个问题的时候，我走了很多弯路，一开始打算是flutter为主，native为辅（毕竟是从零开始的新项目），然后在管理堆栈的时候遇到很多挑战，这种做法在flutter--push-->flutter很简单，当我要flutter--push-->native或者native--push-->flutter的时候，就蒙圈了。当时找了咸鱼的flutter_boost解决方案，奈何他们的flutter版本还未支持到1.7，集成进来之后各种问题，遂放弃之。

随后我改变思路，采用以native为骨架，flutter为血肉的方式将我这个剪不断理还乱的工程重构了一遍。先抽出来一个继承自`FlutterViewController`的基类`CJViewController`，然后提供一个初始化方法`- (instancetype)initWithFlutterOpenUrl:(NSString *)openUrl;`，通过`FlutterViewController`的`setInitialRoute`方法，这样外界传入一个自定义好的路由url，就可以解析到对应的flutter页面，并且可以由native来进行堆栈管理，也可以采用flutter `Navigator`的转场方式跳转到另一个flutter页面。

`flutter`侧：
```
    Map params = {'route':'setting','channel_name':'com.zqtd.cajian/setting'};
    String pStr = convert.jsonEncode(params);
    model.platform.invokeMethod('pushViewControllerWithOpenUrl:', [pStr]);

```

`native`侧：
```
    NSString *openUrl = @"{\"route\":\"login_entrance\",\"channel_name\":\"com.zqtd.cajian/login_entrance\"}";
    CJViewController *nextVc = [[CJViewController alloc] initWithFlutterOpenUrl:openUrl];
    [self.navigationController pushViewController:nextVc
                                         animated:YES];
```

### 如何在native让我集成的插件代码也可以发起网络请求，做一些与用户的反馈交互（弹出提示框hub之类的——Base/里面的代码）

在集成微信登录sdk插件的时候，我并不想只是简单的将微信sdk的方法简单的bridge一遍，然后交给flutter调用。我希望在`sendReq`的同时，我的插件可以处理回调，并一气呵成的完成微信登录的整套操作，包括调用我的网络请求，进行登录提示。但是我不可能把native主工程的代码再在插件pod bridge代码里面再重写一遍，这样即低效又丑陋。我想到了flutter插件的podspec可以依赖其他的pod代码，于是我尝试把我需要用的常用代码（网络请求，弹窗组件，扩展方法等）封装成私有仓库，然后再在插件的podspec里面添加这个依赖，事实证明这样是可行的，由此我便实现了在微信sdk插件里面完成整套微信登录流程。

### 如何进行跨平台通信

这一块也是我初学时比较头疼的，按照官方的思路，传递根视图控制器的`binaryMessenger`注册channel，然后在flutter页面完成对应的注册操作就可以建立通信了。在一开始我采用flutter嵌套native的框架思路时，发现当我登录完成，替换我的keywindow的根视图之后，我的通信就中断了。后来我发现每次当你的flutter路由被native切断，你就需要重新注册你的channel，不然你的消息就无法传递下去。而我实现公共bridge方法的目的是，我可以通过它在任何地方进行双端的通信。于是在我完成页面堆栈的管理之后，在我的基类`CJViewController`初始化方法里，注册这个同名channel，这样不管我是在native页面还是flutter页面，获取到的channel都是同一个。

当我一个flutter页面需要调用一些native操作时，我可以通过创建`CJViewController`的子类，在`- (instancetype)initWithFlutterOpenUrl:(NSString *)openUrl;`的openUrl里面指定我的channelName，然后完成一个独立的私有的通道。

`CJViewController.h`

```
/**
 初始化一个flutter 页面，以FlutterVC为容器

 \\******
 需要的JSON字符串格式如下
 {
 'route':'login',
 'channel_name':'com.zqtd.cajian/login',
 'params':{
    'team_id':'298ssdj9238'
    }
 }
 *******\\
 @param openUrl 页面初始化路由和参数
 
 @return 返回VC
 */
- (instancetype)initWithFlutterOpenUrl:(NSString *)openUrl;

```

`CJViewController.m`
```
- (instancetype)initWithFlutterOpenUrl:(NSString *)openUrl
{
    self = [super initWithProject:nil
                          nibName:nil
                           bundle:nil];
    if(self) {
        [self setInitialRoute:openUrl];
        [self registerChannel];
        
        NSDictionary *params = [NSDictionary cj_dictionary:openUrl];
        
        // 设置回调
        _mc = [FlutterMethodChannel methodChannelWithName:params[@"channel_name"] binaryMessenger:self.engine.binaryMessenger];
        
        __weak typeof(self) wself = self;
        [_mc setMethodCallHandler:^(FlutterMethodCall * _Nonnull call, FlutterResult  _Nonnull result) {
            ZZLog(@"flutter call :%@", call.method);
            SEL callMethod = NSSelectorFromString(call.method);
            if([wself respondsToSelector:callMethod]) {
                [wself performSelector:callMethod
                            withObject:call.arguments
                            afterDelay:0];
            }else {
                ZZLog(@"%@未实现%@", NSStringFromClass(wself.class), call.method);
            }
        }];
        
        // 渲染完成
        [self setFlutterViewDidRenderCallback:^{
//            [_mc invokeMethod:@"会在widget build完成之后调用" arguments:nil];
        }];
        
    }
    return self;
}

/// util 
- (void)registerChannel
{
    __weak typeof(self) weakSelf = self;
    
    _utilChannel = [FlutterMethodChannel
                    methodChannelWithName:@"com.zqtd.cajian/util"
                    binaryMessenger:self.engine.binaryMessenger];
    
    [_utilChannel setMethodCallHandler:^(FlutterMethodCall *call, FlutterResult result) {
        SEL callMethod = NSSelectorFromString(call.method);
        if([weakSelf respondsToSelector:callMethod])
        {
            [weakSelf performSelector:callMethod
                           withObject:call.arguments
                           afterDelay:0];
        }else {
            [CJUtilBridge bridgeCall:call result:result];
        }
    }];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    // (@"view did load --- 会在widget build开始之前调用");
    [GeneratedPluginRegistrant registerWithRegistry:self];
}

// 从flutter发来的push新页面操作
- (void)pushViewControllerWithOpenUrl:(NSArray *)params
{
    NSString *openUrl = params.firstObject;
    CJViewController *nextVc = [[CJViewController alloc] initWithFlutterOpenUrl:openUrl];
    [self.navigationController pushViewController:nextVc
                                         animated:YES];
}

// 推出当前页
- (void)popFlutterViewController
{
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)dealloc
{
    ZZLog(@"%@ - dealloced!", NSStringFromClass(self.class));
}
```









