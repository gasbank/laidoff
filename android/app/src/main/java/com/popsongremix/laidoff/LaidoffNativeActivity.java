package com.popsongremix.laidoff;

import android.app.NativeActivity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Toast;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.reward.RewardItem;
import com.google.android.gms.ads.reward.RewardedVideoAd;
import com.google.android.gms.ads.reward.RewardedVideoAdListener;
import com.google.firebase.iid.FirebaseInstanceId;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Locale;

public class LaidoffNativeActivity extends NativeActivity implements RewardedVideoAdListener
{
    private static boolean mBgmOn;

    static {
        System.loadLibrary("zmq");
        System.loadLibrary("czmq");
        System.loadLibrary("native-activity");
    }

    private static MediaPlayer mBgmPlayer;
    private int mSound_MetalHit;
    private SoundPool mSoundPool;
    private int mSound_Tapping01;
    private int mSound_Completed03;
    private int mSound_GameOver01;
    private int mSound_GameStart01;
    private int mSound_Point;

    public static native String signalResourceReady(Class<LaidoffNativeActivity> and9NativeActivityClass);
    public static native int pushTextureData(int width, int height, int[] data, int texAtlasIndex);
    public static native void registerAsset(String assetPath, int startOffset, int length);
    private static native void sendApkPath(String apkPath, String filesPath, String packageVersion);
    public static native void setPushTokenAndSend(String text, long pLwcLong);
    public static native void setWindowSize(int width, int height, long pLwcLong);

    public static final String PREFS_NAME = "LaidoffPrefs";
    public static final String PREFS_KEY_HIGHSCORE = "highscore";

    private static LaidoffNativeActivity INSTANCE;
    
    public static final String LOG_TAG = "and9";
    private RewardedVideoAd mRewardedVideoAd;
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        INSTANCE = this;
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        // AdMob
        MobileAds.initialize(this, "ca-app-pub-5072035175916776~3533761034");
        // Interstitial
        mRewardedVideoAd = MobileAds.getRewardedVideoAdInstance(this);
        mRewardedVideoAd.setRewardedVideoAdListener(this);

