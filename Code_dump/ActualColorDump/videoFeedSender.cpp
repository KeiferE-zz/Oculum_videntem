/*
Description: Currently this is a VERY BASIC node which will open up a webcam and convert its footgage into ROS IMG msgs and publish them to a topic for others to use. No error handeling is present currently and should eventually be added
*/

#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <ros/ros.h>
#include <string>

cv::VideoCapture cap;

int main(int argc, char** argv){

	ros::init(argc, argv, "videoFeedSender");
	ros::NodeHandle nh;

	image_transport::ImageTransport it(nh);
	image_transport::Publisher publisher = it.advertise("videoInput", 1);

	if( !cap.open(0)) return 0;

	while( nh.ok() ){
		cv::Mat frame;
		cap >> frame;
		if( frame.empty() ) break;

		sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();

		publisher.publish(msg);
		cv::waitKey(1);
		ros::spinOnce();
	}	
	
	return 0;

}
