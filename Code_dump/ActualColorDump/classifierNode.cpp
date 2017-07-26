#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include "std_msgs/String.h"
#define FOCAL 187*950/190 //468
#define WIDTH 190

using namespace cv; using namespace std;

String object_cascade_name = "buoy_cascade.xml"; //currently duck
CascadeClassifier object_cascade;


void imageCallback(const sensor_msgs::ImageConstPtr& msg){
	std::vector<Rect> object;
	Mat frame_gray;
	cv_bridge::CvImagePtr cv_ptr;
	try{
		cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	}
	catch (cv_bridge::Exception e){
		ROS_ERROR("couldn't convert from '%s' to 'bgr8'.", msg->encoding.c_str());
	}

	Mat frame = cv_ptr->image; 

	
		//cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
	//	Mat frame = cv_bridge::toCvShare(msg, "bgr8");
		cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
		equalizeHist( frame_gray, frame_gray );
		object_cascade.detectMultiScale( frame_gray, object, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
			for( size_t i = 0; i < object.size(); i++){
			Point center( object[i].x + object[i].width/2, object[i].y + object[i].height/2 );
			ellipse( frame, center, Size( object[i].width/2, object[i].height/2 ), 0, 0 , 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
		

		float focal = FOCAL ;
	//	float distance = (((190)/(0.4647))*object[i].width);
		float distance = (((WIDTH * focal) / (object[i].width) ) /10);

		float xCoord = (((center.x - frame.size().width/2) * distance) / focal );
		float yCoord = (((frame.size().height/2 - center.y) * distance) / focal );

		std::stringstream textss;
		textss << "("<<" "<< round(xCoord) << "cm," << round(yCoord) << "cm," << round(distance) <<"cm)";
		putText(frame, textss.str().c_str(), center, FONT_HERSHEY_PLAIN, 1.2, Scalar(0, 0, 255), 2);

		std_msgs::String msg;
		std::stringstream ss;
		ss << "The x coord is: " << xCoord << "The y coord is: " << yCoord;
	//	msg.data = ss.str();
//		ROS_INFO("%s", msg.data.c_str());
	//	pos_pub.publish(msg);
		}	
		imshow( "view", frame );
		cv::waitKey(30);
	
//	catch(cv_bridge::Exception& e){
//		ROS_ERROR("could not convert from '%s' to 'brg8'.", msg->encoding.c_str());
//	return ss;
}

int main(int argc, char** argv){
	ros::init(argc, argv, "image_reciever");
	ros::NodeHandle nh;

	if( !object_cascade.load( object_cascade_name ) ){
		return -1;
	}

	cv::namedWindow("view");
	cv::startWindowThread();
//	ros::Publisher ai_pub = n.advertise<std_msgs::String>("aiInfo", 1000);
	ros::Publisher pos_pub = nh.advertise<std_msgs::String>("ai", 1000);	
	

	image_transport::ImageTransport it(nh);
	image_transport::Subscriber sub = it.subscribe("videoInput", 1, imageCallback);
//	ros::Publisher ai_pub = n.advertise<std_msgs::String>("aiInfo", 1000);
	ros::spin();
	cv::destroyWindow("view");
	return 0;
	
}
