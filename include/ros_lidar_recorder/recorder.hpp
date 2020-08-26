#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Empty.h>
#include <ros/package.h>

#include <fstream>
#include <sys/stat.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

std::string time_to_string(const ros::Time& time,const std::string& format);

std::string get_default_path_data();

class Recorder {
    private: 
        ros::Subscriber sub;
        std::string path_dir_data; 
        std::string path_dir_session;

        std::string generate_folder_name_from_time(ros::Time time);
        std::string generate_file_name_from_time(ros::Time time);
        float extract_angle(const float& angle_rad_min,const float& angle_rad_increment,const int& index);
        void save_data(const sensor_msgs::LaserScan::ConstPtr& msg);
        void callback(const sensor_msgs::LaserScan::ConstPtr& msg);

        virtual bool guard() = 0;

    public:
        Recorder(ros::NodeHandle& nh,const std::string& topic_scan, std::string& path_dir_data);
};


class RecorderAuto: public Recorder {
    private: 
        int freq_hz;
        ros::Time last_time_record; 

        bool guard();

    public: 
        RecorderAuto(ros::NodeHandle& nh,const std::string& topic_scan, std::string& path_dir_data,int freq_hz);
};

class RecorderEvent: public Recorder {
    private: 
        ros::Subscriber sub_event;
        bool trig_record = false; 
        bool guard();
        void callback(const std_msgs::Empty::ConstPtr& msg);

    public: 
        RecorderEvent(ros::NodeHandle& nh,const std::string& topic_scan, std::string& path_dir_data,const std::string& topic_event);
};
