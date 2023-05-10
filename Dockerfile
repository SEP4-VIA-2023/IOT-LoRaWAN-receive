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
RUN pwd
RUN ls
#RUN python3 get-platformio.py
#reload so everything works
#RUN udevadm control --reload-rules && udevadm trigger
RUN export PATH=$PATH:$HOME/.local/bin 