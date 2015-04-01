Installation Xubuntu
====================


###Install Xubuntu

<http://xubuntu.org/getxubuntu/>

Install Latest LTS release: 14.04, Trusty Tahr

```shell
sudo apt-get update
sudo apt-get dist-upgrade -y
sudo apt-get install git openssh-server -y
```

###Install Point Grey FlyCapture2 Library

Links and instructions for downloading and installing the latest
FlyCapture 2.x library from Point Grey for Linux can be found here:

<http://www.ptgrey.com/support/downloads>

Download Linux (64-bit, 32-bit, or ARM, whichever is appropriate).
Requires registration.

```shell
cd ~/Downloads
tar -zxvf flycapture*
cd flycapture*
cat README
# follow the instructions that the script takes you through
sudo reboot
```

```shell
# plug in Flea3 camera into USB3 port
flycap
```

###Download, Configure, and Build OpenCV

```shell
sudo apt-get install build-essential checkinstall libgtk2.0-dev libjpeg-dev libtiff4-dev libjasper-dev libopenexr-dev cmake python-dev python-numpy python-tk libtbb-dev libeigen3-dev yasm libfaac-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev libx264-dev libqt4-dev libqt4-opengl-dev sphinx-common texlive-latex-extra libv4l-dev libdc1394-22-dev libavcodec-dev libavformat-dev libswscale-dev default-jdk ant libvtk5-qt4-dev

mkdir ~/git
cd ~/git
git clone https://github.com/Itseez/opencv.git
cd opencv
git checkout -b 2.4 origin/2.4
mkdir -p ~/builds/opencv/opencv-2.4
cd ~/builds/opencv/opencv-2.4
cmake -D WITH_TBB=ON -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=ON -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D BUILD_EXAMPLES=ON -D WITH_QT=ON -D WITH_OPENGL=ON -D WITH_VTK=ON  ~/git/opencv
make -j2
sudo checkinstall
```

To uninstall:

```shell
sudo dpkg -r opencv
```

###Install ROS

<http://wiki.ros.org/ROS/Installation>

```shell
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu trusty main" > /etc/apt/sources.list.d/ros-latest.list'
wget https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -O - | sudo apt-key add -
sudo apt-get update
sudo apt-get install ros-indigo-desktop-full
sudo rosdep init
rosdep update
echo "source /opt/ros/indigo/setup.bash" >> ~/.bashrc
source ~/.bashrc
sudo apt-get install python-rosinstall
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_init_workspace
cd ..
catkin_make
source devel/setup.bash
```

###Install ROS Point Grey Camera Driver

```shell
sudo mkdir /usr/include/flycapture
sudo ln -s /usr/include/FlyCapture2.h /usr/include/flycapture/FlyCapture2.h
sudo apt-get install ros-indigo-roslint
sudo apt-get install ros-indigo-camera-info-manager
sudo apt-get install ros-indigo-driver-common
sudo apt-get install ros-indigo-image-proc
cd ~/catkin_ws/src/
git clone https://github.com/ros-drivers/pointgrey_camera_driver.git
cd ..
catkin_make
echo "source ~/catkin_ws/devel/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

####Test ROS Point Grey Camera Driver

Connect Point Grey USB3 camera to USB3 port.

```shell
roslaunch pointgrey_camera_driver camera.launch
```

Open another terminal and run:

```shell
rosrun image_view image_view image:=/camera/image_raw
```

###Install ROS UVC Camera Driver

```shell
sudo apt-get install ros-indigo-uvc-camera
```

####Test ROS UVC Camera Driver

Connect USB camera to USB port.

```shell
rosrun uvc_camera uvc_camera_node
```

Open another terminal and run:

```shell
rosrun image_view image_view image:=/image_raw
```

