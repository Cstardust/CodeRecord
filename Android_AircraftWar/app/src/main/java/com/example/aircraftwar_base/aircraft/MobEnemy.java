package com.example.aircraftwar_base.aircraft;

import com.example.aircraftwar_base.application.GameView;
import com.example.aircraftwar_base.bullet.BaseBullet;

import java.util.LinkedList;
import java.util.List;

public class MobEnemy extends AbstractAircraft {

    public MobEnemy(int locationX, int locationY, int speedX, int speedY, int hp,int power,int direction,int v,int sN) {
        super(locationX, locationY, speedX, speedY, hp,power,direction,v,sN);
    }


    @Override
    public void forward() {
        super.forward();
        // 判定 y 轴向下飞行出界
        if (locationY >= GameView.screenHeight) {
            vanish();
        }
    }

    @Override
    public List<BaseBullet> shoot() {
        return new LinkedList<>();
    }

}