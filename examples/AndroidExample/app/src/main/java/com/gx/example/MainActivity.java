package com.gx.example;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.gx.example.databinding.ActivityMainBinding;
import com.gx.gany.GAny;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding binding;

    @SuppressLint({"SetTextI18n", "DefaultLocale"})
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        TextView tv = binding.sampleText;

        GAny javaFunc = GAny.function((v0, v1, v2) -> {
            int i0 = v0.toInt32();
            int i1 = v1.toInt32();
            int i2 = v2.toInt32();

            return GAny.create(Math.max(i0, Math.max(i1, i2)));
        });

        if (javaFunc.isFunction()) {
            tv.setText("Result: " + javaFunc.call(12, 7, 46));   // Result: 58
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}