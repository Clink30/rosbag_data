#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <sensor_msgs/Imu.h>

#include <string>
#include <fstream>

int main(int argc, char **argv)
{
    ros::init(argc,argv,"get_data");
    ros::NodeHandle nh;

    rosbag::Bag mybag;
    std::string bagpath;
    //nh.getParam("BAGPATH",bagpath,"/home/xh_cai/Project/XhProject/openvins_nx_debug/rosbag/2022_11_1/trace_2022-11-01-13-41-45.bag");
    mybag.open("/home/xh_cai/Rosbag/20230314/imu_mag_img_2023-03-14-16-50-17.bag",rosbag::bagmode::Read);
    std::vector<std::string> topics;

    topics.push_back(std::string("/fdi_imu"));
    topics.push_back(std::string("/mavros/imu/data"));

    rosbag::View view(mybag,rosbag::TopicQuery(topics));

    rosbag::View::iterator it = view.begin();

    for(; it != view.end(); ++it)
    {
        auto m = *it;
        std::string topic = m.getTopic();

        if(topic == "/fdi_imu")
        {
            sensor_msgs::Imu::ConstPtr fdimu = m.instantiate<sensor_msgs::Imu>();
            std::ofstream foutt265("/home/xh_cai/Project/XhProject/getimudata_ws/src/out/fdimu.txt",std::ios::app);
            foutt265.setf(std::ios::fixed,std::ios::floatfield);
            foutt265.precision(0);
            foutt265 << fdimu->header.stamp.toSec()*1e3 << " ";
            foutt265.precision(5);   
            foutt265 << fdimu->angular_velocity.x<< " "
                     << fdimu->angular_velocity.y << " "
                     << fdimu->angular_velocity.z << " "
                     << fdimu->linear_acceleration.x << " "
                     << fdimu->linear_acceleration.y << " "
                     << fdimu->linear_acceleration.z <<std::endl;
            foutt265.close();

        }
        if(topic == "/mavros/imu/data")
        {
            sensor_msgs::Imu::ConstPtr  mavrosimu = m.instantiate<sensor_msgs::Imu>();
            std::ofstream foutvins("/home/xh_cai/Project/XhProject/getimudata_ws/src/out/mavrosimu.txt",std::ios::app);
            foutvins.setf(std::ios::fixed,std::ios::floatfield);
            foutvins.precision(0);
            foutvins << mavrosimu->header.stamp.toSec()*1e3 << " ";
            foutvins.precision(5);   
            foutvins << mavrosimu->angular_velocity.x<< " "
                     << mavrosimu->angular_velocity.y << " "
                     << mavrosimu->angular_velocity.z << " "
                     << mavrosimu->linear_acceleration.x << " "
                     << mavrosimu->linear_acceleration.y << " "
                     << mavrosimu->linear_acceleration.z <<std::endl;
            foutvins.close();

        }
    }
    mybag.close();

}
