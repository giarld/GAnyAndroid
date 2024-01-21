package com.gx.example;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
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
        EditText radiusEdit = binding.radiusEdit;
        Button calcButton = binding.calcButton;

        GAny tCircular = GAny.Import("Cpp.Circular");
        GAny circular = tCircular.New(3, 1, 1);

        calcButton.setOnClickListener(view -> {
            circular.setItem("radius", Float.parseFloat(radiusEdit.getText().toString()));
            tv.setText(String.format("Origin: %s\nRadius: %.3f\nArea: %.3f\nC: %.3f",
                    circular.getItem("origin").toString(),
                    circular.callMethod("getRadius").toFloat(),
                    circular.callMethod("getArea").toFloat(),
                    circular.callMethod("getCircumference").toFloat()));
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}