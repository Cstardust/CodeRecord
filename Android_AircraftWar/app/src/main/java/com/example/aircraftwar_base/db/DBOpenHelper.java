package com.example.aircraftwar_base.db;

import android.annotation.SuppressLint;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;


import com.example.aircraftwar_base.Player.Player;
import com.example.aircraftwar_base.activity.ScoreActivity;

import java.util.ArrayList;

//   与数据库交互的DAO层
//   database中有两个表 一个是密码表User、一个是游戏记录表Record
//   如果你要写排行榜，
//   那么获取你所需的游戏记录信息的方式是getAllData()；
//   你在数据库中删除一条记录的方式是deleteRecord
public class DBOpenHelper extends SQLiteOpenHelper {
    /**
     * 声明一个AndroidSDK自带的数据库变量db
     */
    private SQLiteDatabase db;

    /**
     * 写一个这个类的构造函数，参数为上下文context，所谓上下文就是这个类所在包的路径
     * 指明上下文，数据库名，工厂默认空值，版本号默认从1开始
     * super(context,"db_test",null,1);
     * 把数据库设置成可写入状态，除非内存已满，那时候会自动设置为只读模式
     * 不过，以现如今的内存容量，估计一辈子也见不到几次内存占满的状态
     * db = getReadableDatabase();
     */
    public DBOpenHelper(Context context){
        super(context,"db_test",null,1);
        db = getReadableDatabase();
    }

    /**
     * 重写两个必须要重写的方法，因为class DBOpenHelper extends SQLiteOpenHelper
     */
    @Override
    public void onCreate(SQLiteDatabase db){
        //  name-pwd表
        db.execSQL("CREATE TABLE IF NOT EXISTS user(" +
                "_id INTEGER PRIMARY KEY AUTOINCREMENT," +
                "name TEXT," +
                "password TEXT)");
        //  name-score-date表
        db.execSQL("CREATE TABLE IF NOT EXISTS record(" +
                "_id INTEGER PRIMARY KEY AUTOINCREMENT,"+
                "name TEXT," +
                "score INTEGER," +
                "mode TEXT,"+
                "date timestamp not null default (datetime('now','localtime','+8 hour')))");
        //  联机之后我想可能是 从name-score-date表中 按每个名字取出最大的Score
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion){
        db.execSQL("DROP TABLE IF EXISTS user");
        db.execSQL("DROP TABLE IF EXISTS record");
        onCreate(db);
    }
    /**
     * 增删改查
     * addUser()：增加kv name-password(user表)
     * deleteRecord()：删除Record表中一条记录(Record表)
     * updatePwd()：更新密码(user表)
     * getPwd():获取密码(user表)
     * getAllRecord():获取Record表中所有记录(Record表)
     */
    public boolean addUser(String name,String password){
        Cursor cursor = db.query("user",null,"name=?",new String[]{name},null,null,null);
        if(cursor.getCount()!=0) {
            return false;
        }
        db.execSQL("INSERT INTO user (name,password) VALUES(?,?)",new Object[]{name,password});
        return true;
    }

    public void addRecord(String name,String score,String mode){
        db.execSQL("INSERT INTO record(name,score,mode) VALUES(?,?,?)",new Object[]{name,score,mode});
    }

    public void deleteRecord(String name,String date){
        db.execSQL("DELETE FROM record WHERE name = " + "'" + name + "'" + " AND date = "+ "'"+date+"'");
    }

    public void updatePwd(String name,String password){
        ContentValues val = new ContentValues();
        val.put("password",password);
        db.update("user",val,"name=?",new String[]{name});
    }


    @SuppressLint("Range")
    public String getPwd(String name){
        Cursor cursor = db.query("user",null,"name=?",new String[]{name},null,null,null);
        if(cursor.getCount()!=1){
            return null;
        }
        if(cursor.moveToFirst()){
            return cursor.getString(cursor.getColumnIndex("password"));
        }
        return null;
    }

    //  获取所有游戏记录
    public ArrayList<Player> getAllRecord(){
        ArrayList<Player> list = new ArrayList<Player>();
        Cursor cursor = db.query("record",null,"mode=?",new String[]{ScoreActivity.getMode()},null,null,"score DESC");
        while(cursor.moveToNext()){
            @SuppressLint("Range") String name = cursor.getString(cursor.getColumnIndex("name"));
            @SuppressLint("Range") int score = cursor.getInt(cursor.getColumnIndex("score"));
            @SuppressLint("Range") String date = cursor.getString(cursor.getColumnIndex("date"));
            list.add(new Player(name,score,date));
        }
        return list;
    }

}
