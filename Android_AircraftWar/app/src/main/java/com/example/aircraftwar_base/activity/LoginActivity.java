package com.example.aircraftwar_base.activity;

import static java.lang.Thread.sleep;

import android.content.Intent;
import android.os.Bundle;
import android.os.StrictMode;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.example.aircraftwar_base.R;
import com.example.aircraftwar_base.client.Client;
import com.example.aircraftwar_base.db.DBOpenHelper;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.UnknownHostException;

/**
 * 此类 implements View.OnClickListener 之后，
 * 就可以把onClick事件写到onCreate()方法之外
 * 这样，onCreate()方法中的代码就不会显得很冗余
 */
public class LoginActivity extends AppCompatActivity implements View.OnClickListener {
    /**
     * 声明自己写的 DBOpenHelper 对象
     * DBOpenHelper(extends SQLiteOpenHelper) 主要用来
     * 创建数据表
     * 然后再进行数据表的增、删、改、查操作
     */
    private DBOpenHelper mDBOpenHelper;
    private TextView mTvLoginactivityRegister;
    private RelativeLayout mRlLoginactivityTop;
    private EditText mEtLoginactivityUsername;
    private EditText mEtLoginactivityPassword;
    private LinearLayout mLlLoginactivityTwo;
    private Button mBtLoginactivityLogin;
    private static Client client;
    private static boolean flag = false;
    /**
     * 创建 Activity 时先来重写 onCreate() 方法
     * 保存实例状态
     * super.onCreate(savedInstanceState);
     * 设置视图内容的配置文件
     * setContentView(R.layout.activity_login);
     * 上面这行代码真正实现了把视图层 View 也就是 layout 的内容放到 Activity 中进行显示
     * 初始化视图中的控件对象 initView()
     * 实例化 DBOpenHelper，待会进行登录验证的时候要用来进行数据查询
     * mDBOpenHelper = new DBOpenHelper(this);
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        MainActivity.activityList.add(this);

        initView();

        mDBOpenHelper = new DBOpenHelper(this);

        if (android.os.Build.VERSION.SDK_INT > 9) {
            StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
            StrictMode.setThreadPolicy(policy);
        }

        //  发起连接
//        new Thread(new NetConn()).start();
        if(!flag){
            connect();
            flag = true;
        }
        //  初始化客户端
    }

    /**
     * onCreae()中大的布局已经摆放好了，接下来就该把layout里的东西
     * 声明、实例化对象然后有行为的赋予其行为
     * 这样就可以把视图层View也就是layout 与 控制层 Java 结合起来了
     */
    private void initView() {
        // 初始化控件
        mBtLoginactivityLogin = findViewById(R.id.bt_loginactivity_login);
        mTvLoginactivityRegister = findViewById(R.id.tv_loginactivity_register);
        mRlLoginactivityTop = findViewById(R.id.rl_loginactivity_top);
        mEtLoginactivityUsername = findViewById(R.id.et_loginactivity_username);
        mEtLoginactivityPassword = findViewById(R.id.et_loginactivity_password);
        mLlLoginactivityTwo = findViewById(R.id.ll_loginactivity_two);

        // 设置点击事件监听器
        mBtLoginactivityLogin.setOnClickListener(this);
        mTvLoginactivityRegister.setOnClickListener(this);
    }

    public void onClick(View view) {
        switch (view.getId()) {
            // 跳转到注册界面
            case R.id.iv_loginactivity_back:
                startActivity(new Intent(this, RegisterActivity.class));
                finish();
                break;
            /**
             * 登录验证：
             *
             * 从EditText的对象上获取文本编辑框输入的数据，并把左右两边的空格去掉
             *  String name = mEtLoginactivityUsername.getText().toString().trim();
             *  String password = mEtLoginactivityPassword.getText().toString().trim();
             *  进行匹配验证,先判断一下用户名密码是否为空，
             *  if (!TextUtils.isEmpty(name) && !TextUtils.isEmpty(password))
             *  再进而for循环判断是否与数据库中的数据相匹配
             *  if (name.equals(user.getName()) && password.equals(user.getPassword()))
             *  一旦匹配，立即将match = true；break；
             *  否则 一直匹配到结束 match = false；
             *
             *  登录成功之后，进行页面跳转：
             *
             *  Intent intent = new Intent(this, MainActivity.class);
             *  startActivity(intent);
             *  finish();//销毁此Activity
             */
            case R.id.bt_loginactivity_login:

                String name = mEtLoginactivityUsername.getText().toString().trim();
                String password = mEtLoginactivityPassword.getText().toString().trim();
                System.out.println("MY_CLIENT " + name+password);

                if (!TextUtils.isEmpty(name) && !TextUtils.isEmpty(password)) {
                    if(client.logIn(name,password)){
//                    if (password.equals(mDBOpenHelper.getPwd(name))) {
                        ScoreActivity.setThis_user_name(name);
                        Toast.makeText(this, "登录成功", Toast.LENGTH_SHORT).show();
                        Intent intent = new Intent(this, ModeChooseActivity.class);
                        startActivity(intent);
//                        finish();//销毁此Activity
                    } else {
                        Toast.makeText(this, "用户名或密码不正确，请重新输入", Toast.LENGTH_SHORT).show();
                    }
                } else {
                    Toast.makeText(this, "请输入你的用户名或密码", Toast.LENGTH_SHORT).show();
                }
                break;
        }
    }

    //  xia
    private static Socket socket;
    public static PrintWriter writer;


    public void connect()
    {
        try{
            System.out.println("MY_CLIENT NETCONN");
            socket = new Socket();
            socket.connect(new InetSocketAddress("192.168.43.228",9999));
//            socket.connect(new InetSocketAddress("10.0.2.2",9999));
//            socket.connect(new InetSocketAddress("10.250.225.85",9999),5000);
//            socket.connect(new InetSocketAddress("192.168.56.1",9999));
            System.out.println("MY_CLIENT " + InetAddress.getLocalHost());
            writer = new PrintWriter(new BufferedWriter(
                    new OutputStreamWriter(
                            socket.getOutputStream(),"UTF-8")),true);
            System.out.println("MY_CLIENT CONNECTED");
            client = new Client(socket);

        }catch(UnknownHostException ex){
            ex.printStackTrace();
        }catch(IOException ex){
            ex.printStackTrace();
        }
    }



    //  负责连接
    protected class NetConn extends Thread{
        @Override
        public void run(){
            try{
                System.out.println("NETCONN");
                socket = new Socket();
                socket.connect(new InetSocketAddress("192.168.43.228",9999));
                writer = new PrintWriter(new BufferedWriter(
                        new OutputStreamWriter(
                                socket.getOutputStream(),"UTF-8")),true);
                System.out.println("CONNECTED");
            }catch(UnknownHostException ex){
                ex.printStackTrace();
            }catch(IOException ex){
                ex.printStackTrace();
            }
        }
    }

    public static Socket getSocket(){return socket;}
    public static Client getClient(){return client;}
}
