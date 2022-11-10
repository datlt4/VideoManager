#include "VideoCapture.h"

// VideoCapture
AN::VideoCapture::VideoCapture(std::string &address_in)
{
    this->auto_select_cap_source(address_in, 1280, 720, 30);
}

void AN::VideoCapture::auto_select_cap_source(std::string &address_in, int width, int height, int fps)
{
    cap_source.address_in = address_in;
    // RTSP
    if (boost::to_lower_copy(cap_source.address_in.substr(0, 7)) == "rtsp://")
    {
        // Using GStreamer for RTSP stability over both UDP/TCP as well as good performance and flexibility.
        // We setup our own GStreamer's pipeline for flexibility. To clear latency, etc
        cap_source.video_capture_backend = cv::CAP_GSTREAMER;
        cap_source.video_source = "rtspsrc location=" + cap_source.address_in + " latency=0 ! rtph264depay ! queue ! h264parse ! nvv4l2decoder ! nvvidconv ! video/x-raw,format=BGRx ! queue ! videoconvert ! queue ! video/x-raw, format=BGR ! appsink";
        cap_source.cap_type = VideoCaptureType::RTSP_STREAM;
    }
    // Youtube
    // else if (cap_source.address_in.find("youtube.com") != std::string::npos)
    //{
    //    cap_source.video_capture_backend = cv::CAP_GSTREAMER;
    //    // Youtube
    //    std::string http_youtube = execv(("youtube-dl -f \"bestvideo\" --get-url \"" + cap_source.address_in + '\"').c_str());
    //    if (http_youtube.substr(0, 8) != "https://")
    //    {
    //        http_youtube = execv(("youtube-dl -f \"best\" --get-url \"" + cap_source.address_in + '\"').c_str());
    //    }
    //    rtrim(http_youtube);
    //    cap_source.video_source = "uridecodebin uri=" + http_youtube + "! nvvidconv ! video/x-raw,format=BGRx ! videoconvert ! video/x-raw,format=BGR ! appsink drop=1";
    //    cap_source.cap_type = VideoCaptureType::YOUTUBE;
    //}
    // Camera USB
    else if (cap_source.address_in.substr(0, 4) == "v4l2")
    {
        cap_source.frame_rate = 30;
        cap_source.video_capture_backend = cv::CAP_GSTREAMER;
        cap_source.video_source = "v4l2src device=" + cap_source.address_in.substr(7, cap_source.address_in.length()) + " ! video/x-raw";
        cap_source.video_source += ",format=YUY2,framerate=" + std::to_string(cap_source.frame_rate) + "/1";
        cap_source.video_source += " ! nvvidconv ! video/x-raw(memory:NVMM),format=BGRx ! nvvidconv ! video/x-raw,format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink";
        cap_source.cap_type = VideoCaptureType::CAM_USB;
    }
    // Local file
    else
    {
        boost::replace_all(cap_source.address_in, "\\", "/");
        cap_source.video_capture_backend = cv::CAP_GSTREAMER;
        cap_source.video_source = "filesrc location=\"" + cap_source.address_in + "\" ! qtdemux ! queue ! h264parse ! omxh264dec ! nvvidconv ! video/x-raw,format=BGRx ! queue ! videoconvert ! queue ! video/x-raw, format=BGR ! appsink";
        cap_source.cap_type = VideoCaptureType::LOCAL_VIDEO;
    }

    if (height != -1)
    {
        cap_source.frame_height = height;
        cap_source.custom_frame_height = true;
    }
    if (width != -1)
    {
        cap_source.frame_width = width;
        cap_source.custom_frame_width = true;
    }
    if (fps != -1)
    {
        cap_source.frame_rate = fps;
        cap_source.custom_frame_rate = true;
    }
}

void AN::VideoCapture::create_connection()
{
    if (video_cap.isOpened())
        video_cap.release();
    std::cout << cap_source << std::endl;
    video_cap = cv::VideoCapture(cap_source.video_source, cap_source.video_capture_backend);
}

bool AN::VideoCapture::is_opened()
{
    return video_cap.isOpened();
}

bool AN::VideoCapture::read(cv::Mat &image)
{
    return video_cap.read(image);
}

int AN::VideoCapture::get_width_frame()
{
    if (cap_source.custom_frame_width)
        return cap_source.frame_width;
    else
        return 0;
}

int AN::VideoCapture::get_height_frame()
{
    if (cap_source.custom_frame_height)
        return cap_source.frame_height;
    else
        return 0;
}
