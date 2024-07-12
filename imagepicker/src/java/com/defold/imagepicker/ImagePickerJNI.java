package com.defold.imagepicker;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Messenger;
import android.util.Log;


public class ImagePickerJNI implements Handler.Callback {

  private static final String TAG = "defold.imagepicker";

  public static final String INTENT_EXTRA_MESSENGER    = "defold.imagepicker.intent.extra.messenger";
  public static final String MSG_KEY_SUCCESS    = "defold.imagepicker.msg.success";
  public static final String MSG_KEY_URI    = "defold.imagepicker.msg.uri";

  private Activity mActivity;
  private Messenger mMessenger;

  public native void onDone(String uri);

  public ImagePickerJNI(Activity activity) {
    mActivity = activity;
  }

  public void show() {
    final ImagePickerJNI self = this;
    mActivity.runOnUiThread(new Runnable() {
      @Override
      public void run() {
        if (mMessenger == null) {
          mMessenger = new Messenger(new Handler(self));
        }
        Intent intent = new Intent(mActivity, ImagePickerActivity.class);
        intent.putExtra(INTENT_EXTRA_MESSENGER, mMessenger);
        mActivity.startActivity(intent);
      }
    });
  }

    @Override
    public boolean handleMessage(Message msg) {
      boolean handled = false;
      Bundle data = msg.getData();
      boolean success = data.getBoolean(MSG_KEY_SUCCESS, false);
      if (success) {
        String uri = data.getString(MSG_KEY_URI);
        Log.d(TAG, uri);
        this.onDone(uri);
      } else {
        this.onDone("");
      }
      return handled;
    }
}
