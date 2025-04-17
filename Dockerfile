# Use the ev3dev cross-compilation environment as the base image
FROM ev3dev/debian-stretch-cross

# Set the working directory in the container
WORKDIR /src



# Set default entry point for compilation
ENTRYPOINT ["arm-linux-gnueabi-g++"]

# Define the command to compile the source code
#CMD ["arm-linux-gnueabi-g++", "-std=c++17", "src/main.cpp", "src/position.cpp", "src/motor.cpp", "-o", "main", "-lstdc++fs", "-pthread"]

