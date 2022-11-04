#include "VideoCapture.h"

// VideoCapture
AN::VideoCapture::VideoCapture(std::string &address_in)
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
    // // Youtube
    // else if (cap_source.address_in.find("youtube.com") != std::string::npos)
    // {
    // 	video_capture_backend = cv::CAP_GSTREAMER;
    // 	// Youtube
    // 	std::string http_youtube = exec(("youtube-dl -f \"bestvideo\" --get-url \"" + cap_source.address_in + '\"').c_str());
    // 	if (http_youtube.substr(0, 8) != "https://")
    // 	{
    // 		http_youtube = exec(("youtube-dl -f \"best\" --get-url \"" + cap_source.address_in + '\"').c_str());
    // 	}
    // 	rtrim(http_youtube);
    // 	video_source = "uridecodebin uri=" + http_youtube + "! nvvidconv ! video/x-raw,format=BGRx ! videoconvert ! video/x-raw,format=BGR ! appsink drop=1";
    // 	video_cap = cv::VideoCapture(video_source, video_capture_backend);
    // 	cap_type = VideoCaptureType::YOUTUBE;
    // }
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
    // cap_source.custom_frame_width = ;
    cap_source.frame_height = 720;
    cap_source.custom_frame_height = true;
    cap_source.frame_width = 1280;
    cap_source.custom_frame_width = true;
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