# CARMA
cd ~/carma_ws/src/carma-base/docker
sudo ./build-image.sh -d
cd ../../autoware.ai/docker
sudo ./build-image.sh -d
cd ../../carma-platform/docker
sudo ./build-image.sh -d
cd ../../carma-web-ui/docker
sudo ./build-image.sh -d

# Drivers
cd ~/carma_ws/src/dataspeed_can_driver/docker
sudo ./build-image.sh -d
cd ~/carma_ws/src/dataspeed_controller_driver/docker
sudo ./build-image.sh -d
cd ~/carma_ws/src/ouster_lidar_driver/docker
sudo ./build-image.sh -d