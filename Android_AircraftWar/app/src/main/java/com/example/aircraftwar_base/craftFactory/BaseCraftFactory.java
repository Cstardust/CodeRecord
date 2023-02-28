package com.example.aircraftwar_base.craftFactory;

import com.example.aircraftwar_base.aircraft.AbstractAircraft;
import com.example.aircraftwar_base.application.GameView;
import com.example.aircraftwar_base.controller.ImageManager;

//  飞机的工厂模式
public abstract class BaseCraftFactory {

    //  坐标、得分、子弹伤害、方向 使用默认数据。不让用户自定义了。怪麻烦的，也没啥视觉效果。
    //  createAircraft 的参数 是用户可以自定义的
    //  如果想交给用户，那么再改工厂的接口即可。
    protected int locationX = (int) ( Math.random() * (GameView.screenWidth - ImageManager.MOB_ENEMY_IMAGE.getWidth()))*1;
    protected int locationY = (int) (Math.random() * GameView.screenHeight * 0.2)*1-80;
    protected int direction ;
    protected int val;          //  击落得分
    protected int power ;
    public abstract AbstractAircraft createAircraft(int hp, int speedX, int speedY, int shootNum);
}

