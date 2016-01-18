#include <iostream>
#include <string.h>
#include <thread>

#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

#include "ps4eye.h"

using namespace ps4eye;

bool isRunning = true;

void yuv2rgb(int y, int u, int v, char *r, char *g, char *b) {
	int r1, g1, b1;
	int c = y - 16, d = u - 128, e = v - 128;

	r1 = (298 * c + 409 * e + 128) >> 8;
	g1 = (298 * c - 100 * d - 208 * e + 128) >> 8;
	b1 = (298 * c + 516 * d + 128) >> 8;

	// Even with proper conversion, some values still need clipping.

	if (r1 > 255)
		r1 = 255;
	if (g1 > 255)
		g1 = 255;
	if (b1 > 255)
		b1 = 255;
	if (r1 < 0)
		r1 = 0;
	if (g1 < 0)
		g1 = 0;
	if (b1 < 0)
		b1 = 0;

	*r = r1;
	*g = g1;
	*b = b1;
}

void yuyvToRgb(uint8_t *in, uint8_t *out, int size_x, int size_y) {
	int i;
	unsigned int *pixel_16 = (unsigned int*) in;
	;     // for YUYV
	unsigned char *pixel_24 = out;    // for RGB
	int y, u, v, y2;
	char r, g, b;

	for (i = 0; i < (size_x * size_y / 2); i++) {
		// read YuYv from newBuffer (2 pixels) and build RGBRGB in pBuffer

		//   v  = ((*pixel_16 & 0x000000ff));
		// y  = ((*pixel_16 & 0x0000ff00)>>8);
		// u  = ((*pixel_16 & 0x00ff0000)>>16);
		// y2 = ((*pixel_16 & 0xff000000)>>24);

		y2 = ((*pixel_16 & 0x000000ff));
		u = ((*pixel_16 & 0x0000ff00) >> 8);
		y = ((*pixel_16 & 0x00ff0000) >> 16);
		v = ((*pixel_16 & 0xff000000) >> 24);

		yuv2rgb(y, u, v, &r, &g, &b);            // 1st pixel

		*pixel_24++ = r;
		*pixel_24++ = g;
		*pixel_24++ = b;

		yuv2rgb(y2, u, v, &r, &g, &b);            // 2nd pixel

		*pixel_24++ = r;
		*pixel_24++ = g;
		*pixel_24++ = b;

		pixel_16++;
	}
}

void update() {
	while (isRunning) {
		if (!ps4eye::PS4EYECam::updateDevices())
			break;
	}
}



void test() {

	cv::Mat frame;
	cv::VideoCapture cap(0);

	while (1) {
		cap >> frame;
		imshow("test", frame);
	}

}


int main() {

	//test();

	cv::Mat yuv, bgr;

	ps4eye::PS4EYECam::PS4EYERef eye;

	uint8_t *frame_rgb_left;
	unsigned char * videoFrame;


	bool finish = false;

	eyeframe *frame;

	std::vector<PS4EYECam::PS4EYERef> devices(PS4EYECam::getDevices());

	if (devices.size() == 1) {
		eye = devices.at(0);

//		if (!eye->firmwareisloaded) {
//
//			eye->firmware_path = "firmware.bin";
//
//			eye->firmware_upload();
//
//			std::cout << "firmware uploaded" << std::endl;
//		}

		eye->firmware_path = "firmware.bin";

		eye->firmware_upload();


		bool res = eye->init(1, 120);
		std::cout << res << std::endl;

		eye->start();

		yuv = cv::Mat(eye->getHeight(), eye->getWidth(), CV_8UC2, cv::Scalar(0, 0));

		bgr = cv::Mat(eye->getHeight(), eye->getWidth(), CV_8UC3, cv::Scalar(0, 0, 0));

		frame_rgb_left = new uint8_t[eye->getWidth() * eye->getHeight() * 3];
		memset(frame_rgb_left, 0, eye->getWidth() * eye->getHeight() * 3);

		videoFrame = new unsigned char[eye->getWidth() * eye->getHeight() * 3];

	}

//	eye->set_led_on();

	cv::namedWindow("left");

	cv::namedWindow("right");

	std::thread updateThread(update);


//	eye->start_sensors_streaming();
	while (!finish) {
//		std::cout << updated << std::endl;
		if (eye) {
			bool isNewFrame = eye->isNewFrame();
			//std::cout << isNewFrame << std::endl;
			//isNewFrame = true;
			if (isNewFrame) {
				std::cout << isNewFrame << std::endl;
				

				eye->check_ff71();
				frame = eye->getLastVideoFramePointer();

//		yuv.data = frame->videoLeftFrame;
				yuv = cv::Mat(eye->getHeight(), eye->getWidth(), CV_8UC2, frame->videoLeftFrame);

				cv::cvtColor(yuv, bgr, cv::COLOR_YUV2BGRA_YUY2);

				imshow("left", bgr);

				yuv = cv::Mat(eye->getHeight(), eye->getWidth(), CV_8UC2, frame->videoRightFrame);

				cv::cvtColor(yuv, bgr, cv::COLOR_YUV2BGRA_YUY2);

				imshow("right", bgr);



				int keyCode = cv::waitKey(1);

//				if (!eye->rightflag) {




//			std::cout << "here: " << isNewFrame << std::endl;

			}


		}

		else {
			finish = true;
		}
	}

	updateThread.join();

//	eye->set_led_off();
//	eye->stop_sensors_streaming();

	if (eye) {
		std::cout << "Shutdown begin wait..." << std::endl;
//		eye->stop();
		eye->shutdown();
		//
	}

	return 0;
}
