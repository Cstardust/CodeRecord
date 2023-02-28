package com.example.aircraftwar_base.shootStrategy;



import java.util.LinkedList;
import java.util.List;

import static java.lang.Math.max;

import android.util.Log;

import com.example.aircraftwar_base.aircraft.AbstractAircraft;
import com.example.aircraftwar_base.aircraft.BossEnemy;
import com.example.aircraftwar_base.aircraft.EliteEnemy;
import com.example.aircraftwar_base.aircraft.HeroAircraft;
import com.example.aircraftwar_base.bullet.BaseBullet;
import com.example.aircraftwar_base.bullet.EnemyBullet;
import com.example.aircraftwar_base.bullet.HeroBullet;

public class ScatteredShoot implements ShootStrategy{
//    private int shootNum = 3;   //  目前仅支持3
    private int power = 0;
    private int direction = 0;
    private int x = 0;
    private int y = 0;
    private int speedX = 0;
    private int speedY = 0;
    private int shootNum = 0;
    int dx[]={-2,0,2};

    @Override
    public List<BaseBullet> doShoot(AbstractAircraft air) {
        power = air.getPower();
        x = air.getLocationX();
        y = air.getLocationY() + air.getImage().getHeight()*direction;
        direction = air.getDirection();
        speedX = air.getSpeedX();
        speedY = air.getSpeedY()+direction*5;
        shootNum = max(3,air.getShootNum());

        List<BaseBullet> res = new LinkedList<>();
        for(int i=0; i<shootNum; i++){
            // 子弹发射位置相对飞机位置向前偏移
            // 多个子弹横向分散
            BaseBullet abstractBullet = null;
            if(air instanceof HeroAircraft)
            {
                abstractBullet = new HeroBullet(x + (i*2 - shootNum + 1)*20, y, speedX+dx[i%3], speedY, power);
            }
            else if(air instanceof BossEnemy || air instanceof EliteEnemy)
            {
                abstractBullet = new EnemyBullet(x + (i*2 - shootNum + 1)*20, y, speedX+dx[i%3], speedY, power);
            }else{
                Log.d("ScatteredShoot","sth else happened");
            }
            res.add(abstractBullet);
        }
        return res;
    }
}
