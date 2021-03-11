package com.ahmedmamdouh13

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.LayoutInflater
import android.widget.TextView
import com.ahmedmamdouh13.databinding.ActivityMainBinding
import org.opencv.android.BaseLoaderCallback
import org.opencv.android.CameraActivity
import org.opencv.android.LoaderCallbackInterface
import org.opencv.android.OpenCVLoader

class MainActivity : CameraActivity() {


    private lateinit var binder: ActivityMainBinding


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binder = ActivityMainBinding.inflate(LayoutInflater.from(this));
        setContentView(binder.root)

    }

    override fun onResume() {
        super.onResume()

        if (!OpenCVLoader.initDebug()) {
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION, this, baseLoaderCallback)
        } else {
            baseLoaderCallback.onManagerConnected(BaseLoaderCallback.SUCCESS)
        }
    }

    external fun drawFeatures(matRGB: Long, matGray: Long): String

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }


    val baseLoaderCallback = object : BaseLoaderCallback(this) {

        override fun onManagerConnected(status: Int) {
            super.onManagerConnected(status)

            if (status == SUCCESS) {

            } else {

            }

        }
    }
}