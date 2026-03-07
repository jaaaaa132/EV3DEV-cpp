sudo docker run --rm -v $(pwd):/src ev3dev-cpp-compiler -mcpu=arm926ej-s -march=armv5te -marm -O3 -Wl,--gc-sections -fdata-sections -ffunction-sections -Wall -Wextra -Wshadow -Werror=return-type -flto src/mains/main1.cpp src/motor.cpp src/position.cpp src/sensor.cpp src/diferential_drive.cpp src/robot.cpp -o main1 -std=c++14 -lm -pthread -lstdc++fs
echo "main1 compiled"
sudo docker run --rm -v $(pwd):/src ev3dev-cpp-compiler -mcpu=arm926ej-s -march=armv5te -marm -O3 -Wl,--gc-sections -fdata-sections -ffunction-sections -Wall -Wextra -Wshadow -Werror=return-type -flto src/mains/main2.cpp src/motor.cpp src/position.cpp src/sensor.cpp src/diferential_drive.cpp src/robot.cpp -o main2 -std=c++14 -lm -pthread -lstdc++fs
echo "main2 compiled"
sudo docker run --rm -v $(pwd):/src ev3dev-cpp-compiler -mcpu=arm926ej-s -march=armv5te -marm -O3 -Wl,--gc-sections -fdata-sections -ffunction-sections -Wall -Wextra -Wshadow -Werror=return-type -flto src/mains/main3.cpp src/motor.cpp src/position.cpp src/sensor.cpp src/diferential_drive.cpp src/robot.cpp -o main3 -std=c++14 -lm -pthread -lstdc++fs
echo "main3 compiled"
sudo docker run --rm -v $(pwd):/src ev3dev-cpp-compiler -mcpu=arm926ej-s -march=armv5te -marm -O3 -Wl,--gc-sections -fdata-sections -ffunction-sections -Wall -Wextra -Wshadow -Werror=return-type -flto src/mains/main4.cpp src/motor.cpp src/position.cpp src/sensor.cpp src/diferential_drive.cpp src/robot.cpp -o main4 -std=c++14 -lm -pthread -lstdc++fs
echo "main4 compiled"
sudo docker run --rm -v $(pwd):/src ev3dev-cpp-compiler -mcpu=arm926ej-s -march=armv5te -marm -O3 -Wl,--gc-sections -fdata-sections -ffunction-sections -Wall -Wextra -Wshadow -Werror=return-type -flto src/mains/main5.cpp src/motor.cpp src/position.cpp src/sensor.cpp src/diferential_drive.cpp src/robot.cpp -o main5 -std=c++14 -lm -pthread -lstdc++fs
echo "main5 compiled"

#scp main robot@10.42.0.3:/home/robot/libc++/
