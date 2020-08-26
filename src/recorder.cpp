#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"

#include <fstream>


class Recorder {
    private: 
        ros::Subscriber sub;
        std::string path_dir_data;
        // TODO: find a way to crate file name and concat with path_dir_data
        int freq_hz; 

        float extract_angle(const float& angle_rad_min,const float& angle_rad_increment,const int& index){
            return angle_rad_min + angle_rad_increment * index;
        }

        void callback(const sensor_msgs::LaserScan::ConstPtr& msg){
            // extract config information
            const float& angle_rad_min = msg->angle_min;
            const float& angle_rad_increment = msg->angle_increment; 

            // define for each LaserPoint the angle from the position in the array
            std::ofstream file;
            file.open("/home/hugo/catkin_ws/src/ros_lidar_recorder/data/example.csv");
            for(int i=0; i < msg->ranges.size(); i++){
                const float& distance_m = msg->ranges[i];
                float angle_rad = this->extract_angle(angle_rad_min,angle_rad_increment,i);
                // ros::Time begin = ros::Time::now();

                ROS_INFO("distance %f",distance_m);
                // file << distance_m;
                // file << ";";
                // file << angle_rad;
                // file << "\n";
            }
            // file.close();
        }


    public:

        Recorder(ros::NodeHandle& nh,const std::string& topic_scan){
            this->sub = nh.subscribe<sensor_msgs::LaserScan>(topic_scan, 5, &Recorder::callback ,this);
        }     

};


int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle nh;

  Recorder recorder(nh,"scan");

  ros::Rate r(10);
  while(ros::ok()){
      ros::spinOnce();
      r.sleep();
  }

  return 0;
}
