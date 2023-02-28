package com.example.aircraftwar_base.reward;

import com.example.aircraftwar_base.aircraft.AbstractAircraft;

public class BombFactory implements PropFactory{

    public AbstractReward creatReward(AbstractAircraft abstractAircraft)
    {
        return new BombReward(abstractAircraft.getLocationX(),abstractAircraft.getLocationY(),0,abstractAircraft.getSpeedY());
    }
}
