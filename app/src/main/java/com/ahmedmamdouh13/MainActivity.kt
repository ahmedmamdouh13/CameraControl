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

class MainActivity : CameraActivity(), CameraBridgeViewBase.CvCameraViewListener2 {


    private lateinit var binder: ActivityMainBinding


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binder = ActivityMainBinding.inflate(LayoutInflater.from(this))
        setContentView(binder.root)
        binder.cameraview.visibility = View.VISIBLE

        binder.cameraview.setCvCameraViewListener(this)

        setOnSwitchButtonClicked()
    }

    private var isGray = false

    private fun setOnSwitchButtonClicked() {
        binder.switchButton.setOnClickListener {
            isGray = !isGray

            binder.switchButton.text = if (isGray) "RGB" else "Gray"
        }
    }

    override fun onResume() {
        super.onResume()

//        if (!OpenCVLoader.initDebug()) {
//            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION, this, baseLoaderCallback)
//        } else {
        baseLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS)

//        }
    }


    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }

    external fun drawFeatures(matRGB: Long, matGray: Long)


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
        val rgba = if (!isGray) inputFrame.rgba() else inputFrame.gray()

        drawFeatures(rgba.nativeObjAddr, inputFrame.gray().nativeObjAddr)

        return rgba
    }

}