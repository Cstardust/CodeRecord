package com.example.aircraftwar_base.Player;

//  其实感觉更应该改名成 “class 游戏记录” 比如 record 或者 data
//  本class里面不存pwd
//  这是一条条游戏记录 又不是一个个玩家信息
//  看大哥的吧还是
public class Player implements Comparable<Player> {
    private String name;
    private int score;
    private String date;

    public Player(String userName,int score,String date) {
        this.name = userName;
        this.score = score;
        this.date = date;
    }

    public int getScore() {
        return score;
    }
    public String getName() {
        return name;
    }
    public String getDate(){
        return date;
    }

    @Override
    public int compareTo(Player player2) {
        if (getScore() > player2.getScore()) {
            return -1;
        } else if (getScore() < player2.getScore()) {
            return 1;
        } else {
            return 0;
        }
    }
}