package com.example.aircraftwar_base.Player;

import java.io.Serializable;
import java.util.ArrayList;

public class MyArrayList implements Serializable {//实现序列化接口...必须

    private ArrayList<Player> list;

    public ArrayList<Player> getList() {
        return list;
    }

    public void setList(ArrayList<Player> list) {
        this.list = list;
    }

}
