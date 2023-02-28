package com.example.aircraftwar_base.aircraft;


import android.util.Log;

import com.example.aircraftwar_base.bullet.BaseBullet;
import com.example.aircraftwar_base.bullet.HeroBullet;
import com.example.aircraftwar_base.shootStrategy.MyContext;
import com.example.aircraftwar_base.shootStrategy.ShootStrategy;
import com.example.aircraftwar_base.shootStrategy.StraightShoot;

import java.util.LinkedList;
import java.util.List;

/**
 * 英雄飞机，游戏玩家操控
 * @author hitsz
 */
public class HeroAircraft extends AbstractAircraft {
    //  默认直射
    private MyContext c = new MyContext(new StraightShoot());
    /** 单例模式 */
    private volatile static HeroAircraft hero;  //  volatile 防止发生reorder问题
    private HeroAircraft(int locationX, int locationY, int speedX, int speedY, int hp,int power,int direction,int shootNum){
        super(locationX,locationY,speedX,speedY,hp,power,direction,0,shootNum);
    }
    public static HeroAircraft getHeroAircraft(int locationX, int locationY, int speedX, int speedY, int hp, int power, int direction, int shootNum){
        if(hero==null){ //  上锁
            synchronized (HeroAircraft.class){
                if(hero==null){
                    hero = new HeroAircraft(locationX,locationY,speedX,speedY,hp,power,direction,shootNum);
                }
            }
        }
        return hero;
    }

    @Override
    public void forward() {
        // 英雄机由鼠标控制，不通过forward函数移动
    }

    @Override
    /**
     * 通过射击产生子弹
     * @return 射击出的子弹List
     */
    public List<BaseBullet> shoot() {
        if(!isValid) {
            return null;
        }
        return c.executeStrategy(this);
    }

    public void setStrategy(ShootStrategy s)
    {
        //  之后会execute这个策略
        c.setStrategy(s);
    }

    public void addHp(int x)
    {
        hp += x;
    }
    public void addPower(int x){
        power+=x;
    }
    public int getNum(){return shootNum;}
    public void addNum(int x){
        shootNum += x;
    }

}
