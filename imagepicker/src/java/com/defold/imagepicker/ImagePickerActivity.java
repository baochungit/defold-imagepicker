package com.defold.imagepicker;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import androidx.appcompat.app.AppCompatActivity;
import com.github.dhaval2404.imagepicker.ImagePicker;


public class ImagePickerActivity extends AppCompatActivity {

  private static final String TAG = "defold.imagepicker";

  private Messenger mMessenger;

  @Override
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    Utils.convertActivityToTranslucent(this);
    getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));

    Intent intent = this.getIntent();
    Bundle extras = intent.getExtras();
    mMessenger = (Messenger) extras.getParcelable(ImagePickerJNI.INTENT_EXTRA_MESSENGER);
    ImagePicker.with(this)
      .galleryOnly()
      .galleryMimeTypes(new String[] {
        "image/png",
        "image/jpg",
        "image/jpeg"
      })
      .cropSquare()
      .compress(1024)
      .maxResultSize(200, 200)
      .start();
  }

  @Override
  public void onDestroy() {
    super.onDestroy();
  }

  @Override
  protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    super.onActivityResult(requestCode, resultCode, data);
    if (resultCode == Activity.RESULT_OK) {
      // Uri object will not be null for RESULT_OK
      Uri uri = data.getData();

      Bundle bundle = new Bundle();
      bundle.putBoolean(ImagePickerJNI.MSG_KEY_SUCCESS, true);
      bundle.putString(ImagePickerJNI.MSG_KEY_URI, uri.toString());
      respond(bundle);
    } else {
      Bundle bundle = new Bundle();
      bundle.putBoolean(ImagePickerJNI.MSG_KEY_SUCCESS, false);
      respond(bundle);
    }
  }

  private void respond(final Bundle bundle) {
    final Activity activity = this;
    this.runOnUiThread(new Runnable() {
      @Override
      public void run() {
        Message message = new Message();
        message.setData(bundle);
        try {
          mMessenger.send(message);
        } catch (RemoteException e) {
          Log.wtf(TAG, e);
        }
        activity.finish();
      }
    });
  }

}
