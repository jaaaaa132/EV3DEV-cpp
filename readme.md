# Other languages
[Slovak/slovenčina](readme_SK.md)

# Setup
1. Setup microSD card: https://www.ev3dev.org/docs/getting-started/
2. Download and move main files onto a SD card to  partition `ROOTFS` and `/home/robot/libc++/` directory
3. Install `putty` for Windows and `shh`, `sshpass` for linux
4. Download costume programing language(no installation required)
5. You are ready to use everything and have fun, PS: do not forget to send robot constants

# Defaults
### - program_1.prgm is run from main1 (program_2.prgm from main2)
### - robot_1.conf is used in main1 to set class Robot (robot_2.conf is used in main2)
## motor connections
### - left arm -> A
### - left wheel -> B
### - right wheel -> C
### - right arm -> D
## sensor connections
### - gyro  -> 1
### - color -> 2
# Positioning
### - angle zero is at Y axis
### - angle is in rad
### - angle goes counterclockwise (left is positive, right negative)
# Costume programing language
### - for send file feature on linux `sshpass` and `ssh` has to be installed, for Windows `putty` has to be installed
### - supports English and Slovak
### - for more information read [príručka M++](prirucka_m++.txt), for now it is only in Slovak
# Helpful links
### https://www.ev3dev.org/
### https://github.com/ev3dev/ev3dev/issues/1032
### https://docs.ev3dev.org/en/ev3dev-stretch/
