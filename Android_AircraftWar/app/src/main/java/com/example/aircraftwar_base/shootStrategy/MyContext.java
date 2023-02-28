package com.example.aircraftwar_base.shootStrategy;

import com.example.aircraftwar_base.aircraft.AbstractAircraft;
import com.example.aircraftwar_base.bullet.BaseBullet;

import java.util.List;

//  策略模式的上下文类
public class MyContext {
    private ShootStrategy strategy;
    public MyContext(ShootStrategy s) {
        strategy = s;
    }
    public void setStrategy(ShootStrategy s)
    {
        strategy = s;
    }
    public List<BaseBullet> executeStrategy(AbstractAircraft air)
    {
        return strategy.doShoot(air);
    }
}
