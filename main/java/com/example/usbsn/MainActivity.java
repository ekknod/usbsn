package com.example.usbsn;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.EditText;
import android.widget.TextView;
import com.example.usbsn.databinding.ActivityMainBinding;
import java.io.IOException;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'usbsn' library on application startup.
    static {
        System.loadLibrary("usbsn");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        try {
            Runtime.getRuntime().exec("su");
        } catch (IOException e) {
            // e.printStackTrace();
        }


        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        EditText productText = binding.productText;
        EditText serialText = binding.serialText;
        Button button = binding.button;

        String serial = getSerialNumber();
        String product = getProduct();





        serialText.setText(serial);

        if (product.length() < 2) {
            productText.setText("Root not found");
            serialText.setText("Root not found");
        } else
            productText.setText(getProduct());




        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                Patch(      productText.getText().toString(), serialText.getText().toString()     );

                productText.setText(getProduct());
                serialText.setText(getSerialNumber());


            }


        });


    }

    /**
     * A native method that is implemented by the 'usbsn' native library,
     * which is packaged with this application.
     */



    public native String getSerialNumber();
    public native String getProduct();
    public native int Patch(String product, String serial);
}