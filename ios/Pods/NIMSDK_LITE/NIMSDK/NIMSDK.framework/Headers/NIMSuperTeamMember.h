//
//  NIMSuperTeamMember.h
//  NIMLib
//
//  Created by He on 2019/5/14.
//  Copyright © 2019 Netease. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NIMSuperTeamDefs.h"

NS_ASSUME_NONNULL_BEGIN

@interface NIMSuperTeamMember : NSObject
/**
 *  群ID
 */
@property (nullable,nonatomic,copy,readonly)         NSString *teamId;

/**
 *  群成员ID
 */
@property (nullable,nonatomic,copy,readonly)         NSString *userId;


/**
 *  邀请者Accid
 */
@property (nullable,nonatomic,copy,readonly)         NSString *inviterAccid;

/**
 *  群成员类型
 */
@property (nonatomic,assign)                NIMSuperTeamMemberType  type;


/**
 *  群昵称
 */
@property (nullable,nonatomic,copy)         NSString *nickname;


/**
 *  被禁言
 */
@property (nonatomic,assign,readonly)       BOOL isMuted;

/**
 *  进群时间
 */
@property (nonatomic,assign,readonly)       NSTimeInterval createTime;


/**
 *  新成员群自定义信息
 */
@property (nullable,nonatomic,copy)        NSString *customInfo;
@end

NS_ASSUME_NONNULL_END
