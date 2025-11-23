package com.example.myapplication;

import android.view.View;

import androidx.core.view.WindowCompat;
import androidx.core.view.WindowInsetsCompat;
import androidx.core.view.WindowInsetsControllerCompat;

import com.google.androidgamesdk.GameActivity;

public class MainActivity extends GameActivity {
    static {
        System.loadLibrary("myapplication");
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);

        if (hasFocus) {
            hideSystemUi();
        }
    }

    private void hideSystemUi() {
        // La nueva API es más declarativa y compatible con versiones anteriores y futuras.

        // Permite a tu app dibujar detrás de las barras del sistema (status bar y navigation bar).
        // Esto reemplaza a: LAYOUT_STABLE, LAYOUT_HIDE_NAVIGATION y LAYOUT_FULLSCREEN.
        WindowCompat.setDecorFitsSystemWindows(getWindow(), false);

        WindowInsetsControllerCompat insetsController =
                WindowCompat.getInsetsController(getWindow(), getWindow().getDecorView());

        // Oculta las barras del sistema (status y navigation).
        // Esto reemplaza a: HIDE_NAVIGATION y FULLSCREEN.
        insetsController.hide(WindowInsetsCompat.Type.systemBars());

        // Configura el comportamiento para el modo inmersivo. Las barras aparecerán temporalmente
        // al deslizar desde el borde de la pantalla.
        // Esto reemplaza a: IMMERSIVE_STICKY.
        insetsController.setSystemBarsBehavior(
            WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
        );
    }
}