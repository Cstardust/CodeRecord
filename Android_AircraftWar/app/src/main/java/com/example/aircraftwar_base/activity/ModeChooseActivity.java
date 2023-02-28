package com.example.aircraftwar_base.activity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.example.aircraftwar_base.R;
import com.example.aircraftwar_base.application.EasyGame;
import com.example.aircraftwar_base.application.GameView;
import com.example.aircraftwar_base.client.Client;

import java.io.PrintWriter;
import java.net.Socket;

public class ModeChooseActivity extends AppCompatActivity {

    private String content = "";
    private Socket socket =LoginActivity.getSocket();
    private PrintWriter writer = LoginActivity.writer;
    public static boolean battleFlag = false;
    private Client client;  //  用的都是LogInActivity中的

    private static boolean isOnline = false;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_modechoose);
        MainActivity.activityList.add(this);
        Button standAloneButton = findViewById(R.id.standAloneButton);
        Button onLineButton = findViewById(R.id.onlineButton);
        client = LoginActivity.getClient();
        //单机模式
        standAloneButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(ModeChooseActivity.this, MainActivity.class);
                isOnline = false;
                startActivity(intent);
                finish();
            }
        });
        onLineButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ModeChooseActivity.this, MainActivity.class);
                isOnline = true;
                startActivity(intent);
                finish();
            }
        });
    }

    private void matchSuccess(){
        GameView gameView = new EasyGame(this);
        battleFlag = true;
        setContentView(gameView);
    }
    private void matchFailed(){
        Toast.makeText(ModeChooseActivity.this, "匹配失败！", Toast.LENGTH_LONG).show();
    }


    public static boolean isIsOnline() {
        return isOnline;
    }

}


