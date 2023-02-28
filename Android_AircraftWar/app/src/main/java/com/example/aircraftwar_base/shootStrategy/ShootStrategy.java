package com.example.aircraftwar_base.shootStrategy;


import com.example.aircraftwar_base.aircraft.AbstractAircraft;
import com.example.aircraftwar_base.bullet.BaseBullet;

import java.util.List;

//对于每种发射策略，伤害power，方向direction，速度speedX，speedY，以及图片type由飞机类传参控制
public interface ShootStrategy {
    public List<BaseBullet> doShoot(AbstractAircraft air); //  ImageHeight是为了设置子弹位置
}
