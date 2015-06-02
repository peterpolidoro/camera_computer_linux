#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

int main()
{
  boost::posix_time::ptime date_time = boost::posix_time::second_clock::local_time();
  std::string date_time_string = boost::posix_time::to_iso_string(date_time);
  boost::filesystem::path dir(date_time_string);
  if(boost::filesystem::create_directory(dir)) {
    std::cout << "Created directory: " << date_time_string << "\n";
  }

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
    image_file_name << "./" << date_time_string << "/" << time_string << ".png";
    std::string image_file_name_string = image_file_name.str();

    cv::imwrite(image_file_name_string,image,compression_params);
  }
  return 0;
}
