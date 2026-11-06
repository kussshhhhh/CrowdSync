FROM ubuntu:22.04
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    make \
    nlohmann-json3-dev \
    wget

WORKDIR /app
COPY . .

# Download cpp-httplib header so CMake can find it
RUN wget https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h -O /usr/include/httplib.h

RUN mkdir build && cd build && cmake .. && make
EXPOSE 8080
CMD ["./build/cpp_engine"]