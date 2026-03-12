package org.qtproject.example.nfc;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.Intent;
import android.content.IntentFilter;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.nfc.tech.*;
import android.os.Bundle;
import android.util.Log;

public class NfcForegroundDispatcher {
    private static final String TAG = "NfcDispatcher";
    private Activity activity;
    private NfcAdapter nfcAdapter;
    private boolean isEnabled = false;

    public NfcForegroundDispatcher(Activity activity) {
        this.activity = activity;
        this.nfcAdapter = NfcAdapter.getDefaultAdapter(activity);
    }

    /**
     * Включает foreground dispatch для перехвата всех NFC меток
     * Должен вызываться в onResume() или когда приложение становится видимым
     */
    public void enableForegroundDispatch() {
        if (nfcAdapter == null) {
            Log.e(TAG, "NFC не поддерживается на этом устройстве");
            return;
        }

        if (!nfcAdapter.isEnabled()) {
            Log.w(TAG, "NFC выключен в настройках системы");
            // Можно показать диалог пользователю с просьбой включить NFC
            return;
        }

        if (isEnabled) {
            Log.d(TAG, "Foreground dispatch уже включен");
            return;
        }

        Log.d(TAG, "Включение foreground dispatch");

        // Создаем Intent, который будет запущен при обнаружении метки
        PendingIntent pendingIntent = PendingIntent.getActivity(
            activity,
            0,
            new Intent(activity, activity.getClass())
                .addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP),
            PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE
        );

        // Фильтры для всех типов NFC меток
        IntentFilter[] filters = new IntentFilter[] {
            new IntentFilter(NfcAdapter.ACTION_NDEF_DISCOVERED),
            new IntentFilter(NfcAdapter.ACTION_TECH_DISCOVERED),
            new IntentFilter(NfcAdapter.ACTION_TAG_DISCOVERED)
        };

        // Все возможные технологии меток
        String[][] techLists = new String[][] {
            new String[] { Ndef.class.getName() },
            new String[] { NdefFormatable.class.getName() },
            new String[] { NfcA.class.getName() },
            new String[] { NfcB.class.getName() },
            new String[] { NfcF.class.getName() },
            new String[] { NfcV.class.getName() },
            new String[] { IsoDep.class.getName() },
            new String[] { MifareClassic.class.getName() },
            new String[] { MifareUltralight.class.getName() }
        };

        try {
            nfcAdapter.enableForegroundDispatch(activity, pendingIntent, filters, techLists);
            isEnabled = true;
            Log.d(TAG, "Foreground dispatch успешно включен");
        } catch (Exception e) {
            Log.e(TAG, "Ошибка включения foreground dispatch: " + e.getMessage());
        }
    }

    /**
     * Выключает foreground dispatch
     * Должен вызываться в onPause() или когда приложение уходит в фон
     */
    public void disableForegroundDispatch() {
        if (nfcAdapter == null || !isEnabled) {
            return;
        }

        Log.d(TAG, "Выключение foreground dispatch");

        try {
            nfcAdapter.disableForegroundDispatch(activity);
            isEnabled = false;
            Log.d(TAG, "Foreground dispatch успешно выключен");
        } catch (Exception e) {
            Log.e(TAG, "Ошибка выключения foreground dispatch: " + e.getMessage());
        }
    }

    /**
     * Обрабатывает новый Intent (должен вызываться из onNewIntent в QtActivity)
     * @return true если метка была обработана, false если это не NFC Intent
     */
    public boolean onNewIntent(Intent intent) {
        String action = intent.getAction();
        Log.d(TAG, "onNewIntent: " + action);

        if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action) ||
            NfcAdapter.ACTION_TECH_DISCOVERED.equals(action) ||
            NfcAdapter.ACTION_TAG_DISCOVERED.equals(action)) {

            Tag tag = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);

            if (tag != null) {
                // Получаем ID метки
                byte[] tagId = tag.getId();
                StringBuilder idHex = new StringBuilder();
                for (byte b : tagId) {
                    idHex.append(String.format("%02X", b));
                }

                Log.d(TAG, "Метка обнаружена! ID: " + idHex.toString());

                // Отправляем данные в Qt через JNI
                notifyTagDetected(tag);
            }

            return true;
        }

        return false;
    }

    /**
     * Уведомляет Qt о обнаружении метки через JNI
     */
    private native void notifyTagDetected(Tag tag);

    /**
     * Проверяет доступность NFC
     */
    public boolean isNfcAvailable() {
        return nfcAdapter != null;
    }

    /**
     * Проверяет, включен ли NFC
     */
    public boolean isNfcEnabled() {
        return nfcAdapter != null && nfcAdapter.isEnabled();
    }
}
