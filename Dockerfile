# choose system
FROM debian:12-slim

# download g++
RUN apt-get update && apt-get install -y g++ && rm -rf /var/lib/apt/lists/*

# working directory
WORKDIR /drone-targeting

# copy files
COPY include/ ./include/
COPY src/ ./src/
COPY run-all.sh .
COPY input/ ./input/

# compile
RUN g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o drop-calc

# make run-all.sh executable
RUN chmod +x run-all.sh

# executes when the container starts
CMD ["./run-all.sh"]