package com.example.aircraftwar_base.controller;

import android.view.MotionEvent;
import android.view.View;


import com.example.aircraftwar_base.aircraft.HeroAircraft;
import com.example.aircraftwar_base.application.GameView;

/**
 * 英雄机控制类
 * 监听鼠标，控制英雄机的移动
 *
 * @author hitsz
 */
import android.annotation.SuppressLint;

/**
 * 英雄机控制类
 * 监听鼠标，控制英雄机的移动
 *
 * @author hitsz
 */
/**
 * 英雄机控制类
 * 监听鼠标，控制英雄机的移动
 *
 * @author hitsz
 */
public class HeroController{
    private GameView mGameView;
    private HeroAircraft heroAircraft;
    private boolean selectFlag = false;
    public HeroController(GameView gameView, HeroAircraft heroAircraft)
    {

        this.mGameView = gameView;
        this.heroAircraft = heroAircraft;
        setGestureListener();   //  监听鼠标位置，并实时改变英雄机位置
    }
    /**
     * 实时监听鼠标位置
     * @author
     */
    @SuppressLint("ClickableViewAccessibility")
    private void setGestureListener() {
        mGameView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                // TODO Auto-generated method stub
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        selectFlag = isSelect(event);
                        break;
                    case MotionEvent.ACTION_MOVE:
                        if ( event.getX()<0 || event.getX()> GameView.screenWidth || event.getY()+heroAircraft.getImage().getHeight()/2<0 ||
                                event.getY()>GameView.screenHeight){

                        }else{
                            if(selectFlag){
                                heroAircraft.setLocation(event.getX(),event.getY()+heroAircraft.getImage().getHeight()/2);
                            }
                        }
                        break;
                    case MotionEvent.ACTION_UP:
                        selectFlag = false;
                        break;
                    case MotionEvent.ACTION_HOVER_MOVE:
                        break;
                }
                return true;
            }
        });
    }

    public boolean isSelect(MotionEvent event){
        int hx = heroAircraft.getLocationX();
        int hy = heroAircraft.getLocationY();
        int hWidth = heroAircraft.getWidth();
        int hHeight = heroAircraft.getHeight();

        return hx + (hWidth+0)/2 >= event.getX()
                && hx - (hWidth+0)/2 <= event.getX()
                && hy + ( hHeight/2+0/2 )/2 >= event.getY()+heroAircraft.getImage().getHeight()/2
                && hy - ( hHeight/2+0/2 )/2 <= event.getY()+heroAircraft.getImage().getHeight()/2;

    }

}
