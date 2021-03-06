/*!*******************************************************************************************
 *  \file       ignition_platform.hpp
 *  \brief      Implementation of an Ignition Gazebo UAV platform
 *  \authors    Miguel Fernández Cortizas
 *              Pedro Arias Pérez
 *              David Pérez Saura
 *              Rafael Pérez Seguí
 *
 *  \copyright  Copyright (c) 2022 Universidad Politécnica de Madrid
 *              All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ********************************************************************************/

#ifndef IGNITION_PLATFORM_HPP_
#define IGNITION_PLATFORM_HPP_

#include <memory>
#include <string>
#include <iostream>
#include <memory>
#include <rclcpp/logging.hpp>

#include <unordered_map>
#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>
#include <math.h>
#include <as2_core/core_functions.hpp>
#include <as2_core/aerial_platform.hpp>
#include <as2_core/frame_utils/frame_utils.hpp>
#include <as2_core/sensor.hpp>
#include <as2_core/names/topics.hpp>
#include <as2_core/tf_utils.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <geometry_msgs/msg/transform.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <image_transport/image_transport.hpp>
#include "sensor_msgs/msg/nav_sat_fix.hpp"
#include <tf2/exceptions.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/static_transform_broadcaster.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/transform_listener.h>

#include "ignition_bridge.hpp"

#define CMD_FREQ 10  // miliseconds

namespace ignition_platform
{
    using Vector3d = Eigen::Vector3d;

    class IgnitionPlatform : public as2::AerialPlatform
    {
    public:
        IgnitionPlatform();
        ~IgnitionPlatform(){};

    public:

        void configureSensors() override;
        bool ownSendCommand() override;
        bool ownSetArmingState(bool state) override;
        bool ownSetOffboardControl(bool offboard) override;
        bool ownSetPlatformControlMode(const as2_msgs::msg::ControlMode &msg) override;

        static std::unique_ptr<as2::sensors::Sensor<geometry_msgs::msg::PoseStamped>> pose_ptr_;
        static void poseCallback(geometry_msgs::msg::PoseStamped &msg);
        
        static std::unordered_map<std::string, bool> callbacks_tf_;
        static void poseStaticCallback(tf2_msgs::msg::TFMessage &msg);
        static bool checkTf(const std::string &sensor_name);

        static std::unique_ptr<as2::sensors::Sensor<nav_msgs::msg::Odometry>> odometry_raw_estimation_ptr_;
        static void odometryCallback(nav_msgs::msg::Odometry &msg);

        static std::unique_ptr<as2::sensors::Sensor<sensor_msgs::msg::Imu>> imu_ptr_;
        static void imuSensorCallback(sensor_msgs::msg::Imu &msg);

        static std::unique_ptr<as2::sensors::Sensor<sensor_msgs::msg::FluidPressure>> air_pressure_ptr_;
        static void airPressureSensorCallback(sensor_msgs::msg::FluidPressure &msg);

        static std::unique_ptr<as2::sensors::Sensor<sensor_msgs::msg::MagneticField>> magnetometer_ptr_;
        static void magnetometerSensorCallback(sensor_msgs::msg::MagneticField &msg);

        static std::unordered_map<std::string, as2::sensors::Camera> callbacks_camera_;
        static void cameraCallback(sensor_msgs::msg::Image &msg, const std::string &sensor_name);
        static void cameraInfoCallback(sensor_msgs::msg::CameraInfo &msg, const std::string &sensor_name);
        static void cameraTFCallback(geometry_msgs::msg::TransformStamped &msg, const std::string &sensor_name);

        static std::unordered_map<std::string, as2::sensors::Sensor<sensor_msgs::msg::LaserScan>> callbacks_laser_scan_;
        static void laserScanCallback(sensor_msgs::msg::LaserScan &msg, const std::string &sensor_name);
        static std::unordered_map<std::string, as2::sensors::Sensor<sensor_msgs::msg::PointCloud2>> callbacks_point_cloud_;
        static void pointCloudCallback(sensor_msgs::msg::PointCloud2 &msg, const std::string &sensor_name);
        static void lidarTFCallback(geometry_msgs::msg::TransformStamped &msg, const std::string &sensor_name);

        static std::unordered_map<std::string, as2::sensors::Sensor<sensor_msgs::msg::NavSatFix>> callbacks_gps_;
        static void gpsCallback(sensor_msgs::msg::NavSatFix &msg, const std::string &sensor_name);
        static void gpsTFCallback(geometry_msgs::msg::TransformStamped &msg, const std::string &sensor_name);

        static std::unordered_map<std::string, as2::sensors::Sensor<sensor_msgs::msg::Imu>> callbacks_imu_;
        static void imuCallback(sensor_msgs::msg::Imu &msg, const std::string &sensor_name);
        static void imuTFCallback(geometry_msgs::msg::TransformStamped &msg, const std::string &sensor_name);

    private:
        static std::shared_ptr<IgnitionBridge> ignition_bridge_;
        static bool odometry_info_received_;
        as2_msgs::msg::ControlMode control_in_;
        static geometry_msgs::msg::Quaternion self_orientation_;
        double yaw_rate_limit_ = M_PI_2;
        static std::string namespace_;

    private:
        void resetCommandTwistMsg();
    };
}

#endif // IGNITION_PLATFORM_HPP_
