/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2016 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LVL_PLAYER_H
#define LVL_PLAYER_H

#include "lvl_camera.h"
#include "lvl_warp.h"
#include "lvl_npc.h"

#include "lvl_base_object.h"
#include <PGE_File_Formats/file_formats.h>
#include <controls/controllable_object.h>
#include <data_configs/obj_player.h>
#include <common_features/matrix_animator.h>
#include <common_features/point_mover.h>
#include <common_features/event_queue.h>
#include <common_features/data_array.h>

#include <QHash>
#include <QVector>

class LVL_Player_harm_event
{
public:
    LVL_Player_harm_event();
    bool    doHarm;
    int     doHarm_damage;
};

class LVL_Section;
class LVL_Block;
class lua_LevelPlayerState;

class LVL_Player :
    public PGE_Phys_Object,
    public ControllableObject
{
public:
    LVL_Player(LevelScene *_parent = NULL);
    virtual ~LVL_Player();
    bool setCharacter(unsigned long CharacterID, unsigned long _stateID);
    void setCharacterSafe(unsigned long CharacterID, unsigned long _stateID);
private:
    bool _doSafeSwitchCharacter;
public:
    void setPlayerPointInfo(PlayerPoint pt);
    bool init();
    void update(double tickTime);
    void updateCamera();

    void refreshEnvironmentState();

    void processContacts();
    void preCollision();
    void postCollision();
    void collisionHitBlockTop(std::vector<PGE_Phys_Object *> &blocksHit);
    bool preCollisionCheck(PGE_Phys_Object *body);

    bool forceCollideCenter;//!< collide with invizible blocks at center
    double _heightDelta; //Delta of changing height. Need to protect going through block on character switching

    /*****************NPC's and blocks******************/
    typedef QHash<int, PGE_Phys_Object *> PlayerColliders;
    LVL_Npc   *collided_talkable_npc;
    bool _stucked;
    /***************************************************/

    int playerID;
    obj_player setup;
    PlayerPoint data;
    PGE_LevelCamera *camera; //!< Connected camera

    void teleport(double x, double y);
    void exitFromLevel(QString levelFile, unsigned long targetWarp, long wX = -1, long wY = -1);

    PGE_DataArray<obj_player_state > states;
    unsigned long   characterID;
    unsigned long   stateID;
    obj_player_state state_cur;


    enum kill_npc_reasons
    {
        NPC_Unknown = -1,
        NPC_Stomped = 0,
        NPC_Kicked,
        NPC_Taked_Coin,
        NPC_Taked_Powerup
    };

    void kill_npc(LVL_Npc *target, kill_npc_reasons reason);
    //QQueue<LVL_Npc*> npc_queue;

    /*******************Environmept*********************/
    PGE_DataArray<obj_player_physics > physics;
    QHash<int, int> environments_map;

    obj_player_physics physics_cur;
    int     environment;
    int     last_environment;
    /*******************Environmept*********************/

    /*******************Motion*************************/
    bool isRunning();
    bool    _isRunning;

    int  direction();
    int     _direction;
    /*******************Motion*************************/

    /*******************Life and Death*****************/
    bool isAlive;
    enum deathReason
    {
        DEAD_fall = 0,
        DEAD_burn,
        DEAD_killed
    };
    void    kill(deathReason reason = DEAD_killed);
    void    unregister();

    bool    doKill;
    deathReason kill_reason;

    int     health;
    bool    invincible;
    float   invincible_delay;
    bool    blink_screen;
    bool    blink_screen_state;
    void    harm(int _damage = 1);
    void    setInvincible(bool state, float delay, bool enableScreenBlink = false);
    /*******************Life and Death*****************/

    double  gscale_Backup; //!< BackUP of last gravity scale

    /********************Jumps***************************/
    bool    JumpPressed;
    bool    onGround();
    QHash<intptr_t, PGE_Phys_Object * > foot_contacts_map;  //!< staying on ground surfaces
    QHash<intptr_t, PGE_Phys_Object * > foot_sl_contacts_map; //!< Slipery surfaces
    double  jumpTime;
    double  jumpVelocity;
    /********************Jumps***************************/

    /********************Bump***************************/
    bool    bumpDown;
    bool    bumpUp;
    double  bumpVelocity;//!< down velocity
    double  bumpJumpVelocity;//! Up jump velocity
    int     bumpJumpTime;    //!< Up jump time
    void    bump(bool _up = false, double bounceSpeed = 0.0, int timeToJump = 0);
    inline void bumpf(bool _up, float bounceSpeed = 0.0f, int timeToJump = 0)
    {
        bump(_up, static_cast<double>(bounceSpeed), timeToJump);
    }
    /********************Bump***************************/

    /********************Climbing***************************/
    QHash<intptr_t, PGE_Phys_Object * > climbable_map;
    bool        climbing;
    double      climbableHeight;
    /********************Climbing***************************/

    /*******************Warps*********************/
    PointMover m_cameraMover;
    bool    contactedWithWarp;
    LVL_Warp *contactedWarp;
    bool    wasEntered;
    int     wasEnteredTimeout;
    bool    isWarping;

    int     warpDirectO;
    double  warpPipeOffset;
    double  warpFrameW;
    double  warpFrameH;

    EventQueue<LVL_Player > event_queue;
    void    processWarpChecking();
    void    WarpTo(double x, double y, int warpType, int warpDirection = 1, bool cannon = false, double cannon_speed = 10.0);
    void    WarpTo(LevelDoor warp);
    /*******************Warps*********************/\

    /******************floating*******************/
    bool    floating_allow;
    bool    floating_isworks;
    double  floating_timer;
    double  floating_maxtime;
    bool    floating_start_type;//!< true= sin(time), false= cos(time)
    /******************floating*******************/

    /******************Attack*******************/
    /*This feature will provide teporary ability to break any nearest blocks*/
    enum AttackDirection
    {
        Attack_Forward = 0,
        Attack_Up,
        Attack_Down
    };
    bool    attack_enabled;
    bool    attack_pressed;
    void    attack(AttackDirection _dir);
    /******************Attack*******************/

    /************************************************
                                __
                              /` ,\__
                             |    ).-'
                            / .--'
                           / /
             ,      _.==''`  \
           .'(  _.='         |
          {   ``  _.='       |
           {    \`     ;    /
            `.   `'=..'  .='
              `=._    .='
           duck '-`\\`__
                    `-._{
    ************************************************/
    bool duck_allow;
    bool ducking;
    void setDuck(bool duck);
    /******************Duck*************************/
public:
    void render(double camX, double camY);
    MatrixAnimator animator;
    int frameW;
    int frameH;
    bool locked();
    void setLocked(bool lock);
    bool isExiting;
    int  _exiting_swimTimer;

    /********************Lua Stuff*******************
                        .-""""-
                       F   .-'
                      F   J
                     I    I
                      L   `.
                       L    `-._,
                        `-.__.-'
     ***********************************************/

    lua_LevelPlayerState *global_state;

    /*Controller processing*/
    void lua_processKeyEvents();
    void lua_updateKey(bool &target_key, ControllableObject::KeyType ktype, bool &state);
    controller_keys keys_prev;
    inline bool  lua_getKeyState(int keyType)
    {
        return getKeyState(keyType);
    }
    virtual void lua_onKeyPressed(ControllableObject::KeyType) {}
    virtual void lua_onKeyReleased(ControllableObject::KeyType) {}
    /*---------------------*/

    virtual void lua_onInit() {}
    virtual void lua_onLoop(double) {}
    virtual void lua_onHarm(LVL_Player_harm_event *) {}
    virtual void lua_onTransform(unsigned long, unsigned long) {}
    virtual void lua_onTakeNpc(LVL_Npc *) {}
    virtual void lua_onKillNpc(LVL_Npc *) {}
    inline void lua_playAnimationOnce(int animationID,
                                      int speed,
                                      bool fixed_speed,
                                      bool locked,
                                      int skipLastFrames)
    {
        animator.playOnce(static_cast<MatrixAnimator::MatrixAnimates>(animationID),
                          _direction, speed, fixed_speed,
                          locked, skipLastFrames);
    }
    inline void lua_setAnimation(int animationID, int framespeed)
    {
        animator.switchAnimation(static_cast<MatrixAnimator::MatrixAnimates>(animationID),
                                 _direction,
                                 framespeed);
    }
    inline long getHealth()
    {
        return health;
    }
    void setHealth(int _health);
    inline void setCharacterID(unsigned long _character)
    {
        setCharacterSafe(_character, stateID);
    }
    inline void setState(unsigned long _state)
    {
        setCharacterSafe(characterID, _state);
    }
    LVL_Npc *lua_spawnNPC(unsigned long npcID, int sp_type, int sp_dir, bool reSpawnable);
    bool isLuaPlayer;
    /********************Lua Stuff******************/

    bool isInited();

private:
    bool _no_render;
    bool isLocked;
    bool m_isInited;
    void refreshAnimation();
};

#endif // LVL_PLAYER_H
