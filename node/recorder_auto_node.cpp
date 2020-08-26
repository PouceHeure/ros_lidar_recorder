#include <ros_lidar_recorder/recorder.hpp>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "recorder_event");
  ros::NodeHandle nh;

  std::string path_dir_data = get_default_path_data();
  RecorderAuto recorder_auto(nh,"scan",path_dir_data,10);

  ros::spin();

  return 0;
}