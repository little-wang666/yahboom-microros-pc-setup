cmake_minimum_required(VERSION 3.16)

# ESP-IDF 工程入口文件。
# 真正构建时需要先安装 ESP-IDF，并在 components/ 中放入 micro_ros_espidf_component。
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(yahboom_esp32_micro_ros_car)
