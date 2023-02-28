package com.example.aircraftwar_base.aircraft;

import com.example.aircraftwar_base.basic.AbstractFlyingObject;
import com.example.aircraftwar_base.bullet.BaseBullet;


import java.util.List;

/**
 * 所有种类飞机的抽象父类：
 * 敌机（BOSS, ELITE, MOB），英雄飞机
 *
 * @author hitsz
 */
public abstract class AbstractAircraft extends AbstractFlyingObject {

    protected int maxHp;
    protected int hp;
    protected int val;          //  击落得分
    protected int shootNum;     //  子弹一次发射数量  暂时没用了 不知道以后是否该用
    protected int power;       //   子弹伤害
    protected int direction ;  //   子弹射击方向 (向上发射：1，向下发射：-1)

    public AbstractAircraft(int locationX, int locationY, int speedX, int speedY, int hp,int power,int direction,int val,int shootnum) {
        super(locationX, locationY, speedX, speedY);
        this.hp = hp;
        this.maxHp = hp;
        this.power = power;
        this.direction = direction;
        this.val = val;
        this.shootNum = shootnum;
    }

    public void decreaseHp(int decrease){
        hp -= decrease;
        if(hp <= 0){
            hp=0;
            vanish();
        }
    }

    public int getHp() {
        return hp;
    }


    /**
     * 飞机射击方法，可射击对象必须实现
     * @return
     *  可射击对象需实现，返回子弹
     *  非可射击对象空实现，返回null
     */
    public abstract List<BaseBullet> shoot();
    public int getShootNum()
    {
        return shootNum;
    }

    public int getPower(){
        return power;
    }
    public int getDirection(){
        return direction;
    }
    public int getVal(){return val;}
}


