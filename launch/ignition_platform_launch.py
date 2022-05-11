from os.path import join

import launch
from launch import LaunchDescription, conditions
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, EnvironmentVariable
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    config = join(
        get_package_share_directory('ignition_platform'),
        'config',
        'control_modes.yaml'
    )
    return LaunchDescription([
        DeclareLaunchArgument('drone_id', default_value='drone_sim_rafa_0'),
        DeclareLaunchArgument('mass', default_value='1.0'),
        DeclareLaunchArgument('control_modes_file', default_value=config),
        
        Node(
            package="ignition_platform",
            executable="ignition_platform_node",
            # name="platform",
            namespace=LaunchConfiguration('drone_id'),
            output="screen",
            emulate_tty=True,
            parameters=[
                {"control_modes_file": LaunchConfiguration('control_modes_file')
                }],
            remappings=[("sensor_measurements/odometry", "self_localization/odom")]
        ),
    ])
