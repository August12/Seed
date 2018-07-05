package com.example.gyuyeong.hereur;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

public class introScreen extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.intro);

        Thread timerThread = new Thread(){
            public void run(){
                try{
                    sleep(3000);
                }catch (InterruptedException e){
                    e.printStackTrace();
                }finally {
                    Intent intent = new Intent(introScreen.this , MainActivity.class);
                    startActivity(intent);
                }
            }

        };

        timerThread.start();

    }

    @Override
    protected void onPause(){
        super.onPause();
        finish();
    }

    @Override
    public  void onBackPressed(){
        if(false){
            super.onBackPressed();
        }else{

        }
    }
}
