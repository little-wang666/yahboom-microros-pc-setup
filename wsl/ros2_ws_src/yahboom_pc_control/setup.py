from glob import glob
from setuptools import setup

package_name = "yahboom_pc_control"

setup(
    name=package_name,
    version="0.1.0",
    packages=[package_name],
    data_files=[
        ("share/ament_index/resource_index/packages", [f"resource/{package_name}"]),
        (f"share/{package_name}", ["package.xml"]),
        (f"share/{package_name}/launch", glob("launch/*.launch.py")),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="Yahboom MicroROS PC User",
    maintainer_email="user@example.com",
    description="Minimal PC-side ROS2 control nodes for Yahboom MicroROS car bring-up.",
    license="MIT",
    tests_require=["pytest"],
    entry_points={
        "console_scripts": [
            "cmd_vel_publisher = yahboom_pc_control.cmd_vel_publisher:main",
        ],
    },
)
