Name:           ros-kinetic-rtabmap-ros
Version:        0.11.4
Release:        0%{?dist}
Summary:        ROS rtabmap_ros package

Group:          Development/Libraries
License:        BSD
Source0:        %{name}-%{version}.tar.gz

Requires:       ros-kinetic-class-loader
Requires:       ros-kinetic-cv-bridge
Requires:       ros-kinetic-dynamic-reconfigure
Requires:       ros-kinetic-eigen-conversions
Requires:       ros-kinetic-geometry-msgs
Requires:       ros-kinetic-image-transport
Requires:       ros-kinetic-image-transport-plugins
Requires:       ros-kinetic-laser-geometry
Requires:       ros-kinetic-message-filters
Requires:       ros-kinetic-move-base-msgs
Requires:       ros-kinetic-nav-msgs
Requires:       ros-kinetic-nodelet
Requires:       ros-kinetic-octomap
Requires:       ros-kinetic-octomap-ros
Requires:       ros-kinetic-pcl-conversions
Requires:       ros-kinetic-pcl-ros
Requires:       ros-kinetic-roscpp
Requires:       ros-kinetic-rospy
Requires:       ros-kinetic-rtabmap
Requires:       ros-kinetic-rviz
Requires:       ros-kinetic-sensor-msgs
Requires:       ros-kinetic-std-msgs
Requires:       ros-kinetic-std-srvs
Requires:       ros-kinetic-stereo-msgs
Requires:       ros-kinetic-tf
Requires:       ros-kinetic-tf-conversions
Requires:       ros-kinetic-tf2-ros
Requires:       ros-kinetic-visualization-msgs
BuildRequires:  pcl-devel
BuildRequires:  ros-kinetic-catkin
BuildRequires:  ros-kinetic-class-loader
BuildRequires:  ros-kinetic-cv-bridge
BuildRequires:  ros-kinetic-dynamic-reconfigure
BuildRequires:  ros-kinetic-eigen-conversions
BuildRequires:  ros-kinetic-genmsg
BuildRequires:  ros-kinetic-geometry-msgs
BuildRequires:  ros-kinetic-image-transport
BuildRequires:  ros-kinetic-laser-geometry
BuildRequires:  ros-kinetic-message-filters
BuildRequires:  ros-kinetic-move-base-msgs
BuildRequires:  ros-kinetic-nav-msgs
BuildRequires:  ros-kinetic-nodelet
BuildRequires:  ros-kinetic-octomap
BuildRequires:  ros-kinetic-octomap-ros
BuildRequires:  ros-kinetic-pcl-conversions
BuildRequires:  ros-kinetic-pcl-ros
BuildRequires:  ros-kinetic-roscpp
BuildRequires:  ros-kinetic-rospy
BuildRequires:  ros-kinetic-rtabmap
BuildRequires:  ros-kinetic-rviz
BuildRequires:  ros-kinetic-sensor-msgs
BuildRequires:  ros-kinetic-std-msgs
BuildRequires:  ros-kinetic-std-srvs
BuildRequires:  ros-kinetic-stereo-msgs
BuildRequires:  ros-kinetic-tf
BuildRequires:  ros-kinetic-tf-conversions
BuildRequires:  ros-kinetic-tf2-ros
BuildRequires:  ros-kinetic-visualization-msgs

%description
RTAB-Map's ros-pkg. RTAB-Map is a RGB-D SLAM approach with real-time
constraints.

%prep
%setup -q

%build
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree that was dropped by catkin, and source it.  It will
# set things like CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/kinetic/setup.sh" ]; then . "/opt/ros/kinetic/setup.sh"; fi
mkdir -p obj-%{_target_platform} && cd obj-%{_target_platform}
%cmake .. \
        -UINCLUDE_INSTALL_DIR \
        -ULIB_INSTALL_DIR \
        -USYSCONF_INSTALL_DIR \
        -USHARE_INSTALL_PREFIX \
        -ULIB_SUFFIX \
        -DCMAKE_INSTALL_LIBDIR="lib" \
        -DCMAKE_INSTALL_PREFIX="/opt/ros/kinetic" \
        -DCMAKE_PREFIX_PATH="/opt/ros/kinetic" \
        -DSETUPTOOLS_DEB_LAYOUT=OFF \
        -DCATKIN_BUILD_BINARY_PACKAGE="1" \

make %{?_smp_mflags}

%install
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree that was dropped by catkin, and source it.  It will
# set things like CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/kinetic/setup.sh" ]; then . "/opt/ros/kinetic/setup.sh"; fi
cd obj-%{_target_platform}
make %{?_smp_mflags} install DESTDIR=%{buildroot}

%files
/opt/ros/kinetic

%changelog
* Fri May 06 2016 Mathieu Labbe <matlabbe@gmail.com> - 0.11.4-0
- Autogenerated by Bloom

