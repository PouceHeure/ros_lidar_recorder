#include <ros_lidar_recorder/recorder.hpp>

#define PARAM_NAME_FREQ "freq"
#define PARAM_NAME_PATH_DATA "path_data"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "recorder_event");
  ros::NodeHandle nh_private("~");

  int param_freq = 10;
  nh_private.getParam(PARAM_NAME_FREQ,param_freq);

  std::string param_path_data = get_default_path_data();
  nh_private.getParam(PARAM_NAME_PATH_DATA,param_path_data);  

  ros::NodeHandle nh;
  RecorderAuto recorder_auto(nh,"scan",param_path_data,param_freq);

  ros::spin();

  return 0;
}