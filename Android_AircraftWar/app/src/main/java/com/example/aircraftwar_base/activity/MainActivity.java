package com.example.aircraftwar_base.activity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.Switch;

import androidx.appcompat.app.AppCompatActivity;

import com.example.aircraftwar_base.R;
import com.example.aircraftwar_base.application.EasyGame;
import com.example.aircraftwar_base.application.GameView;
import com.example.aircraftwar_base.application.HardGame;
import com.example.aircraftwar_base.application.MediumGame;

import java.io.File;
import java.net.Socket;
import java.util.LinkedList;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    public String TAG = "aaaa";
    private GameView mGameView;
    public static List<Activity> activityList = new LinkedList();
    public static int choose = 0;
    private static boolean isMusic;     //  游戏是否有音乐
    private static Thread t = null;
    public static File fileDir= null;
    public static String mutex = "";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        MainActivity.activityList.add(this);
        fileDir = getApplication().getExternalCacheDir();
        Switch sw = (Switch)findViewById(R.id.switch1);
        //  添加监听
        sw.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                                          @Override
                                          public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                                              if(isChecked){
                                                  isMusic = true;
                                              }else{
                                                  isMusic = false;
                                              }
                                              System.out.println("isMusic " + isMusic);
                                          }
                                      }
        );
    }

    public void showEasyGameView(View v) {
        //  准备数据
        ScoreActivity.setMode("EASY");
        getScreenHW();
        //  切换界面
        if(ModeChooseActivity.isIsOnline()){
            Intent intent = new Intent(this, OnlineActivity.class);
            startActivity(intent);
            finish();
        } else{
            mGameView = new EasyGame(this);
            setContentView(mGameView);
//            finish();
        }

    }


    public void showMediumGameView(View v)
    {
        //  准备数据
        ScoreActivity.setMode("MEDIUM");
        getScreenHW();
        //  切换界面
        if(ModeChooseActivity.isIsOnline()){
            Intent intent = new Intent(this, OnlineActivity.class);
            startActivity(intent);
            finish();
        } else{
            mGameView = new MediumGame(this);
            setContentView(mGameView);
//            finish();
        }
//        getScreenHW();
//        mGameView = new MediumGame(this);
//        ScoreActivity.setMode("MEDIUM");
//        setContentView(mGameView);
    }

    public void showHardGameView(View v)
    {
        //  准备数据
        ScoreActivity.setMode("HARD");
        getScreenHW();
        //  切换界面
        if(ModeChooseActivity.isIsOnline()){
            Intent intent = new Intent(this, OnlineActivity.class);
            startActivity(intent);
//            finish();
        } else{
            mGameView = new HardGame(this);
            setContentView(mGameView);
//            finish();
        }
    }
//        getScreenHW();
//        ScoreActivity.setMode("HARD");
//        mGameView = new HardGame(this);
//        setContentView(mGameView);

    //  获取界面的长、宽
    public void getScreenHW()
    {
        //  定义DisplayMetric对象
        DisplayMetrics dm = new DisplayMetrics();
        //  取得窗口属性
        getWindowManager().getDefaultDisplay().getMetrics(dm);

        //  窗口宽度
        GameView.screenWidth = dm.widthPixels;
        Log.i(TAG,"screenWidth : "+ GameView.screenWidth);

        //  窗口高度
        GameView.screenHeight = dm.heightPixels;
        Log.i(TAG,"screenHeight : "+GameView.screenHeight);
    }

    public static boolean getIsMusic(){
        return isMusic;
    }

}
