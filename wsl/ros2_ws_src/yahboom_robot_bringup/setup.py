from glob import glob
from setuptools import setup

package_name = "yahboom_robot_bringup"

setup(
    name=package_name,
    version="0.1.0",
    packages=[package_name],
    data_files=[
        ("share/ament_index/resource_index/packages", [f"resource/{package_name}"]),
        (f"share/{package_name}", ["package.xml", "README_zh.md"]),
        (f"share/{package_name}/launch", glob("launch/*.launch.py")),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="Yahboom MicroROS PC User",
    maintainer_email="user@example.com",
    description="Launch files for PC-side Yahboom micro-ROS robot bring-up.",
    license="MIT",
)
