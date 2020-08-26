#include <ros_lidar_recorder/recorder.hpp>

std::string time_to_string(const ros::Time& time,const std::string& format){
    auto current_time = time.toBoost();
    std::stringstream ss;
    auto facet = new boost::posix_time::time_facet(format.c_str());
    ss.imbue(std::locale(std::locale::classic(), facet));
    ss << current_time;
    return ss.str();
}

std::string get_default_path_data(){
    return ros::package::getPath("ros_lidar_recorder") + "/data/";
}






std::string Recorder::generate_folder_name_from_time(ros::Time time){
        return time_to_string(time,"%Y%m%d-%H_%M_%s") + "/";
}

std::string Recorder::generate_file_name_from_time(ros::Time time){
    return time_to_string(time,"%Y%m%d-%H_%M_%s") + ".csv";
}

float Recorder::extract_angle(const float& angle_rad_min,const float& angle_rad_increment,const int& index){
    return angle_rad_min + angle_rad_increment * index;
}

void Recorder::save_data(const sensor_msgs::LaserScan::ConstPtr& msg){
    // extract config information
    const float& angle_rad_min = msg->angle_min;
    const float& angle_rad_increment = msg->angle_increment; 

    std::string name_file = generate_file_name_from_time(ros::Time::now());
    std::string path_file = this->path_dir_session + name_file;

    std::ofstream file;
    file.open(path_file);
    // define for each LaserPoint the angle from the position in the array
    for(int i=0; i < msg->ranges.size(); i++){
        const float& distance_m = msg->ranges[i];
        float angle_rad = this->extract_angle(angle_rad_min,angle_rad_increment,i);

        file << angle_rad;
        file << ";";
        file << distance_m;
        file << "\n";
    }
    file.close();
}

void Recorder::callback(const sensor_msgs::LaserScan::ConstPtr& msg){
    if(guard()){
        this->save_data(msg);
    }
}

Recorder::Recorder(ros::NodeHandle& nh,const std::string& topic_scan, std::string& path_dir_data):path_dir_data(path_dir_data){
    // create Subscriber
    this->sub = nh.subscribe<sensor_msgs::LaserScan>(topic_scan, 5, &Recorder::callback ,this);

    // create dir data 
    this->path_dir_session = this->path_dir_data + this->generate_folder_name_from_time(ros::Time::now());
    int status = mkdir(this->path_dir_session.c_str(),0777);
    if (status){
        ROS_ERROR("can't create folder: %s", this->path_dir_session.c_str());
        ROS_ERROR("please verify if %s exists",this->path_dir_data.c_str());
        ros::shutdown();
    }else{
        ROS_INFO("folder %s created",this->path_dir_session.c_str()); 
        ROS_INFO("recording session started");
    }
}     






bool RecorderAuto::guard(){
    ros::Duration diff=ros::Time::now()-last_time_record;
    return diff.toSec() >= 1.0/this->freq_hz;
}

RecorderAuto::RecorderAuto(ros::NodeHandle& nh,const std::string& topic_scan, std::string& path_dir_data,int freq_hz):
Recorder(nh,topic_scan,path_dir_data),freq_hz(freq_hz){
    this->last_time_record = ros::Time::now();
}



bool RecorderEvent::guard(){
    if(trig_record){
        trig_record = false; 
        return true;
    }
    return false; 
}

void RecorderEvent::callback(const std_msgs::Empty::ConstPtr& msg){
    trig_record = true;
}

RecorderEvent::RecorderEvent(ros::NodeHandle& nh,const std::string& topic_scan, std::string& path_dir_data,const std::string& topic_event):
Recorder(nh,topic_scan,path_dir_data){
    this->sub_event = nh.subscribe<std_msgs::Empty>(topic_event, 5, &RecorderEvent::callback ,this);
}
