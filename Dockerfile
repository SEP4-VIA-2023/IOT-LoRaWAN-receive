FROM alpine:latest
# Set the working directory to /app
WORKDIR /app
# Copy the current directory contents into the container at /app
COPY . /app
# update apk
RUN apk update
# install python3 for platformio
RUN apk add --no-cache python3 py3-pip py3-virtualenv
# install platformio
RUN python3 get-platformio.py
# allowing pio to be ran from anywhere
RUN export PATH=$PATH:$HOME/.local/bin
RUN mkdir -p /usr/local/bin
RUN ln -s ~/.platformio/penv/bin/platformio /usr/local/bin/platformio
RUN ln -s ~/.platformio/penv/bin/pio /usr/local/bin/pio
RUN ln -s ~/.platformio/penv/bin/piodebuggdb /usr/local/bin/piodebuggdb
# Dockerfile change test