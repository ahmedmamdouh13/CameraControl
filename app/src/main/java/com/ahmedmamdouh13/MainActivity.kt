package com.ahmedmamdouh13

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.widget.TextView
import com.ahmedmamdouh13.databinding.ActivityMainBinding
import org.opencv.android.*
import org.opencv.core.Mat
import java.util.*

class MainActivity : CameraActivity(),CameraBridgeViewBase.CvCameraViewListener2  {


    private lateinit var binder: ActivityMainBinding


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binder = ActivityMainBinding.inflate(LayoutInflater.from(this));
        setContentView(binder.root)
        binder.cameraview.visibility = View.VISIBLE

        binder.cameraview.setCvCameraViewListener(this)

    }

    override fun onResume() {
        super.onResume()

//        if (!OpenCVLoader.initDebug()) {
//            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION, this, baseLoaderCallback)
//        } else {
            baseLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS)

//        }
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
                System.loadLibrary("opencv_java4")
                binder.cameraview.enableView()

            }

        }
    }

    override fun getCameraViewList(): MutableList<out CameraBridgeViewBase> {
        return Collections.singletonList(binder.cameraview)
    }

    override fun onCameraViewStarted(width: Int, height: Int) {

    }

    override fun onCameraViewStopped() {
    }

    override fun onCameraFrame(inputFrame: CameraBridgeViewBase.CvCameraViewFrame): Mat {
        val rgba = inputFrame.rgba()

        return rgba
    }

}