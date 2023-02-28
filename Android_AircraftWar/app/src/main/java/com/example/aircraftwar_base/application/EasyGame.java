package com.example.aircraftwar_base.application;

import android.content.Context;
import android.graphics.BitmapFactory;

import com.example.aircraftwar_base.R;
import com.example.aircraftwar_base.controller.ImageManager;
import com.example.aircraftwar_base.craftFactory.EliteEnemyFactory;
import com.example.aircraftwar_base.craftFactory.MobEnemyFactory;

import java.util.LinkedHashMap;

public class EasyGame extends GameView{

    public EasyGame(Context context)
    {
        super(context);
    }

    @Override
    public void loadBG() {
        ImageManager.BACKGROUND_IMAGE = BitmapFactory.decodeResource(getResources(), R.drawable.bg2);
    }
    @Override
    protected void initArgs()
    {
        enemyMaxNumber = 5;

        elitesHp = 50;
        elitesX = 0;
        elitesY = 5;
        elitesNum = 1;

        mobHp = 30;
        mobsX = 0;
        mobsY = 10;
        mobsNum = 0;
        bossLimit = 0;

        shootDuration = new LinkedHashMap<String,Double>(){
            {
                put("hero",500.0);
                put("enemy",900.0);
            }
        };
        airDuration = new LinkedHashMap<String,Double>(){
            {
                put("normal",600.0);
                put("elite",3000.0);
            }
        };

    }

    @Override
    protected void loadAirCrafts(){
        // 周期性执行（控制频率）
        if (timeCountAndNewCycleJudge("normal")) {
            // 新敌机产生
            if (enemyAircrafts.size() < enemyMaxNumber) {
                enemyAircrafts.add(new MobEnemyFactory().createAircraft(mobHp,mobsX,mobsY,mobsNum));
            }
        }
        else if(timeCountAndNewCycleJudge("elite"))
        {
            // 新敌机产生
            if (enemyAircrafts.size() < enemyMaxNumber) {
                enemyAircrafts.add(new EliteEnemyFactory().createAircraft(elitesHp,elitesX,elitesY,elitesNum));
            }
        }
    }

    @Override
    protected void levelUp() {

    }
}