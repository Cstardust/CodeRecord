package com.example.aircraftwar_base.reward;

import com.example.aircraftwar_base.aircraft.AbstractAircraft;

public interface PropFactory {

    public AbstractReward creatReward(AbstractAircraft abstractAircraft);

}
