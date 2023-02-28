package com.example.aircraftwar_base.reward;

import com.example.aircraftwar_base.aircraft.AbstractAircraft;

public class PropCreator {

    private PropFactory propFactory;
    public void setPropFactory(PropFactory propFactory){
        this.propFactory = propFactory;
    }
    public AbstractReward getProp(AbstractAircraft abstractAircraft){
        AbstractReward prop = propFactory.creatReward(abstractAircraft);
        return prop;
    }
}