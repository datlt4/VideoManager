# VideoManager

```bash
xhost +local:root

docker run -it --rm --net=host --runtime nvidia \
    --name emoi-dev \
    -e DISPLAY=$DISPLAY \
    -v /home/emoi/Software/VideoManager:/root/VideoManager \
    -v /tmp/.X11-unix/:/tmp/.X11-unix \
    hienanh/jetson:latest

xhost -local:root
```
