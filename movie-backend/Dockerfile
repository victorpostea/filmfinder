# Use an official C++ image with GCC and CMake
FROM gcc:latest

# Install dependencies
RUN apt update && apt install -y cmake libpqxx-dev libasio-dev wget unzip

# Set working directory inside the container
WORKDIR /app

# Copy source files
COPY . .

# Create build directory
RUN mkdir -p build && cd build && cmake .. && make

# Expose the port that Crow runs on
EXPOSE 8080

# Run the application
CMD ["./build/movie-backend"]