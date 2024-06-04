package com.defold.imagepicker;

import android.app.Activity;
import java.util.TimeZone;

import com.github.dhaval2404.imagepicker.ImagePicker;

/**
 */
public class ImagePickerJNI {

  private Activity mActivity;

  public ImagePickerJNI(Activity activity) {
    mActivity = activity;
  }

  public void show() {
    ImagePicker.with(mActivity)
      .crop()	    			//Crop image(Optional), Check Customization for more option
      .compress(1024)			//Final image size will be less than 1 MB(Optional)
      .maxResultSize(1080, 1080)	//Final image resolution will be less than 1080 x 1080(Optional)
      .start();
  }

}
