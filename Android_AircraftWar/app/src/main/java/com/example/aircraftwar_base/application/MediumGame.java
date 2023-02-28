package com.example.aircraftwar_base.application;

import static java.lang.Math.max;
import static java.lang.Math.min;

import android.content.Context;
import android.graphics.BitmapFactory;

import com.example.aircraftwar_base.R;
import com.example.aircraftwar_base.controller.ImageManager;
import com.example.aircraftwar_base.craftFactory.BossEnemyFactory;
import com.example.aircraftwar_base.craftFactory.EliteEnemyFactory;
import com.example.aircraftwar_base.craftFactory.MobEnemyFactory;

import java.util.LinkedHashMap;

public class MediumGame extends GameView{
    public MediumGame(Context context)
    {
        super(context);
    }

    @Override
    protected void loadBG() {
        ImageManager.BACKGROUND_IMAGE = BitmapFactory.decodeResource(getResources(), R.drawable.bg4);
    }
    @Override
    protected void initArgs(){
        enemyMaxNumber = 10;

        elitesHp = 60;
        elitesX = 5;
        elitesY = 5;
        elitesNum = 1;

        mobHp = 40;
        mobsX = 0;
        mobsY = 10;
        mobsNum = 0;

        bossThreshold = 500;
        bossHp = 500;
        bosssX = 2;
        bosssY = 0;
        bossNum = 3;
        bossLimit = 1;
        levelTime = 15000;

        shootDuration = new LinkedHashMap<String,Double>(){
            {
                put("hero",600.0);
                put("enemy",700.0);
            }
        };
        airDuration = new LinkedHashMap<String,Double>(){
            {
                put("normal",600.0);
                put("elite",2000.0);
            }
        };
    }

    @Override
    protected void loadAirCrafts() {
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

        if(isBoss()){
            if(enemyAircrafts.size()<enemyMaxNumber){
                enemyAircrafts.add(new BossEnemyFactory().createAircraft(bossHp,bosssX,bosssY,bossNum));
                if(isIsmusicON()){
                    bossBgm.start();
                }
            }
        }
    }

    @Override
    protected void levelUp() {
        if(totalTime%levelTime==0 || totalTime>levelTime){
            System.out.println("难度升高");
            System.out.println("界面内最大敌机数量+1上升至"+enemyMaxNumber);
            System.out.println("普通敌机出现周期缩短10% ; 纵向速度+1,上限为15");
            System.out.println("精英飞机血量上限+10上升至" +elitesHp + " ; 精英敌机出现周期缩短10%" +  " ; 精英机发射子弹速度加快2%" + " ; 精英机子弹数量+1,上限为3" + "横向速度+1,上限为3");
            System.out.println("英雄伤害+2 ; 英雄机发射速度加快5%" );
            System.out.println("Boss机出现的得分阈值降低10，下限为200");
            System.out.println();
            totalTime /= levelTime;
            //  飞机数量
            enemyMaxNumber += 1;

            //  mob出现周期
            airDuration.put("normal",airDuration.get("normal")*0.9);
            mobsY = min(mobHp+1,15);

            //  精英飞机
            elitesHp += 10;
            elitesNum = min(3,elitesNum+1);
            elitesX = min(elitesX+1,3);
            airDuration.put("elite",airDuration.get("elite")*0.9);
            shootDuration.put("enemy",shootDuration.get("enemy")*0.98);

            //  英雄机
            heroAircraft.addPower(2);
            shootDuration.put("hero",shootDuration.get("hero")*0.95);
            if(heroAircraft.getNum()<2){
                heroAircraft.addNum(1);
            }
            //  Boss机阈值
            bossThreshold = max(bossThreshold-10,300);
        }
    }
}