package com.example.aircraftwar_base.craftFactory;

import com.example.aircraftwar_base.aircraft.AbstractAircraft;
import com.example.aircraftwar_base.aircraft.BossEnemy;

public class BossEnemyFactory extends BaseCraftFactory {

    /** 攻击方式 */
    @Override
    public AbstractAircraft createAircraft(int hp, int speedX, int speedY, int shootNum) {
        direction = 1;
        val = 90;
        power = 35;
        return new BossEnemy(locationX,locationY,speedX,speedY,hp,power,direction,val,shootNum);
    }
}
