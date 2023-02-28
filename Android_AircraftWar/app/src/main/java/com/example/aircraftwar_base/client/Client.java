package com.example.aircraftwar_base.client;

import android.os.Looper;
import android.util.JsonToken;

import com.example.aircraftwar_base.Player.MyArrayList;
import com.example.aircraftwar_base.Player.Player;
import com.example.aircraftwar_base.activity.ScoreActivity;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

//  负责登录
public class Client {
    private BufferedReader in = null;
    private PrintWriter pw = null;
    private Socket socket;
    //  登录
    private final String LOG_IN = "1";
    //  注册
    private final String REG_IN = "2";
    //  提交游戏记录
    private final String SUB_RE = "3";
    //  获取游戏记录
    private final String GET_RE = "4";
    //  删除
    private final String DEL_RE = "5";
    //  建立房间
    private final String CRE_RO = "6";
    //  联机等待对手
    private final String WAIT_RI = "7";
    //  联机加入对手
    private final String JOIN_RO = "8";
    //  结束
    private final String SUB_EN = "9";

    public Client(Socket socket){
        try{
            this.socket = socket;
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            pw = new PrintWriter(new OutputStreamWriter(socket.getOutputStream(),"utf-8"),true);
        }catch (IOException ex){
            ex.printStackTrace();
        }
    }

    public boolean logIn(String name,String password)
    {
        System.out.println("MY_CLIENT logIn");
        String con;
        if(name.isEmpty() || password.isEmpty()) {
            return false;
        }
        pw.println(LOG_IN);
        pw.println(name + "=" + password);
        try {
            if ((con = in.readLine()) != null) {
                System.out.println("CON " + con);
                return con.equals("Yes");
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        return false;
    }

    public boolean regIn(String name,String password)
    {
        System.out.println("MY_CLIENT regIn");
        String con;
        if(name.isEmpty() || password.isEmpty()) {
            return false;
        }
        pw.println(REG_IN);
        pw.println(name + "=" + password);
        try{
            if(((con=in.readLine())!=null)){
                System.out.println();
                return con.equals("Yes");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return false;
    }


    public void connect()
    {

    }

    public boolean subRecord(String name,String score,String mode)
    {
        System.out.println("MY_CLIENT subRecord");
        if(name.isEmpty()||score.isEmpty()||mode.isEmpty()){
            return false;
        }
        pw.println(SUB_RE);
        pw.println(name+"="+score+"="+mode);
        try {
            String res = in.readLine();
            return res.equals("Yes");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return false;
    }

    public List<Player> getAllRecords()
    {
        System.out.println("MY_CLIENT getAllRecords");
        pw.println(GET_RE);
        pw.println(ScoreActivity.getMode());
        ArrayList<Player> list = new ArrayList<Player>();
        System.out.println("MY_CLIENT reopen buffer stream");
        String data;
        try{
            while(!((data=in.readLine()).equals("#"))){
                System.out.println("MY_CLIENT " + data);
                if(!data.isEmpty()){
                    String []msg = data.split("=");
                    for(String x:msg){
                        System.out.println("MY STRING + "+x);
                    }
                    list.add(new Player(msg[0],Integer.valueOf(msg[1]),msg[2]));
                }
                data="";
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        for(Player p : list){
            System.out.println("MY_CLIENT"+p.getName() + "=" + p.getScore() + "="+p.getDate());
        }
        return list;
    }

    public boolean reqDelete(String name,String date,String mode)
    {
        System.out.println("MY_CLIENT reqDelete");
        pw.println(DEL_RE);
        pw.println(name+"="+date+"="+mode);
        try {
            String res = in.readLine();
            System.out.println("MY_CLIENT " + res);
            if(res==null || res.isEmpty()){
                return false;
            }
            return res.equals("Yes");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return true;
    }

    public boolean createRoom(String roomName)
    {
        System.out.println("MY_CLIENT createRoom");
        pw.println(CRE_RO);
        pw.println(roomName);   //  房间名称
        //  返回Yes/No
        try {
            String res = in.readLine();
            System.out.println("MY_CLIENT " + res);
            if(res==null || res.isEmpty()){
                return false;
            }
            return res.equals("Yes");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return false;
    }

    public boolean waitRival()
    {
        System.out.println("MY_CLIENT waitRival");
        pw.println(WAIT_RI);
        //  返回Yes/No
        try {
            String res = in.readLine();
            System.out.println("MY_CLIENT " + res);
            if(res==null || res.isEmpty()){
                return false;
            }
            return res.equals("Yes");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return true;
    }

    public boolean joinRoom(String roomName)
    {
        pw.println(JOIN_RO);
        pw.println(roomName);
        //  Server返回Yes/No
        try {
            String res = in.readLine();
            System.out.println("MY_CLIENT " + res);
            if(res==null || res.isEmpty()){
                return false;
            }
            return res.equals("Yes");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return true;
    }

    public boolean subEnd()
    {
        //  结束标志
        pw.println(SUB_EN);
        try {
            String res = in.readLine();     //  阻塞读 我认为
            return res.equals("Over");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return false;
    }
}