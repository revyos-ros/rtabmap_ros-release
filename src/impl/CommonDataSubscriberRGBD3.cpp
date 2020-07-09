/*
Copyright (c) 2010-2016, Mathieu Labbe - IntRoLab - Universite de Sherbrooke
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Universite de Sherbrooke nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <rtabmap_ros/CommonDataSubscriber.h>
#include <rtabmap/utilite/UConversion.h>
#include <rtabmap/core/Compression.h>
#include <rtabmap_ros/MsgConversion.h>
#include <cv_bridge/cv_bridge.h>

namespace rtabmap_ros {

#define IMAGE_CONVERSION() \
		callbackCalled(); \
		std::vector<cv_bridge::CvImageConstPtr> imageMsgs(3); \
		std::vector<cv_bridge::CvImageConstPtr> depthMsgs(3); \
		rtabmap_ros::toCvShare(image1Msg, imageMsgs[0], depthMsgs[0]); \
		rtabmap_ros::toCvShare(image2Msg, imageMsgs[1], depthMsgs[1]); \
		rtabmap_ros::toCvShare(image3Msg, imageMsgs[2], depthMsgs[2]); \
		std::vector<sensor_msgs::CameraInfo> cameraInfoMsgs; \
		cameraInfoMsgs.push_back(image1Msg->rgb_camera_info); \
		cameraInfoMsgs.push_back(image2Msg->rgb_camera_info); \
		cameraInfoMsgs.push_back(image3Msg->rgb_camera_info); \
		std::vector<rtabmap_ros::GlobalDescriptor> globalDescriptorMsgs; \
		std::vector<std::vector<rtabmap_ros::KeyPoint> > localKeyPoints; \
		std::vector<std::vector<rtabmap_ros::Point3f> > localPoints3d; \
		std::vector<cv::Mat> localDescriptors; \
		if(!image1Msg->global_descriptor.data.empty()) \
			globalDescriptorMsgs.push_back(image1Msg->global_descriptor); \
		if(!image2Msg->global_descriptor.data.empty()) \
			globalDescriptorMsgs.push_back(image2Msg->global_descriptor); \
		if(!image3Msg->global_descriptor.data.empty()) \
			globalDescriptorMsgs.push_back(image3Msg->global_descriptor); \
		localKeyPoints.push_back(image1Msg->key_points); \
		localKeyPoints.push_back(image2Msg->key_points); \
		localKeyPoints.push_back(image3Msg->key_points); \
		localPoints3d.push_back(image1Msg->points); \
		localPoints3d.push_back(image2Msg->points); \
		localPoints3d.push_back(image3Msg->points); \
		localDescriptors.push_back(rtabmap::uncompressData(image1Msg->descriptors)); \
		localDescriptors.push_back(rtabmap::uncompressData(image2Msg->descriptors)); \
		localDescriptors.push_back(rtabmap::uncompressData(image3Msg->descriptors));

// 3 RGBD
void CommonDataSubscriber::rgbd3Callback(
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	sensor_msgs::LaserScan scanMsg; // Null
	sensor_msgs::PointCloud2 scan3dMsg; // Null
	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3Scan2dCallback(
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::LaserScanConstPtr& scanMsg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	sensor_msgs::PointCloud2 scan3dMsg; // Null
	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, *scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3Scan3dCallback(
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::PointCloud2ConstPtr& scan3dMsg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	sensor_msgs::LaserScan scanMsg; // Null
	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			*scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3ScanDescCallback(
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const rtabmap_ros::ScanDescriptorConstPtr& scanDescMsg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	if(!scanDescMsg->global_descriptor.data.empty())
	{
		globalDescriptorMsgs.push_back(scanDescMsg->global_descriptor);
	}
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanDescMsg->scan,
			scanDescMsg->scan_cloud, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3InfoCallback(
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	sensor_msgs::LaserScan scanMsg; // Null
	sensor_msgs::PointCloud2 scan3dMsg; // Null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3Scan2dInfoCallback(
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::LaserScanConstPtr& scanMsg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	sensor_msgs::PointCloud2 scan3dMsg; // Null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, *scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3Scan3dInfoCallback(
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::PointCloud2ConstPtr& scan3dMsg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	sensor_msgs::LaserScan scanMsg; // Null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			*scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3ScanDescInfoCallback(
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const rtabmap_ros::ScanDescriptorConstPtr& scanDescMsg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	if(!scanDescMsg->global_descriptor.data.empty())
	{
		globalDescriptorMsgs.push_back(scanDescMsg->global_descriptor);
	}
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanDescMsg->scan,
			scanDescMsg->scan_cloud, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}

// 2 RGBD + Odom
void CommonDataSubscriber::rgbd3OdomCallback(
		const nav_msgs::OdometryConstPtr & odomMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg)
{
	IMAGE_CONVERSION();

	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	sensor_msgs::LaserScan scanMsg; // Null
	sensor_msgs::PointCloud2 scan3dMsg; // Null
	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3OdomScan2dCallback(
		const nav_msgs::OdometryConstPtr & odomMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::LaserScanConstPtr& scanMsg)
{
	IMAGE_CONVERSION();

	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	sensor_msgs::PointCloud2 scan3dMsg; // Null
	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, *scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3OdomScan3dCallback(
		const nav_msgs::OdometryConstPtr & odomMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::PointCloud2ConstPtr& scan3dMsg)
{
	IMAGE_CONVERSION();

	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	sensor_msgs::LaserScan scanMsg; // Null
	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			*scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3OdomScanDescCallback(
		const nav_msgs::OdometryConstPtr & odomMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const rtabmap_ros::ScanDescriptorConstPtr& scanDescMsg)
{
	IMAGE_CONVERSION();

	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	if(!scanDescMsg->global_descriptor.data.empty())
	{
		globalDescriptorMsgs.push_back(scanDescMsg->global_descriptor);
	}
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanDescMsg->scan,
			scanDescMsg->scan_cloud, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3OdomInfoCallback(
		const nav_msgs::OdometryConstPtr & odomMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	sensor_msgs::LaserScan scanMsg; // Null
	sensor_msgs::PointCloud2 scan3dMsg; // Null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3OdomScan2dInfoCallback(
		const nav_msgs::OdometryConstPtr & odomMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::LaserScanConstPtr& scanMsg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	sensor_msgs::PointCloud2 scan3dMsg; // Null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, *scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3OdomScan3dInfoCallback(
		const nav_msgs::OdometryConstPtr & odomMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::PointCloud2ConstPtr& scan3dMsg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	sensor_msgs::LaserScan scanMsg; // Null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			*scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3OdomScanDescInfoCallback(
		const nav_msgs::OdometryConstPtr & odomMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const rtabmap_ros::ScanDescriptorConstPtr& scanDescMsg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	rtabmap_ros::UserDataConstPtr userDataMsg; // Null
	sensor_msgs::LaserScan scanMsg; // Null
	if(!scanDescMsg->global_descriptor.data.empty())
	{
		globalDescriptorMsgs.push_back(scanDescMsg->global_descriptor);
	}
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanDescMsg->scan,
			scanDescMsg->scan_cloud, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}

#ifdef RTABMAP_SYNC_USER_DATA
// 2 RGBD + User Data
void CommonDataSubscriber::rgbd3DataCallback(
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	sensor_msgs::LaserScan scanMsg; // Null
	sensor_msgs::PointCloud2 scan3dMsg; // Null
	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3DataScan2dCallback(
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::LaserScanConstPtr& scanMsg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	sensor_msgs::PointCloud2 scan3dMsg; // Null
	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, *scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3DataScan3dCallback(
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::PointCloud2ConstPtr& scan3dMsg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	sensor_msgs::LaserScan scanMsg; // Null
	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			*scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3DataScanDescCallback(
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const rtabmap_ros::ScanDescriptorConstPtr& scanDescMsg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	if(!scanDescMsg->global_descriptor.data.empty())
	{
		globalDescriptorMsgs.push_back(scanDescMsg->global_descriptor);
	}
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanDescMsg->scan,
			scanDescMsg->scan_cloud, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3DataInfoCallback(
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	sensor_msgs::LaserScan scanMsg; // Null
	sensor_msgs::PointCloud2 scan3dMsg; // Null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3DataScan2dInfoCallback(
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::LaserScanConstPtr& scanMsg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	sensor_msgs::PointCloud2 scan3dMsg; // Null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, *scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3DataScan3dInfoCallback(
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::PointCloud2ConstPtr& scan3dMsg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	sensor_msgs::LaserScan scanMsg; // Null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			*scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3DataScanDescInfoCallback(
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const rtabmap_ros::ScanDescriptorConstPtr& scanDescMsg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	nav_msgs::OdometryConstPtr odomMsg; // Null
	if(!scanDescMsg->global_descriptor.data.empty())
	{
		globalDescriptorMsgs.push_back(scanDescMsg->global_descriptor);
	}
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanDescMsg->scan,
			scanDescMsg->scan_cloud, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}

// 2 RGBD + Odom + User Data
void CommonDataSubscriber::rgbd3OdomDataCallback(
		const nav_msgs::OdometryConstPtr& odomMsg,
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg)
{
	IMAGE_CONVERSION();

	sensor_msgs::LaserScan scanMsg; // Null
	sensor_msgs::PointCloud2 scan3dMsg; // Null
	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3OdomDataScan2dCallback(
		const nav_msgs::OdometryConstPtr& odomMsg,
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::LaserScanConstPtr& scanMsg)
{
	IMAGE_CONVERSION();

	sensor_msgs::PointCloud2 scan3dMsg; // Null
	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, *scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3OdomDataScan3dCallback(
		const nav_msgs::OdometryConstPtr& odomMsg,
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::PointCloud2ConstPtr& scan3dMsg)
{
	IMAGE_CONVERSION();

	sensor_msgs::LaserScan scanMsg; // Null
	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			*scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3OdomDataScanDescCallback(
		const nav_msgs::OdometryConstPtr& odomMsg,
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const rtabmap_ros::ScanDescriptorConstPtr& scanDescMsg)
{
	IMAGE_CONVERSION();

	rtabmap_ros::OdomInfoConstPtr odomInfoMsg; // null
	if(!scanDescMsg->global_descriptor.data.empty())
	{
		globalDescriptorMsgs.push_back(scanDescMsg->global_descriptor);
	}
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanDescMsg->scan,
			scanDescMsg->scan_cloud, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3OdomDataInfoCallback(
		const nav_msgs::OdometryConstPtr& odomMsg,
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	sensor_msgs::LaserScan scanMsg; // Null
	sensor_msgs::PointCloud2 scan3dMsg; // Null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3OdomDataScan2dInfoCallback(
		const nav_msgs::OdometryConstPtr& odomMsg,
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::LaserScanConstPtr& scanMsg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	sensor_msgs::PointCloud2 scan3dMsg; // Null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, *scanMsg,
			scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3OdomDataScan3dInfoCallback(
		const nav_msgs::OdometryConstPtr& odomMsg,
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const sensor_msgs::PointCloud2ConstPtr& scan3dMsg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	sensor_msgs::LaserScan scanMsg; // Null
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanMsg,
			*scan3dMsg, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
void CommonDataSubscriber::rgbd3OdomDataScanDescInfoCallback(
		const nav_msgs::OdometryConstPtr& odomMsg,
		const rtabmap_ros::UserDataConstPtr& userDataMsg,
		const rtabmap_ros::RGBDImageConstPtr& image1Msg,
		const rtabmap_ros::RGBDImageConstPtr& image2Msg,
		const rtabmap_ros::RGBDImageConstPtr& image3Msg,
		const rtabmap_ros::ScanDescriptorConstPtr& scanDescMsg,
		const rtabmap_ros::OdomInfoConstPtr& odomInfoMsg)
{
	IMAGE_CONVERSION();

	if(!scanDescMsg->global_descriptor.data.empty())
	{
		globalDescriptorMsgs.push_back(scanDescMsg->global_descriptor);
	}
	commonDepthCallback(odomMsg, userDataMsg, imageMsgs,
			depthMsgs, cameraInfoMsgs, scanDescMsg->scan,
			scanDescMsg->scan_cloud, odomInfoMsg, globalDescriptorMsgs,
			localKeyPoints, localPoints3d, localDescriptors);
}
#endif

void CommonDataSubscriber::setupRGBD3Callbacks(
		ros::NodeHandle & nh,
		ros::NodeHandle & pnh,
		bool subscribeOdom,
		bool subscribeUserData,
		bool subscribeScan2d,
		bool subscribeScan3d,
		bool subscribeScanDescriptor,
		bool subscribeOdomInfo,
		int queueSize,
		bool approxSync)
{
	ROS_INFO("Setup rgbd3 callback");

	rgbdSubs_.resize(3);
	for(int i=0; i<3; ++i)
	{
		rgbdSubs_[i] = new message_filters::Subscriber<rtabmap_ros::RGBDImage>;
		rgbdSubs_[i]->subscribe(nh, uFormat("rgbd_image%d", i), 1);
	}
#ifdef RTABMAP_SYNC_USER_DATA
	if(subscribeOdom && subscribeUserData)
	{
		odomSub_.subscribe(nh, "odom", 1);
		userDataSub_.subscribe(nh, "user_data", 1);
		if(subscribeScanDescriptor)
		{
			subscribedToScanDescriptor_ = true;
			scanDescSub_.subscribe(nh, "scan_descriptor", 1);
			if(subscribeOdomInfo)
			{
				subscribedToOdomInfo_ = true;
				odomInfoSub_.subscribe(nh, "odom_info", 1);
				SYNC_DECL7(rgbd3OdomDataScanDescInfo, approxSync, queueSize, odomSub_, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanDescSub_, odomInfoSub_);
			}
			else
			{
				SYNC_DECL6(rgbd3OdomDataScanDesc, approxSync, queueSize, odomSub_, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanDescSub_);
			}
		}
		else if(subscribeScan2d)
		{
			subscribedToScan2d_ = true;
			scanSub_.subscribe(nh, "scan", 1);
			if(subscribeOdomInfo)
			{
				subscribedToOdomInfo_ = true;
				odomInfoSub_.subscribe(nh, "odom_info", 1);
				SYNC_DECL7(rgbd3OdomDataScan2dInfo, approxSync, queueSize, odomSub_, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanSub_, odomInfoSub_);
			}
			else
			{
				SYNC_DECL6(rgbd3OdomDataScan2d, approxSync, queueSize, odomSub_, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanSub_);
			}
		}
		else if(subscribeScan3d)
		{
			subscribedToScan3d_ = true;
			scan3dSub_.subscribe(nh, "scan_cloud", 1);
			if(subscribeOdomInfo)
			{
				subscribedToOdomInfo_ = true;
				odomInfoSub_.subscribe(nh, "odom_info", 1);
				SYNC_DECL7(rgbd3OdomDataScan3dInfo, approxSync, queueSize, odomSub_, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scan3dSub_, odomInfoSub_);
			}
			else
			{
				SYNC_DECL6(rgbd3OdomDataScan3d, approxSync, queueSize, odomSub_, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scan3dSub_);
			}
		}
		else if(subscribeOdomInfo)
		{
			subscribedToOdomInfo_ = true;
			odomInfoSub_.subscribe(nh, "odom_info", 1);
			SYNC_DECL6(rgbd3OdomDataInfo, approxSync, queueSize, odomSub_, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), odomInfoSub_);
		}
		else
		{
			SYNC_DECL5(rgbd3OdomData, approxSync, queueSize, odomSub_, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]));
		}
	}
	else
#endif		
	if(subscribeOdom)
	{
		odomSub_.subscribe(nh, "odom", 1);
		if(subscribeScanDescriptor)
		{
			subscribedToScanDescriptor_ = true;
			scanDescSub_.subscribe(nh, "scan_descriptor", 1);
			if(subscribeOdomInfo)
			{
				subscribedToOdomInfo_ = true;
				odomInfoSub_.subscribe(nh, "odom_info", 1);
				SYNC_DECL6(rgbd3OdomScanDescInfo, approxSync, queueSize, odomSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanDescSub_, odomInfoSub_);
			}
			else
			{
				SYNC_DECL5(rgbd3OdomScanDesc, approxSync, queueSize, odomSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanDescSub_);
			}
		}
		else if(subscribeScan2d)
		{
			subscribedToScan2d_ = true;
			scanSub_.subscribe(nh, "scan", 1);
			if(subscribeOdomInfo)
			{
				subscribedToOdomInfo_ = true;
				odomInfoSub_.subscribe(nh, "odom_info", 1);
				SYNC_DECL6(rgbd3OdomScan2dInfo, approxSync, queueSize, odomSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanSub_, odomInfoSub_);
			}
			else
			{
				SYNC_DECL5(rgbd3OdomScan2d, approxSync, queueSize, odomSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanSub_);
			}
		}
		else if(subscribeScan3d)
		{
			subscribedToScan3d_ = true;
			scan3dSub_.subscribe(nh, "scan_cloud", 1);
			if(subscribeOdomInfo)
			{
				subscribedToOdomInfo_ = true;
				odomInfoSub_.subscribe(nh, "odom_info", 1);
				SYNC_DECL6(rgbd3OdomScan3dInfo, approxSync, queueSize, odomSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scan3dSub_, odomInfoSub_);
			}
			else
			{
				SYNC_DECL5(rgbd3OdomScan3d, approxSync, queueSize, odomSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scan3dSub_);
			}
		}
		else if(subscribeOdomInfo)
		{
			subscribedToOdomInfo_ = true;
			odomInfoSub_.subscribe(nh, "odom_info", 1);
			SYNC_DECL5(rgbd3OdomInfo, approxSync, queueSize, odomSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), odomInfoSub_);
		}
		else
		{
			SYNC_DECL4(rgbd3Odom, approxSync, queueSize, odomSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]));
		}
	}
#ifdef RTABMAP_SYNC_USER_DATA
	else if(subscribeUserData)
	{
		userDataSub_.subscribe(nh, "user_data", 1);
		if(subscribeScanDescriptor)
		{
			subscribedToScanDescriptor_ = true;
			scanDescSub_.subscribe(nh, "scan_descriptor", 1);
			if(subscribeOdomInfo)
			{
				subscribedToOdomInfo_ = true;
				odomInfoSub_.subscribe(nh, "odom_info", 1);
				SYNC_DECL6(rgbd3DataScanDescInfo, approxSync, queueSize, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanDescSub_, odomInfoSub_);
			}
			else
			{
				SYNC_DECL5(rgbd3DataScanDesc, approxSync, queueSize, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanDescSub_);
			}
		}
		else if(subscribeScan2d)
		{
			subscribedToScan2d_ = true;
			scanSub_.subscribe(nh, "scan", 1);
			if(subscribeOdomInfo)
			{
				subscribedToOdomInfo_ = true;
				odomInfoSub_.subscribe(nh, "odom_info", 1);
				SYNC_DECL6(rgbd3DataScan2dInfo, approxSync, queueSize, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanSub_, odomInfoSub_);
			}
			else
			{
				SYNC_DECL5(rgbd3DataScan2d, approxSync, queueSize, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanSub_);
			}
		}
		else if(subscribeScan3d)
		{
			subscribedToScan3d_ = true;
			scan3dSub_.subscribe(nh, "scan_cloud", 1);
			if(subscribeOdomInfo)
			{
				subscribedToOdomInfo_ = true;
				odomInfoSub_.subscribe(nh, "odom_info", 1);
				SYNC_DECL6(rgbd3DataScan3dInfo, approxSync, queueSize, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scan3dSub_, odomInfoSub_);
			}
			else
			{
				SYNC_DECL5(rgbd3DataScan3d, approxSync, queueSize, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scan3dSub_);
			}
		}
		else if(subscribeOdomInfo)
		{
			subscribedToOdomInfo_ = true;
			odomInfoSub_.subscribe(nh, "odom_info", 1);
			SYNC_DECL5(rgbd3DataInfo, approxSync, queueSize, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), odomInfoSub_);
		}
		else
		{
			SYNC_DECL4(rgbd3Data, approxSync, queueSize, userDataSub_, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]));
		}
	}
#endif
	else
	{
		if(subscribeScanDescriptor)
		{
			subscribedToScanDescriptor_ = true;
			scanDescSub_.subscribe(nh, "scan_descriptor", 1);
			if(subscribeOdomInfo)
			{
				subscribedToOdomInfo_ = true;
				odomInfoSub_.subscribe(nh, "odom_info", 1);
				SYNC_DECL5(rgbd3ScanDescInfo, approxSync, queueSize, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanDescSub_, odomInfoSub_);
			}
			else
			{
				SYNC_DECL4(rgbd3ScanDesc, approxSync, queueSize, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanDescSub_);
			}
		}
		else if(subscribeScan2d)
		{
			subscribedToScan2d_ = true;
			scanSub_.subscribe(nh, "scan", 1);
			if(subscribeOdomInfo)
			{
				subscribedToOdomInfo_ = true;
				odomInfoSub_.subscribe(nh, "odom_info", 1);
				SYNC_DECL5(rgbd3Scan2dInfo, approxSync, queueSize, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanSub_, odomInfoSub_);
			}
			else
			{
				SYNC_DECL4(rgbd3Scan2d, approxSync, queueSize, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scanSub_);
			}
		}
		else if(subscribeScan3d)
		{
			subscribedToScan3d_ = true;
			scan3dSub_.subscribe(nh, "scan_cloud", 1);
			if(subscribeOdomInfo)
			{
				subscribedToOdomInfo_ = true;
				odomInfoSub_.subscribe(nh, "odom_info", 1);
				SYNC_DECL5(rgbd3Scan3dInfo, approxSync, queueSize, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scan3dSub_, odomInfoSub_);
			}
			else
			{
				SYNC_DECL4(rgbd3Scan3d, approxSync, queueSize, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), scan3dSub_);
			}
		}
		else if(subscribeOdomInfo)
		{
			subscribedToOdomInfo_ = true;
			odomInfoSub_.subscribe(nh, "odom_info", 1);
			SYNC_DECL4(rgbd3Info, approxSync, queueSize, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]), odomInfoSub_);
		}
		else
		{
			SYNC_DECL3(rgbd3, approxSync, queueSize, (*rgbdSubs_[0]), (*rgbdSubs_[1]), (*rgbdSubs_[2]));
		}
	}
}

} /* namespace rtabmap_ros */