        //getWindow().getDecorView().setKeepScreenOn(true);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED |
                WindowManager.LayoutParams.FLAG_DISMISS_KEYGUARD |
                WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON |
                WindowManager.LayoutParams.FLAG_TURN_SCREEN_ON |
                WindowManager.LayoutParams.FLAG_ALLOW_LOCK_WHILE_SCREEN_ON);
        final View decorView = getWindow().getDecorView();
        decorView.setOnSystemUiVisibilityChangeListener
                (new View.OnSystemUiVisibilityChangeListener() {
                    @Override
                    public void onSystemUiVisibilityChange(int visibility) {
                        Log.i(LOG_TAG, "decorView window size width: " + decorView.getWidth());
                        Log.i(LOG_TAG, "decorView window size height: " + decorView.getHeight());
                        setWindowSize(decorView.getWidth(), decorView.getHeight(), 0);
                        // Note that system bars will only be "visible" if none of the
                        // LOW_PROFILE, HIDE_NAVIGATION, or FULLSCREEN flags are set.
                        if ((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0) {
                            // TODO: The system bars are visible. Make any desired
                            // adjustments to your UI, such as showing the action bar or
                            // other navigational controls.
                        } else {
                            // TODO: The system bars are NOT visible. Make any desired
                            // adjustments to your UI, such as hiding the action bar or
                            // other navigational controls.
                        }
                    }
                });

        Log.i(LOG_TAG, "Device Android Version: " + Build.VERSION.SDK_INT);

        AssetsLoader assetsLoader = new AssetsLoader(this);
        Log.i(LOG_TAG, "APK Path: " + assetsLoader.GetAPKPath());
        assetsLoader.registerAllAssetsOfType("tex");
        assetsLoader.registerAllAssetsOfType("pkm");
        assetsLoader.registerAllAssetsOfType("ktx");
        assetsLoader.registerAllAssetsOfType("vbo");
        assetsLoader.registerAllAssetsOfType("svbo");
        assetsLoader.registerAllAssetsOfType("armature");
        assetsLoader.registerAllAssetsOfType("action");
        assetsLoader.registerAllAssetsOfType("fnt");
        assetsLoader.registerAllAssetsOfType("d");
        assetsLoader.registerAllAssetsOfType("glsles");
        assetsLoader.registerAllAssetsOfType("l");
        assetsLoader.registerAllAssetsOfType("field");
        assetsLoader.registerAllAssetsOfType("nav");
        sendApkPath(assetsLoader.GetAPKPath(), getApplicationContext().getFilesDir().getAbsolutePath(), getPackageVersion());

        downloadResFromServer();

        //noinspection deprecation
        mSoundPool = new SoundPool(5, AudioManager.STREAM_MUSIC, 0);
        mSoundPool.setOnLoadCompleteListener(new SoundPool.OnLoadCompleteListener() {
            @Override
            public void onLoadComplete(SoundPool soundPool, int sampleId,
                                       int status) {
                //loaded = true;

                //mSoundPool.play(sampleId, 1, 1, 0, 0, 1);
            }
        });

        mSound_Tapping01 = mSoundPool.load(getApplicationContext(), R.raw.jump, 1);
        mSound_Completed03 = mSoundPool.load(getApplicationContext(), R.raw.completed, 1);
        mSound_GameOver01 = mSoundPool.load(getApplicationContext(), R.raw.over, 1);
        mSound_GameStart01 = mSoundPool.load(getApplicationContext(), R.raw.start, 1);
        mSound_Point = mSoundPool.load(getApplicationContext(), R.raw.point, 1);
        mSound_MetalHit = mSoundPool.load(getApplicationContext(), R.raw.sfx_metal_hit, 1);

        mBgmPlayer = MediaPlayer.create(getApplicationContext(), R.raw.opening); // in 2nd param u have to pass your desire ringtone
        mBgmPlayer.setLooping(true);
        //mBgmPlayer.prepare();
        //mBgmPlayer.start();

        /*
        final int atlasCount = 3;
        for (int i = 0; i < atlasCount; i++) {
            String assetName = String.format(Locale.US, "tex/kiwi-atlas-set-%02d.png", i + 1);
            loadBitmap(assetName);
        }
        */

        //Intent intent = new Intent(this, TextInputActivity.class);
        //startActivity(intent);
    }

    private void loadRewardedVideoAd() {
        // production: ca-app-pub-5072035175916776/9587253247
        mRewardedVideoAd.loadAd("ca-app-pub-3940256099942544/5224354917",
                new AdRequest.Builder().build());
    }

    public void playMetalHitSound() {
        mSoundPool.play(mSound_MetalHit, 1, 1, 0, 0, 1);
    }

    private void downloadResFromServer() {

        File files = getApplicationContext().getFilesDir();

        File[] fileList = files.listFiles();

        Log.i(LOG_TAG, String.format("Download cache dir: %s (%d files)", files.getAbsolutePath(), fileList.length));

        for (File aFileList : fileList) {
            //Date d = new Date(fileList[i].lastModified());
            Log.i(LOG_TAG, String.format(" - file: %s", aFileList.getAbsolutePath()));
        }

        UpdateResTaskParam urtp = new UpdateResTaskParam();
        urtp.fileAbsolutePath = files.getAbsolutePath();
        urtp.remoteAssetsBasePath = "http://sky.popsongremix.com/laidoff/assets";
        //urtp.remoteAssetsBasePath = "http://192.168.0.28:19876/assets";
        urtp.remoteApkBasePath = "http://sky.popsongremix.com/laidoff/apk";
        //urtp.remoteAssetsBasePath = "http://222.110.4.119:18080";
        urtp.remoteListFilePath = "list.txt";
        urtp.localListFilename = "list.txt";

         new UpdateResTask(this).execute(urtp);
    }

    static public int loadBitmap(String assetName) {
        return loadBitmapWithIndex(0, assetName);
    }

    static public int loadBitmapWithIndex(int i, String assetName) {
        Bitmap bitmap = getBitmapFromAsset(INSTANCE.getApplicationContext(), assetName);

        Log.i(LOG_TAG, String.format(Locale.US, "Tex(asset name) %s Bitmap width: %d", assetName, bitmap.getWidth()));
        Log.i(LOG_TAG, String.format(Locale.US, "Tex(asset name) %s Bitmap height: %d", assetName, bitmap.getHeight()));

        int[] pixels = new int[bitmap.getWidth()*bitmap.getHeight()];
        bitmap.getPixels(pixels, 0, bitmap.getWidth(), 0, 0, bitmap.getWidth(), bitmap.getHeight());
        int bytes_allocated_on_native = INSTANCE.pushTextureData(bitmap.getWidth(), bitmap.getHeight(), pixels, i);

        Log.i(LOG_TAG, String.format(Locale.US, "Tex(asset name) %s Bitmap copied to native side %d bytes", assetName, bytes_allocated_on_native));

        return bitmap.getWidth() * bitmap.getHeight();
    }

    public static Bitmap getBitmapFromAsset(Context context, String filePath) {
        AssetManager assetManager = context.getAssets();

        InputStream inputStr;
        Bitmap bitmap = null;
        try {
            boolean fromDownloaded = true;
            if (fromDownloaded) {

                String filenameOnly = filePath.substring(filePath.lastIndexOf("/")+1);

                File f = new File(context.getFilesDir().getAbsoluteFile(), filenameOnly);

                inputStr = new FileInputStream(f);
            } else {
                inputStr = assetManager.open(filePath);
            }

            bitmap = BitmapFactory.decodeStream(inputStr);
        } catch (IOException e) {
            // handle exception
        }

        return bitmap;
    }

    @Override
    public void onDestroy() {
        mRewardedVideoAd.destroy(this);
        super.onDestroy();

        Log.d(LOG_TAG, "onDestroy()");
    }

    @Override
    public void onPause() {
        mRewardedVideoAd.pause(this);
        super.onPause();

        if (mBgmOn)
        {
            mBgmPlayer.pause();
        }

        Log.d(LOG_TAG, "onPause()");
    }

    @Override
    public void onResume() {
        mRewardedVideoAd.resume(this);
        super.onResume();

        if (mBgmOn)
        {
            mBgmPlayer.start();
        }

        Log.d(LOG_TAG, "onResume()");
    }


    public static void startTextInputActivity(String dummy) {
        Intent intent = new Intent(INSTANCE, TextInputActivity.class);
//        EditText editText = (EditText) findViewById(R.id.editText);
//        String message = editText.getText().toString();
//        intent.putExtra(EXTRA_MESSAGE, message);
        INSTANCE.startActivity(intent);

        // test interstitial
        INSTANCE.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                INSTANCE.loadRewardedVideoAd();
            }
        });


    }

    public static void requestPushToken(long pLwc) {
        setPushTokenAndSend(FirebaseInstanceId.getInstance().getToken(), pLwc);
    }

    public static void startPlayMetalHitSound(String dummy) {
        INSTANCE.playMetalHitSound();
    }

    public static String getPackageVersion() {
        String packageVersionName = "0.0.0";
        try {
            PackageInfo packageInfo = INSTANCE.getPackageManager().getPackageInfo(INSTANCE.getPackageName(), 0);
            packageVersionName = packageInfo.versionName;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return packageVersionName;
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
                final View decorView = getWindow().getDecorView();
                decorView.setSystemUiVisibility(
                        View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                                | View.SYSTEM_UI_FLAG_LOW_PROFILE
                                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
                                | View.SYSTEM_UI_FLAG_FULLSCREEN // hide status bar
                                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                );
                Log.i(LOG_TAG, "decorView window width: " + decorView.getWidth());
                Log.i(LOG_TAG, "decorView window height: " + decorView.getHeight());
            }
        }
    }

    // AdMob reward video callbacks

    public void onRewarded(RewardItem reward) {
        Toast.makeText(this, "onRewarded! currency: " + reward.getType() + "  amount: " +
                reward.getAmount(), Toast.LENGTH_SHORT).show();
        // Reward the user.
    }

    public void onRewardedVideoAdLeftApplication() {
        Toast.makeText(this, "onRewardedVideoAdLeftApplication",
                Toast.LENGTH_SHORT).show();
    }

    public void onRewardedVideoAdClosed() {
        Toast.makeText(this, "onRewardedVideoAdClosed", Toast.LENGTH_SHORT).show();
    }

    public void onRewardedVideoAdFailedToLoad(int errorCode) {
        Toast.makeText(this, "onRewardedVideoAdFailedToLoad", Toast.LENGTH_SHORT).show();
    }

    public void onRewardedVideoAdLoaded() {
        Toast.makeText(this, "onRewardedVideoAdLoaded", Toast.LENGTH_SHORT).show();
        if (mRewardedVideoAd.isLoaded()) {
            mRewardedVideoAd.show();
        }
    }

    public void onRewardedVideoAdOpened() {
        Toast.makeText(this, "onRewardedVideoAdOpened", Toast.LENGTH_SHORT).show();
    }

    public void onRewardedVideoStarted() {
        Toast.makeText(this, "onRewardedVideoStarted", Toast.LENGTH_SHORT).show();
    }
}
