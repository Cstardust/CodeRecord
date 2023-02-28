package com.example.aircraftwar_base.craftFactory;

import com.example.aircraftwar_base.aircraft.AbstractAircraft;
import com.example.aircraftwar_base.aircraft.EliteEnemy;

public class EliteEnemyFactory extends BaseCraftFactory {
    /** 攻击方式 */
    @Override
    public AbstractAircraft createAircraft(int hp, int speedX, int speedY, int shootNum) {
        val = 50;
        power = 30;
        direction = 1;
        return new EliteEnemy(locationX,locationY,speedX,speedY,hp,power,direction,val,shootNum);
    }
}