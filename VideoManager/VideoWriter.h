#ifndef _VIDEO_WRITER_H_
#define _VIDEO_WRITER_H_

#include <opencv2/opencv.hpp>
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include "utils.h"

#include <iostream>
#include <fstream>
#include <string>
#include <functional>
//#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <opencv2/opencv.hpp>

namespace AN
{
    struct RTSPContext
    {
        GstClockTime timestamp = 0;
    };

    class RtspServer
    {
    public:
        void start_rtsp_server();
        void stop_rtsp_server();
        static void rtsp_media_configure(GstRTSPMediaFactory *factory, GstRTSPMedia *media, gpointer user_data);
        static void rtsp_need_data(GstElement *appsrc, guint unused, RTSPContext *ctx);
        void push_frame(cv::Mat &frame);
        
    private:
        // RTSP stuff
        GstRTSPServer *rtsp_server;
        GMainLoop *rtsp_loop = nullptr;
        guint rtsp_server_sid;
        GstRTSPMediaFactory *factory;
        GstRTSPMountPoints *mounts;

        bool is_ready_rtsp = false;
    };

    class VideoWriter
    {
    public:
        VideoWriter(std::string &output_souce);

        // private:
    };
}

#endif // _VIDEO_WRITER_H_