#include <iostream>
#include <string.h>
#include <thread>

#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

#include "ps4eye.h"

using namespace ps4eye;

bool isRunning = true;

void update() {
	while (isRunning) {
		if (!ps4eye::PS4EYECam::updateDevices())
			break;
	}
}

int main() {



	cv::Mat yuv, bgr;

	ps4eye::PS4EYECam::PS4EYERef eye;

	uint8_t *frame_rgb_left;
	unsigned char * videoFrame;


	bool finish = false;

	eyeframe *frame;

	std::vector<PS4EYECam::PS4EYERef> devices(PS4EYECam::getDevices());

	if (devices.size() == 1) {
		eye = devices.at(0);



		eye->firmware_path = "firmware.bin";

		eye->firmware_upload();


		bool res = eye->init(2, 120);
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

	std::thread updateThread(update);



	while (!finish) {

		if (eye) {
			bool isNewFrame = eye->isNewFrame();
			//std::cout << isNewFrame << std::endl;
			//isNewFrame = true;
			if (isNewFrame) {
				std::cout << isNewFrame << std::endl;
				

				eye->check_ff71();
				frame = eye->getLastVideoFramePointer();


				yuv = cv::Mat(eye->getHeight(), eye->getWidth(), CV_8UC2, frame->videoLeftFrame);

				cv::cvtColor(yuv, bgr, cv::COLOR_YUV2BGRA_YUY2);

				imshow("left", bgr);

				int keyCode = cv::waitKey(1);


			}


		}

		else {
			finish = true;
		}
	}

	updateThread.join();

//	eye->set_led_off();


	if (eye) {
		std::cout << "Shutdown begin wait..." << std::endl;

		eye->shutdown();

	}

	return 0;
}
