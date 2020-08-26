#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Empty.h>
#include <ros/package.h>

#include <fstream>
#include <sys/stat.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

/**
 * @brief convert time ros to string 
 * 
 * @param time time ros to convert 
 * @param format format output 
 * @return std::string the time in the format given 
 */
std::string time_to_string(const ros::Time& time,const std::string& format);

/**
 * @brief get the default path data dir i.e.: ros_lidar_recorder/data
 * 
 * @return std::string 
 */
std::string get_default_path_data();

class Recorder {
    private: 
        ros::Subscriber sub;
        std::string path_dir_data; 
        std::string path_dir_session;

        std::string generate_folder_name_from_time(ros::Time time);
        std::string generate_file_name_from_time(ros::Time time);
        
        /**
         * @brief extract the angle of the data, from the position in range
         * 
         * @param angle_rad_min the minimum angle in rad 
         * @param angle_rad_increment delta angle between 2 values 
         * @param index position in range 
         * @return float 
         */
        float extract_angle(const float& angle_rad_min,const float& angle_rad_increment,const int& index);
        
        /**
         * @brief read msg and save data in csv 
         * 
         * @param msg 
         */
        void save_data(const sensor_msgs::LaserScan::ConstPtr& msg);

        /**
         * @brief callback of "scan" topic 
         * 
         * @param msg 
         */
        void callback(const sensor_msgs::LaserScan::ConstPtr& msg);

        /**
         * @brief abstract method given or not access to save_data method 
         * 
         * @return true the data is saved 
         * @return false the data isn't saved 
         */
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
