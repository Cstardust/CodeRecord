package com.example.aircraftwar_base.aircraft;

import android.util.Log;

import com.example.aircraftwar_base.bullet.BaseBullet;
import com.example.aircraftwar_base.shootStrategy.MyContext;
import com.example.aircraftwar_base.shootStrategy.ScatteredShoot;

import java.util.List;

//  boss出现时有背景音乐。所以这里面本来也有关于启动音乐线程的代码、
//  想了想还是只注释不删掉。如果觉得我的代码让人比较困惑可以教练可以参考一下哈哈哈。
public class BossEnemy extends AbstractAircraft {

    /** 攻击方式 */
    private MyContext c = new MyContext(new ScatteredShoot());
//    private MusicThread mt;
    public BossEnemy(int locationX, int locationY, int speedX, int speedY, int hp,int power,int direction,int val,int shootNum) {
        super(locationX, locationY, speedX, speedY, hp,power,direction,val,shootNum);
//        if(MusicThread.getIsMusic()){
//            mt = new MusicThread("src/videos/bgm_boss.wav",false,false);
//            mt.start();
//        }
    }

    @Override
    public List<BaseBullet> shoot() {
        if(!isValid) {
            return null;
        }
        Log.d("Boss","SHOOT");
        return c.executeStrategy(this);
    }

    @Override
    public void vanish() {
        isValid = false;
        System.out.println("BOSS " + isValid);
    }

}
