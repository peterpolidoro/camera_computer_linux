#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <signal.h>


// Globals
struct sigaction g_action_sigterm;
struct sigaction g_action_sigusr1;
struct sigaction g_action_sigusr2;
bool g_setup = false;
volatile bool g_killed = false;
volatile bool g_saving = false;
boost::filesystem::path g_output_path_base;
boost::filesystem::path g_output_path;
boost::posix_time::ptime g_start_time;


static void help()
{
  std::cout
    << "------------------------------------------------------------------------------" << std::endl
    << "This program shows how to write image files."                                   << std::endl
    << "Start Program:"                                                                 << std::endl
    << "./opencv_datetime_imwrite output_path_base"                                     << std::endl
    << std::endl
    << "Start Saving Images:"                                                           << std::endl
    << "PID=`pidof opencv_datetime_imwrite`"                                            << std::endl
    << "kill -s USR1 $PID"                                                              << std::endl
    << std::endl
    << "Stop Saving Images:"                                                            << std::endl
    << "kill -s USR2 $PID"                                                              << std::endl
    << std::endl
    << "Stop Program:"                                                                  << std::endl
    << "kill $PID"                                                                      << std::endl
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

void sighandler_sigterm(int signum)
{
  std::cout << "Killing process." << std::endl;
  g_killed = true;
}

void sighandler_sigusr1(int signum)
{
  if (g_setup)
  {
    std::cout << "Saving images... " << std::endl;

    // Create directory for saving images
    boost::posix_time::ptime date_time = boost::posix_time::second_clock::local_time();
    std::string date_time_string = boost::posix_time::to_iso_string(date_time);
    boost::filesystem::path output_dir(date_time_string);
    boost::filesystem::path output_path = g_output_path_base / output_dir;
    createDirectory(output_path);
    g_output_path = output_path;

    // Save information about run into file
    boost::filesystem::path run_info_file_path("run_info");
    boost::filesystem::path run_info_file_path_full = g_output_path_base / run_info_file_path;
    std::ofstream run_info_file;
    run_info_file.open(run_info_file_path_full.string().c_str());
    run_info_file << output_path << std::endl;
    run_info_file.close();

    g_start_time = boost::posix_time::second_clock::local_time();
    g_saving = true;
  }
}

void sighandler_sigusr2(int signum)
{
  g_saving = false;

  boost::posix_time::ptime stop_time = boost::posix_time::second_clock::local_time();
  boost::posix_time::time_duration run_duration = stop_time - g_start_time;
  int run_duration_seconds = run_duration.total_seconds();
  std::cout << "Run duration: " << run_duration_seconds << "s" << std::endl;

  int image_count = 0;
  boost::filesystem::directory_iterator end_iter;
  for (boost::filesystem::directory_iterator dir_itr(g_output_path);
       dir_itr != end_iter;
       ++dir_itr )
  {
    try
    {
      if (boost::filesystem::is_regular_file(dir_itr->status()))
      {
        ++image_count;
      }
    }
    catch (const std::exception & ex)
    {
      std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
    }
  }
  std::cout << "Image count: " << image_count << std::endl;
  if (run_duration_seconds > 0)
  {
    std::cout << "Images per second: " << image_count/run_duration_seconds << std::endl;
  }
}

int main(int argc, char *argv[])
{
  g_setup = false;

  help();

  if (argc != 2)
  {
    std::cout << "Not enough parameters" << std::endl;
    return -1;
  }

  // Setup signal handler SIGTERM
  memset(&g_action_sigterm, 0, sizeof(g_action_sigterm));
  g_action_sigterm.sa_handler = sighandler_sigterm;
  sigaction(SIGTERM, &g_action_sigterm, NULL);

  // Setup signal handler SIGUSR1
  memset(&g_action_sigusr1, 0, sizeof(g_action_sigusr1));
  g_action_sigusr1.sa_handler = sighandler_sigusr1;
  sigaction(SIGUSR1, &g_action_sigusr1, NULL);

  // Setup signal handler SIGUSR2
  memset(&g_action_sigusr2, 0, sizeof(g_action_sigusr2));
  g_action_sigusr2.sa_handler = sighandler_sigusr2;
  sigaction(SIGUSR2, &g_action_sigusr2, NULL);

  // Create base directory
  boost::filesystem::path output_path_base(argv[1]);
  createDirectory(output_path_base);
  g_output_path_base = output_path_base;

  // Setup image parameters
  std::vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(0);

  g_setup = true;

  char key = 0;
  int color = 0;
  while ((key != 'q') && !(g_killed))
  {
    if (g_saving)
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
      boost::posix_time::ptime date_time = boost::posix_time::microsec_clock::local_time();
      std::string time_string = boost::posix_time::to_iso_string(date_time);
      std::ostringstream image_file_name;
      image_file_name << time_string << ".png";
      std::string image_file_name_string = image_file_name.str();
      boost::filesystem::path output_file_name_path(image_file_name_string);
      boost::filesystem::path output_path_full = g_output_path / output_file_name_path;

      // Write image to file
      cv::imwrite(output_path_full.string(),image,compression_params);
    }
  }
  return 0;
}
