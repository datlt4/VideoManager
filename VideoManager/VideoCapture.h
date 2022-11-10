#ifndef _VIDEO_CAPTURE_H_
#define _VIDEO_CAPTURE_H_

#include <opencv2/opencv.hpp>
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include "utils.h"

#include <iostream>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <opencv2/opencv.hpp>

namespace AN
{
    enum class VideoCaptureType
    {
        LOCAL_VIDEO,
        CAM_USB,
        CAM_CSI,
        RTSP_STREAM,
        WEB_RTC_STREAM,
        YOUTUBE
    };

    typedef struct _CaptureSourceStruct
    {
        std::string address_in;
        std::string video_source;
        int video_capture_backend;
        VideoCaptureType cap_type;
        int frame_rate;
        bool custom_frame_rate = false;
        int frame_width;
        bool custom_frame_width = false;
        int frame_height;
        bool custom_frame_height = false;

        _CaptureSourceStruct() {}

        friend std::ostream &operator<<(std::ostream &os, const _CaptureSourceStruct &spec)
        {
            os << "Capure Source:\n\tAddress In: " << spec.address_in << "\n\tSource: " << spec.video_source << "\n\tBackend: " << spec.video_capture_backend << "\n\tType: ";
            switch (spec.cap_type)
            {
            case VideoCaptureType::LOCAL_VIDEO:
                os << "LOCAL_VIDEO";
                break;
            case VideoCaptureType::CAM_USB:
                os << "CAM_USB";
                break;
            case VideoCaptureType::CAM_CSI:
                os << "CAM_CSI";
                break;
            case VideoCaptureType::RTSP_STREAM:
                os << "RTSP_STREAM";
                break;
            case VideoCaptureType::WEB_RTC_STREAM:
                os << "WEB_RTC_STREAM";
                break;
            case VideoCaptureType::YOUTUBE:
                os << "YOUTUBE";
                break;
            default:
                os << "None";
                break;
            }
            os << "\n\tcustom FPS: " << (spec.custom_frame_rate ? std::to_string(spec.frame_rate) : "Null");
            os << "\n\tcustom Width: " << (spec.custom_frame_width ? std::to_string(spec.frame_width) : "Null");
            os << "\n\tcustom Height: " << (spec.custom_frame_height ? std::to_string(spec.frame_height) : "Null");

            return os;
        }
    } CapSrcStruct;

    class VideoCapture
    {
    public:
        VideoCapture(std::string &input_souce);
        void create_connection();
        void auto_select_cap_source(std::string &address_in, int width=-1, int height=-1, int fps=-1);
        bool is_opened();
        bool read(cv::Mat &image);
        int get_width_frame();
        int get_height_frame();

    private:
        // Default
        CapSrcStruct cap_source;
        cv::VideoCapture video_cap;
    };
}

#endif // _VIDEO_CAPTURE_H_
