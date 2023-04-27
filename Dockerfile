# Use Ubuntu as the base image
FROM ubuntu:latest

# Install build-essential for C development
RUN apt-get update && apt-get install -y build-essential

# Copy your C code
COPY src /iot/src

# Set the working directory
WORKDIR /iot/src

# Compile your code
RUN gcc -o myapp main.c

# Define the entry point
CMD ["/app/src/myapp"]
