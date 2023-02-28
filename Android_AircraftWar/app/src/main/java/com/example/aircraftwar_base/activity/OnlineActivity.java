package com.example.aircraftwar_base.activity;

import static java.lang.Thread.sleep;

import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import com.example.aircraftwar_base.R;
import com.example.aircraftwar_base.application.EasyGame;
import com.example.aircraftwar_base.application.GameView;
import com.example.aircraftwar_base.application.HardGame;
import com.example.aircraftwar_base.application.MediumGame;
import com.example.aircraftwar_base.client.Client;

import org.w3c.dom.ls.LSOutput;

import java.net.Socket;

public class OnlineActivity extends AppCompatActivity {
    private Client client;
    private GameView mGameView;
    ProgressDialog waitingDialog;
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.online_menu);

        Button bt1 = findViewById(R.id.createRoom);
        Button bt2 = findViewById(R.id.joinRoom);
        client = LoginActivity.getClient();
        //  创建房间
        bt1.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                final EditText editText = new EditText(OnlineActivity.this);
                AlertDialog.Builder inputDialog =
                        new AlertDialog.Builder(OnlineActivity.this);
                inputDialog.setTitle("Please create a room：Input the Room name").setView(editText);
                inputDialog.setPositiveButton("OK",
                        new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                String roomName = editText.getText().toString();
                                showWaitingDialog();
                                System.out.println("MY_CLIENT AFTER DIALOG");
                                new Thread(new Runnable() {
                                    @Override
                                    public void run() {
                                        client.createRoom(roomName);
                                        client.waitRival();
                                        dismissDialog();
                                    }
                                }).start();

                            }
                        }).show();
            }
        });
        //  加入房间
        bt2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                /*@setView 装入一个EditView
                 */
                final EditText editText = new EditText(OnlineActivity.this);
                AlertDialog.Builder inputDialog =
                        new AlertDialog.Builder(OnlineActivity.this);
                inputDialog.setTitle("Join a room ; Input the Room name").setView(editText);
                inputDialog.setPositiveButton("确定",
                        new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                String roomName = editText.getText().toString();
                                //  阻塞等待对手加入
                                boolean res = client.joinRoom(roomName);
                                System.out.println("CONNECT " + res);
                                //  结束等待
                                //此时已在主线程中，可以更新UI了
                                switch(ScoreActivity.getMode()){
                                    case "EASY":
                                    {
                                        mGameView = new EasyGame(OnlineActivity.this);
                                        break;
                                    }
                                    case "MEDIUM":
                                    {
                                        mGameView = new MediumGame(OnlineActivity.this);
                                        break;
                                    }
                                    case "HARD":
                                    {
                                        mGameView = new HardGame(OnlineActivity.this);
                                        break;
                                    }
                                    default:
                                        System.out.println("MY_CLIENT UNKNOWN");
                                        break;
                                }
                                setContentView(mGameView);
//                                finish();
                            }
                        }).show();
            }
        });
    }


    private void showWaitingDialog() {
        /* 等待Dialog具有屏蔽其他控件的交互能力
         * @setCancelable 为使屏幕不可点击，设置为不可取消(false)
         * 下载等事件完成后，主动调用函数关闭该Dialog
         */
        waitingDialog=
                new ProgressDialog(OnlineActivity.this);
        waitingDialog.setTitle("waiting for your friend");
        waitingDialog.setMessage("waitng");
        waitingDialog.setIndeterminate(true);
        waitingDialog.setCancelable(false);
        waitingDialog.show();
    }

    //  结束等待 并进入游戏
    public void dismissDialog()
    {
        waitingDialog.dismiss();
        (OnlineActivity.this).runOnUiThread(new Runnable() {
            @Override
            public void run() {
                //此时已在主线程中，可以更新UI了
                switch(ScoreActivity.getMode()){
                    case "EASY":
                    {
                        mGameView = new EasyGame(OnlineActivity.this);
                        break;
                    }
                    case "MEDIUM":
                    {
                        mGameView = new MediumGame(OnlineActivity.this);
                        break;
                    }
                    case "HARD":
                    {
                        mGameView = new HardGame(OnlineActivity.this);
                        break;
                    }
                    default:
                        System.out.println("MY_CLIENT UNKNOWN");
                        break;
                }
                setContentView(mGameView);
            }
        });

    }
}
