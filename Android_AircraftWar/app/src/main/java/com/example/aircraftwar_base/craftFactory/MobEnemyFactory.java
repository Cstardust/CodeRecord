package com.example.aircraftwar_base.craftFactory;

import com.example.aircraftwar_base.aircraft.AbstractAircraft;
import com.example.aircraftwar_base.aircraft.MobEnemy;

public class MobEnemyFactory extends BaseCraftFactory{

    @Override
    public AbstractAircraft createAircraft(int hp, int speedX, int speedY, int shootNum) {
        val = 20;
        direction = 1;
        power = 0;
        return new MobEnemy(locationX,locationY,speedX,speedY,hp,power,direction,val,shootNum);
    }
}
