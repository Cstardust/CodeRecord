package com.example.aircraftwar_base.reward;

import com.example.aircraftwar_base.aircraft.HeroAircraft;
import com.example.aircraftwar_base.reward.AbstractReward;

/*
道具：血包
作用：加血
 */
public class BloodReward extends AbstractReward {
    private int adds = 80;
    public void takeEffect(HeroAircraft heroAircraft) {
        heroAircraft.addHp(adds);
    }

    public BloodReward(int locationX, int locationY, int speedX, int speedY) {
        super(locationX, locationY, speedX, speedY);
    }
}
