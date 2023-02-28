package com.example.aircraftwar_base.activity;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.example.aircraftwar_base.R;
import com.example.aircraftwar_base.application.GameView;
import com.example.aircraftwar_base.Player.Player;
import com.example.aircraftwar_base.client.Client;
import com.example.aircraftwar_base.db.DBOpenHelper;


import java.util.LinkedList;
import java.util.List;



public class ScoreActivity extends AppCompatActivity {
    private final int WC = ViewGroup.LayoutParams.WRAP_CONTENT;
    private final int FP = ViewGroup.LayoutParams.WRAP_CONTENT;
    private static String mode;
    private static String this_user_name = null;
    private Button bt;
    private Button[] button;//先声明按钮数组
    private LinearLayout linearLayout;
    //  players：所有游戏记录
    private static List<Player> players = new LinkedList<Player>();
//    private DBOpenHelper mDBOpenHelper;
    private static boolean flag = false;
    private static Client client = LoginActivity.getClient();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_rank);
        MainActivity.activityList.add(this);

//        mDBOpenHelper = new DBOpenHelper(this);
        if(!flag){
//            mDBOpenHelper.addRecord(this_user_name,
//                    Integer.valueOf(GameView.getScore()).toString(),
//                    mode);
            flag = true;
            //  游戏记录
            players = client.getAllRecords();
        }

        button = new Button[players.size()];//先声明按钮数组
        for (int i = 0; i <players.size(); i++) {//循环实例化按钮，并添加到布局中
            button[i] = new Button(this);
            button[i].setText("删除");
            button[i].setId(i);                         //注意，这里是关键，我们要手动的设置id给按钮，不然这个按钮返回的id为-1.
            button[i].setOnClickListener(new MyClick());
        }
        createTable();
    }


    class MyClick implements View.OnClickListener{
        @Override
        public void onClick(View v) {
            for (int i = 0; i < button.length; i++) {
                if (v.getId() == button[i].getId()) {   //这里获取的id就不会错啦。
                    System.out.println("MY_CLIENT " + players.get(i).getName() +players.get(i).getDate());
//                    mDBOpenHelper.deleteRecord(players.get(i).getName(),players.get(i).getDate());
                    client.reqDelete(players.get(i).getName(),players.get(i).getDate(),mode);
                    players.remove(i);
                    flush();
                }
            }
        }
    }


    public void flush() {
        Intent intent = new Intent(this, ScoreActivity.class);
        startActivity(intent);
        ScoreActivity.this.finish();
    }



    public void remove(View v) {
        TableLayout tableLayout = (TableLayout) findViewById(R.id.id_tableLayout);
        //获取TableLayout中的行数
        int len = tableLayout.getChildCount();
        tableLayout.removeView(tableLayout.getChildAt(len));
    }

    public void createTable() {
        TableLayout tableLayout = (TableLayout) findViewById(R.id.id_tableLayout);
        tableLayout.setStretchAllColumns(true);     //设置指定列号的列属性为Stretchable
        for(int i=0;i<players.size();++i){
            TableRow tableRow = new TableRow(ScoreActivity.this);
            tableRow.setBackgroundColor(Color.rgb(222, 220, 210));

            TextView tv = new TextView(ScoreActivity.this);


            tv.setText(Integer.valueOf(i+1).toString());
            tableRow.addView(tv);

            tv = new TextView(ScoreActivity.this);
            String name = players.get(i).getName();
            tv.setText(name);
            tableRow.addView(tv);

            tv = new TextView(ScoreActivity.this);
            String score = Integer.valueOf(players.get(i).getScore()).toString();
            tv.setText(score);
            tableRow.addView(tv);

            tv = new TextView(ScoreActivity.this);
            String date = players.get(i).getDate();
            tv.setText(date);
            tableRow.addView(tv);

            tableRow.addView(button[i]);

            tableLayout.addView(tableRow,new ViewGroup.LayoutParams(FP,WC));
        }

    }
    public void exit()
    {
        for(Activity act:MainActivity.activityList)
        {
            act.finish();
        }
        System.exit(0);
    }
    public void exitGame(View v)
    {
        exit();
    }

    public static void setMode(String m){
        mode = m;
    }

    public static String getMode(){
        return mode;
    }

    public static String getThis_user_name() {
        return this_user_name;
    }

    public static void setThis_user_name(String this_user_name) {
        ScoreActivity.this_user_name = this_user_name;
    }
}