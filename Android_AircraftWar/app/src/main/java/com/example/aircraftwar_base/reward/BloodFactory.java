package com.example.aircraftwar_base.reward;

import com.example.aircraftwar_base.aircraft.AbstractAircraft;

public class BloodFactory implements PropFactory {
    public AbstractReward creatReward(AbstractAircraft abstractAircraft)
    {
        return new BloodReward(abstractAircraft.getLocationX(),abstractAircraft.getLocationY(),0,abstractAircraft.getSpeedY());
    }
}
