//
//  SuperTeamManagerProtocol.h
//  NIMLib
//
//  Created by He on 2019/5/14.
//  Copyright © 2019 Netease. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NIMSuperTeamDefs.h"
#import "NIMSuperTeam.h"
#import "NIMSuperTeamMember.h"
#import "NIMSuperTeamFetchMemberOption.h"

NS_ASSUME_NONNULL_BEGIN

/**
 *  通用的群组操作block
 *
 *  @param error 错误,如果成功则error为nil
 */
typedef void(^NIMSuperTeamHandler)(NSError * __nullable error);



/**
 *  拉取群信息Block
 *
 *  @param error 错误,如果成功则error为nil
 *  @param team  群信息
 */
typedef void(^NIMSuperTeamFetchInfoHandler)(NSError * __nullable error, NIMSuperTeam * __nullable team);


/**
 *  群成员获取 block
 *
 *  @param error   错误,如果成功则error为nil
 *  @param members 成功的群成员列表,内部为NIMSuperTeamMember
 */
typedef void(^NIMSuperTeamMemberHandler)(NSError * __nullable error, NSArray<NIMSuperTeamMember *> * __nullable members);

/**
 *  邀请人Accids
 *
 *  @param error       错误,如果成功则error为nil
 *  @param inviters    群成员与邀请人关系
 */
typedef void(^NIMSuperTeamFetchInviterAccidsHandler)(NSError * __nullable error, NSDictionary<NSString *, NSString *> * __nullable inviters);

/**
 *  群成员获取 block
 *
 *  @param error   错误,如果成功则error为nil
 *  @param members 成功的群成员列表,内部为NIMSuperTeamMember
 */
typedef void(^NIMSuperTeamMemberHandler)(NSError * __nullable error, NSArray<NIMSuperTeamMember *> * __nullable members);

/**
 *  超大群委托
 */
@protocol NIMSuperTeamManagerDelegate <NSObject>

@optional
/**
 *  群组增加回调
 *
 *  @param team 添加的群组
 */
- (void)onSuperTeamAdded:(NIMSuperTeam *)team;

/**
 *  群组更新回调
 *
 *  @param team 更新的群组
 */
- (void)onSuperTeamUpdated:(NIMSuperTeam *)team;

/**
 *  群组移除回调
 *
 *  @param team 被移除的群组
 */
- (void)onSuperTeamRemoved:(NIMSuperTeam *)team;

/**
 *  群组成员变动回调,包括数量增减以及成员属性变动
 *
 *  @param team 变动的群组
 */
- (void)onSuperTeamMemberChanged:(NIMSuperTeam *)team;

@end

/**
 *  超大群接口
 */
@protocol NIMSuperTeamManager  <NSObject>

/**
 *  获取所有群组
 *
 *  @return 返回所有群组
 */
- (nullable NSArray<NIMSuperTeam *> *)allMyTeams;

/**
 *  根据群组 ID 获取具体的群组信息
 *
 *  @param teamId 群组 ID
 *
 *  @return 群组信息
 *  @discussion 如果自己不在群里，则该接口返回 nil
 */
- (nullable NIMSuperTeam *)teamById:(NSString *)teamId;

/**
 *  邀请用户入群
 *
 *  @param users       用户ID列表
 *  @param teamId      群组ID
 *  @param completion  完成后的回调
 */
- (void)addUsers:(NSArray<NSString *>  *)users
          toTeam:(NSString *)teamId
      completion:(nullable NIMSuperTeamMemberHandler)completion;

/**
 *  从群组内移除成员
 *
 *  @param users       需要移除的用户ID列表
 *  @param teamId      群组ID
 *  @param completion  完成后的回调
 */
- (void)kickUsers:(NSArray<NSString *> *)users
         fromTeam:(NSString *)teamId
       completion:(nullable NIMSuperTeamHandler)completion;


/**
 *  退出群组
 *
 *  @param teamId     群组ID
 *  @param completion 完成后的回调
 */
- (void)quitTeam:(NSString *)teamId
      completion:(nullable NIMSuperTeamHandler)completion;

/**
 *  获取群信息
 *
 *  @param teamId      群组ID
 *  @param completion  完成后的回调
 */
- (void)fetchTeamInfo:(NSString *)teamId
           completion:(nullable NIMSuperTeamFetchInfoHandler)completion;


/**
 *  更新群信息
 *
 *  @param values      需要更新的群信息键值对
 *  @param teamId      群组ID
 *  @param completion  完成后的回调
 *  @discussion   这个接口可以一次性修改群的多个属性,如名称,公告等,传入的数据键值对是 {@(NIMSuperTeamUpdateTag) : NSString},无效数据将被过滤
 */
- (void)updateTeamInfos:(NSDictionary<NSNumber *,NSString *> *)values
                 teamId:(NSString *)teamId
             completion:(nullable NIMSuperTeamHandler)completion;


/**
 *  更新成员群昵称
 *
 *  @param newNick      新的群成员昵称
 *  @param teamId       群组ID
 *  @param completion   完成后的回调
 */
- (void)updateMyNick:(NSString *)newNick
              inTeam:(NSString *)teamId
          completion:(nullable NIMSuperTeamHandler)completion;

/**
 *  更新成员群自定义属性
 *
 *  @param newInfo      新的自定义属性
 *  @param teamId       群组ID
 *  @param completion   完成后的回调
 */
- (void)updateMyCustomInfo:(NSString *)newInfo
                    inTeam:(NSString *)teamId
                completion:(nullable NIMSuperTeamHandler)completion;

/**
 *  获取超大群组成员
 *
 *  @param teamId     群组ID
 *  @param completion 完成后的回调
 *  @discussion   绝大多数情况这个请求都是从本地读取缓存并同步返回，但是由于群成员信息量较大， SDK 采取的是登录后延迟拉取的策略
 *                考虑到用户网络等问题, SDK 有可能没有及时缓存群成员信息,那么这个请求将是个带网络请求的异步操作(增量请求)。
 *                同时这个接口会去请求本地没有缓存的群用户的资料信息，但不会触发 - (void)onUserInfoChanged: 回调。
 */
- (void)fetchTeamMembers:(NSString *)teamId
                  option:(NIMSuperTeamFetchMemberOption *)option
              completion:(nullable NIMSuperTeamMemberHandler)completion;


/**
 *  添加超大群组委托
 *
 *  @param delegate 群组委托
 */
- (void)addDelegate:(id<NIMSuperTeamManagerDelegate>)delegate;

/**
 *  移除超大群组委托
 *
 *  @param delegate 群组委托
 */
- (void)removeDelegate:(id<NIMSuperTeamManagerDelegate>)delegate;
@end


NS_ASSUME_NONNULL_END
