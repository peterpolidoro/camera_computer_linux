opencv_datetime_imwrite
=======================

##Install OpenCV

[Install Instructions](../../INSTALL_XUBUNTU.md)

##Build This Example

```shell
mkdir -p ~/builds/opencv_datetime_imwrite/opencv_datetime_imwrite-1.0
cd ~/builds/opencv_datetime_imwrite/opencv_datetime_imwrite-1.0
cmake ~/git/camera_computer_linux/examples/opencv_datetime_imwrite/
make
```

##Start Program

```shell
cd ~/builds/opencv_datetime_imwrite/opencv_datetime_imwrite-1.0
./opencv_datetime_imwrite output_path_base
```

##Start Saving Images

```shell
PID=`pidof opencv_datetime_imwrite`
kill -s USR1 $PID
```

##Stop Saving Images

```shell
PID=`pidof opencv_datetime_imwrite`
kill -s USR2 $PID
```

##Stopping Program

In originating terminal type:

```shell
q
```

Or open another terminal and type:

```shell
PID=`pidof opencv_datetime_imwrite`
kill $PID
```
