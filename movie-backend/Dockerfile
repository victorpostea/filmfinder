FROM gcc:latest

# Install dependencies
RUN apt update && apt install -y cmake libpqxx-dev libasio-dev wget unzip git

# Install CPR (HTTP request library)
RUN git clone https://github.com/libcpr/cpr.git /cpr
WORKDIR /cpr
RUN cmake -B build -S . -DCPR_USE_SYSTEM_CURL=ON && cmake --build build && cmake --install build

# Set working directory for backend
WORKDIR /app
COPY . .

# Build movie-backend
RUN mkdir -p build && cd build && cmake .. && make

# Expose the port
EXPOSE 8080

# Run the application
CMD ["./build/movie-backend"]