#include <ros_lidar_recorder/recorder.hpp>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "recorder_event");
  ros::NodeHandle nh_private("~");

  std::string param_path_data = get_default_path_data();
  nh_private.getParam(PARAM_NAME_PATH_DATA,param_path_data);  

  ros::NodeHandle nh;

  RecorderEvent recorder_event(nh,"scan",param_path_data,"record_event");

  ros::spin();

  return 0;
}