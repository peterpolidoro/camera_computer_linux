#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <signal.h>


static void help()
{
  std::cout
    << "------------------------------------------------------------------------------" << std::endl
    << "This program shows how to write image files."                                   << std::endl
    << "Usage:"                                                                         << std::endl
    << "./opencv_datetime_imwrite output_path_base"                                     << std::endl
    << "------------------------------------------------------------------------------" << std::endl
    << std::endl;
}

boost::filesystem::path createDirectory(const boost::filesystem::path &path)
{
  if (!boost::filesystem::exists(path))
  {
    if(boost::filesystem::create_directory(path)) {
      std::cout << "Created directory: " << path << std::endl;
    }
    else
    {
      std::cout << "Unable to create directory: " << path << std::endl;
    }
  }
  else if (boost::filesystem::is_directory(path))
  {
    std::cout << "Directory exists: " << path << std::endl;
  }
  else
  {
    std::cout << "Error! " << path << " exists, but is not a directory!" << std::endl;
  }
  return path;
}

struct sigaction act;
volatile bool killed = false;

void sighandler(int signum, siginfo_t *info, void *ptr)
{
  std::cout << "Received signal " << signum << std::endl;
  std::cout << "Signal originates from process " << (unsigned long)info->si_pid << std::endl;
  killed = true;
}

int main(int argc, char *argv[])
{
  help();

  if (argc != 2)
  {
    std::cout << "Not enough parameters" << std::endl;
    return -1;
  }

  // Setup signal handler
  std::cout << "I am " << (unsigned long)getpid() << std::endl;
  memset(&act, 0, sizeof(act));

  act.sa_sigaction = sighandler;
  act.sa_flags = SA_SIGINFO;

  sigaction(SIGTERM, &act, NULL);

  // Create base directory
  boost::filesystem::path output_path_base(argv[1]);
  createDirectory(output_path_base);

  // Create directory for saving images
  boost::posix_time::ptime date_time = boost::posix_time::second_clock::local_time();
  std::string date_time_string = boost::posix_time::to_iso_string(date_time);
  boost::filesystem::path output_dir(date_time_string);
  boost::filesystem::path output_path = output_path_base / output_dir;
  createDirectory(output_path);

  // Save information about run into file
  boost::filesystem::path run_info_file_path("run_info");
  boost::filesystem::path run_info_file_path_full = output_path_base / run_info_file_path;
  std::ofstream run_info_file;
  run_info_file.open(run_info_file_path_full.string().c_str());
  run_info_file << output_path << std::endl;
  run_info_file.close();

  // Setup image parameters
  std::vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(0);

  char key = 0;
  int color = 0;
  boost::posix_time::ptime start_time = boost::posix_time::second_clock::local_time();
  while ((key != 'q') && !(killed))
  {
    cv::Mat image = cv::Mat(100,100,CV_8UC3,cv::Scalar(color,color,255));
    color++;
    if (color > 255)
    {
      color = 0;
    }
    cv::imshow("image",image);
    key = cv::waitKey(1);

    // Create full image output path
    date_time = boost::posix_time::microsec_clock::local_time();
    std::string time_string = boost::posix_time::to_iso_string(date_time);
    std::ostringstream image_file_name;
    image_file_name << time_string << ".png";
    std::string image_file_name_string = image_file_name.str();
    boost::filesystem::path output_file_name_path(image_file_name_string);
    boost::filesystem::path output_path_full = output_path / output_file_name_path;

    // Write image to file
    cv::imwrite(output_path_full.string(),image,compression_params);
  }
  boost::posix_time::ptime stop_time = boost::posix_time::second_clock::local_time();
  boost::posix_time::time_duration run_duration = stop_time - start_time;
  int run_duration_seconds = run_duration.total_seconds();
  std::cout << "Run duration: " << run_duration_seconds << std::endl;

  int image_count = 0;
  boost::filesystem::directory_iterator end_iter;
  for (boost::filesystem::directory_iterator dir_itr(output_path);
       dir_itr != end_iter;
       ++dir_itr )
  {
    try
    {
      if (boost::filesystem::is_regular_file(dir_itr->status()))
      {
        ++image_count;
        // std::cout << dir_itr->path().filename() << "\n";
      }
    }
    catch (const std::exception & ex)
    {
      std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
    }
  }
  std::cout << "Image count: " << image_count << std::endl;
  std::cout << "Images per second: " << image_count/run_duration_seconds << std::endl;
  return 0;
}

// #include <stdio.h>
// #include <signal.h>
// #include <string.h>
// #include <unistd.h>

// struct sigaction act;

// void sighandler(int signum, siginfo_t *info, void *ptr)
// {
//     printf("Received signal %d\n", signum);
//     printf("Signal originates from process %lu\n",
//         (unsigned long)info->si_pid);
// }

// int main()
// {
//     printf("I am %lu\n", (unsigned long)getpid());

//     memset(&act, 0, sizeof(act));

//     act.sa_sigaction = sighandler;
//     act.sa_flags = SA_SIGINFO;

//     sigaction(SIGTERM, &act, NULL);

//     // Waiting for CTRL+C...
//     sleep(100);

//     return 0;
// }
