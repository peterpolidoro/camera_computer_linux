#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


static void help()
{
  std::cout
    << "------------------------------------------------------------------------------" << std::endl
    << "This program shows how to write image files."                                   << std::endl
    << "Usage:"                                                                         << std::endl
    << "./opencv_datetime_imwrite output_path"                                          << std::endl
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

int main(int argc, char *argv[])
{
  help();

  if (argc != 2)
  {
    std::cout << "Not enough parameters" << std::endl;
    return -1;
  }

  boost::filesystem::path output_path_base(argv[1]);
  createDirectory(output_path_base);

  boost::posix_time::ptime date_time = boost::posix_time::second_clock::local_time();
  std::string date_time_string = boost::posix_time::to_iso_string(date_time);
  boost::filesystem::path output_dir(date_time_string);
  boost::filesystem::path output_path = output_path_base / output_dir;
  createDirectory(output_path);

  std::vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(0);

  char key = 0;
  int color = 0;
  while(key != 'q')
  {
    cv::Mat image = cv::Mat(100,100,CV_8UC3,cv::Scalar(color,color,255));
    color++;
    if (color > 255)
    {
      color = 0;
    }
    cv::imshow("image",image);
    key = cv::waitKey(30);

    date_time = boost::posix_time::microsec_clock::local_time();
    std::string time_string = boost::posix_time::to_iso_string(date_time);
    std::ostringstream image_file_name;
    image_file_name << time_string << ".png";
    std::string image_file_name_string = image_file_name.str();
    boost::filesystem::path output_file_name_path(image_file_name_string);
    boost::filesystem::path output_path_full = output_path / output_file_name_path;

    cv::imwrite(output_path_full.string(),image,compression_params);
  }
  return 0;
}
