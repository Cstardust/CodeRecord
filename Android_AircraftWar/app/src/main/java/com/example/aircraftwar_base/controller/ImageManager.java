package com.example.aircraftwar_base.controller;

import android.graphics.Bitmap;

import java.util.HashMap;
import java.util.Map;

/**
 * 综合管理图片的加载，访问
 * 提供图片的静态访问方法
 *
 * @author hitsz
 */
public class ImageManager {

    /**
     * 类名-图片 映射，存储各基类的图片 <br>
     * 可使用 CLASSNAME_IMAGE_MAP.get( obj.getClass().getName() ) 获得 obj 所属基类对应的图片
     */
    public static Map<String, Bitmap> CLASSNAME_IMAGE_MAP = new HashMap<>();

    public static Bitmap BACKGROUND_IMAGE;

    public static Bitmap HERO_IMAGE;
    public static Bitmap MOB_ENEMY_IMAGE;
    public static Bitmap BOSS_ENEMY_IMAGE;
    public static Bitmap ELITE_ENEMY_IMAGE;

    public static Bitmap HERO_BULLET_IMAGE;
    public static Bitmap ENEMY_BULLET_IMAGE;

    public static Bitmap BLOOD_REWARD_IMAGE;
    public static Bitmap BOMB_REWARD_IMAGE;
    public static Bitmap BULLET_REWARD_IMAGE;

    public static Bitmap get(Object obj){
        if (obj == null){
            return null;
        }
        return CLASSNAME_IMAGE_MAP.get(obj.getClass().getName());
    }

}
