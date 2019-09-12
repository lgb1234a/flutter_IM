//
//  NIMSuperTeamDefs.h
//  NIMLib
//
//  Created by He on 2019/5/17.
//  Copyright © 2019 Netease. All rights reserved.
//

#ifndef NIMSuperTeamDefs_h
#define NIMSuperTeamDefs_h

/**
 *  超大群验证方式
 */
typedef NS_ENUM(NSInteger, NIMSuperTeamJoinMode) {
    /**
     *  允许所有人加入
     */
    NIMSuperTeamJoinModeNoAuth    = 0,
    /**
     *  需要验证
     */
    NIMSuperTeamJoinModeNeedAuth  = 1,
    /**
     *  不允许任何人加入
     */
    NIMSuperTeamJoinModeRejectAll = 2,
};

/**
 *  邀请模式
 */
typedef NS_ENUM(NSInteger, NIMSuperTeamInviteMode) {
    /**
     *  只有管理员/群主可以邀请他人入群
     */
    NIMSuperTeamInviteModeManager    = 0,
    /**
     *   所有人可以邀请其他人入群
     */
    NIMSuperTeamInviteModeAll        = 1,
};


/**
 *  超大群信息修改权限
 */
typedef NS_ENUM(NSInteger, NIMSuperTeamUpdateInfoMode) {
    /**
     *  只有管理员/群主可以修改
     */
    NIMSuperTeamUpdateInfoModeManager    = 0,
    /**
     *  所有人可以修改
     */
    NIMSuperTeamUpdateInfoModeAll  = 1,
};


/**
 *  修改群客户端自定义字段权限
 */
typedef NS_ENUM(NSInteger, NIMSuperTeamUpdateClientCustomMode) {
    /**
     *  只有管理员/群主可以修改
     */
    NIMSuperTeamUpdateClientCustomModeManager    = 0,
    /**
     *  所有人可以修改
     */
    NIMSuperTeamUpdateClientCustomModeAll  = 1,
};


/**
 *  超大群成员类型
 */
typedef NS_ENUM(NSInteger, NIMSuperTeamMemberType){
    /**
     *  普通群员
     */
    NIMSuperTeamMemberTypeNormal = 0,
    /**
     *  群拥有者
     */
    NIMSuperTeamMemberTypeOwner = 1,
};


#endif /* NIMSuperTeamDefs_h */
