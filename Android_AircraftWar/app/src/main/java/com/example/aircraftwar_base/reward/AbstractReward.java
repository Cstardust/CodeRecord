package com.example.aircraftwar_base.reward;

import com.example.aircraftwar_base.aircraft.HeroAircraft;
import com.example.aircraftwar_base.application.GameView;
import com.example.aircraftwar_base.basic.AbstractFlyingObject;

/*
道具类
*/

//  我把这几个道具类建立起来 但是 没写方法
//  因为要帮你把这几个道具类的图片资源准备好。所以先建立个类名，没具体写方法、
public abstract class AbstractReward extends AbstractFlyingObject {

    //  道具作用
    // public abstract void takeEffect(HeroAircraft heroAircraft);

    public AbstractReward(int locationX, int locationY, int speedX, int speedY) {
        super(locationX, locationY, speedX, speedY);
    }

    @Override
    public void forward() {
        super.forward();

        // 判定 x 轴出界
        if (locationX <= 0 || locationX >= GameView.screenWidth) {
            vanish();
        }

        // 判定 y 轴出界
        if (speedY > 0 && locationY >= GameView.screenHeight ) {
            // 向下飞行出界
            vanish();
        }else if (locationY <= 0){
            // 向上飞行出界
            vanish();
        }
    }

}
