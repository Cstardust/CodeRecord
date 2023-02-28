package com.example.aircraftwar_base.reward;

import com.example.aircraftwar_base.aircraft.HeroAircraft;
import com.example.aircraftwar_base.shootStrategy.ScatteredShoot;
import com.example.aircraftwar_base.shootStrategy.StraightShoot;


public class BulletReward extends AbstractReward {
    private volatile static int validTime = 0;              //  累计时间清0
    private static Thread t = null;
    public void takeEffect(HeroAircraft heroAircraft) {
        //  6s后恢复
        Runnable r = ()->{
            heroAircraft.setStrategy(new ScatteredShoot());
            try {
                for(int i=0;i<validTime;++i){
                    Thread.sleep(1000*3);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            heroAircraft.setStrategy(new StraightShoot());
            validTime = 0;                      //  累计时间清0
        };
        if(t==null||!t.isAlive()){
            ++validTime;
            t = new Thread(r,"bullet_reward thread");
            t.start();
        }


    }

    public BulletReward(int locationX, int locationY, int speedX, int speedY) {
        super(locationX, locationY, speedX, speedY);
    }

}
