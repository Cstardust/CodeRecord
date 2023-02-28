package com.example.aircraftwar_base.reward;


import android.media.MediaPlayer;

import com.example.aircraftwar_base.R;
import com.example.aircraftwar_base.aircraft.AbstractAircraft;
import com.example.aircraftwar_base.aircraft.BossEnemy;
import com.example.aircraftwar_base.aircraft.EliteEnemy;
import com.example.aircraftwar_base.aircraft.HeroAircraft;
import com.example.aircraftwar_base.aircraft.MobEnemy;
import com.example.aircraftwar_base.bullet.BaseBullet;

import java.util.ArrayList;
import java.util.List;

/*
道具：炸药包
 */
public class BombReward extends AbstractReward {

    public void takeEffect(List<AbstractAircraft> enemys, List<BaseBullet> bullets) {
        for(AbstractAircraft enemy : enemys)
        {

            if(enemy instanceof EliteEnemy)
                enemy.vanish();
            if(enemy instanceof MobEnemy)
                enemy.vanish();
            if(enemy instanceof BossEnemy)
                enemy.decreaseHp(100);

        }
        for(BaseBullet bullet : bullets)
        {
            bullet.vanish();
        }

    }

    public BombReward(int locationX, int locationY, int speedX, int speedY) {
        super(locationX, locationY, speedX, speedY);
    }

}

