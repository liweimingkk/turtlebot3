#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/msg/pose2_d.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>  // Updated to the new header
#include <tf2_ros/buffer.h>
#include <tf2/LinearMath/Quaternion.h>  // Ensure this is included for Quaternion
#include <tf2/LinearMath/Matrix3x3.h>  // Include for getYaw
#include <iostream>
#include <turtlebot3_node/location.hpp>
using namespace std;

double x, y, z;
double theta;
geometry_msgs::msg::Pose2D pos_now;
#123
int main(int argc, char** argv){
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("tf_pose_publisher");

    auto pose_pub = node->create_publisher<geometry_msgs::msg::Pose2D>("pose_data", 10);
    tf2_ros::Buffer tf_buffer(node->get_clock());
    tf2_ros::TransformListener tf_listener(tf_buffer);

    rclcpp::Rate rate(10.0);

    while (rclcpp::ok()){
        auto start = node->get_clock()->now();
        cout << "StartTime: " << start.seconds() << endl;

        geometry_msgs::msg::TransformStamped transform_stamped;
        try {
            transform_stamped = tf_buffer.lookupTransform("map", "base_link", tf2::TimePointZero);
        } catch (tf2::TransformException &ex) {
            RCLCPP_ERROR(node->get_logger(), "%s", ex.what());
            rclcpp::sleep_for(std::chrono::seconds(1));
            continue;
        }

        x = transform_stamped.transform.translation.x;
        y = transform_stamped.transform.translation.y;
        z = transform_stamped.transform.translation.z;

        tf2::Quaternion q(
            transform_stamped.transform.rotation.x,
            transform_stamped.transform.rotation.y,
            transform_stamped.transform.rotation.z,
            transform_stamped.transform.rotation.w
        );

        // Get the yaw (theta) from the quaternion
        tf2::Matrix3x3 m(q);
        double roll, pitch;
        m.getRPY(roll, pitch, theta);

        pos_now.x = x;
        pos_now.y = y;
        pos_now.theta = theta;

        pose_pub->publish(pos_now);

        printf("x: %f, y: %f, z: %f, qx: %f, qy: %f, qz: %f, qw: %f, theta: %f\n", x, y, z, q.x(), q.y(), q.z(), q.w(), pos_now.theta);
        
        rate.sleep();

        auto end = node->get_clock()->now();
        cout << "EndTime: " << end.seconds() << endl;
    }

    rclcpp::shutdown();
    return 0;
}
