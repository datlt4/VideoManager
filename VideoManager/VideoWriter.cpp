#include "VideoWriter.h"

namespace AN
{
    size_t rtsp_data_size;
    queue_data<cv::Mat, 3> rtsp_manager;

    void rtsp_need_data(GstElement *appsrc, guint unused, RTSPContext *ctx)
    {
        GstBuffer *buffer = gst_buffer_new_allocate(NULL, AN::rtsp_data_size, NULL);
        gst_buffer_fill(buffer, 0, AN::rtsp_manager.receive().data, AN::rtsp_data_size);

        // TODO: Improve FPS/timestamp/duration settings
        GST_BUFFER_PTS(buffer) = ctx->timestamp;
        GST_BUFFER_DURATION(buffer) = gst_util_uint64_scale_int(1, GST_SECOND, 30);
        ctx->timestamp += GST_BUFFER_DURATION(buffer);

        GstFlowReturn ret; // Not used, but prevent annoying warnings
        g_signal_emit_by_name(appsrc, "push-buffer", buffer, &ret);
        gst_buffer_unref(buffer);
    }

    void rtsp_media_configure(GstRTSPMediaFactory *factory, GstRTSPMedia *media, gpointer user_data)
    {
        GstElement *element = gst_rtsp_media_get_element(media);
        GstElement *appsrc = gst_bin_get_by_name_recurse_up(GST_BIN(element), "rtsp_src");

        g_object_set_data_full(G_OBJECT(media), "my-extra-data", nullptr, (GDestroyNotify)g_free);
        g_signal_connect(appsrc, "need-data", (GCallback)rtsp_need_data, g_new0(RTSPContext, 1));

        gst_object_unref(appsrc);
        gst_object_unref(element);
    }
}

void AN::RtspServer::start_rtsp_server()
{
    gst_init(nullptr, nullptr);
    rtsp_server = gst_rtsp_server_new();
    /* make a mainloop for the default context */
    rtsp_loop = g_main_loop_new(NULL, FALSE);
    gst_rtsp_server_set_address(rtsp_server, "127.0.0.1");
    gst_rtsp_server_set_service(rtsp_server, "8554");
    // TODO: Improve factory/pipeline
    std::string launch = "( appsrc name=rtsp_src format=GST_FORMAT_TIME max-latency=0 caps=video/x-raw,format=BGR,width=1920,height=1080,framerate=30/1 ! videoconvert ! videorate ! video/x-raw,format=I420 ! x264enc speed-preset=ultrafast tune=zerolatency ! rtph264pay name=pay0 )";
    gst_rtsp_media_factory_set_launch(factory, launch.c_str());
    factory = gst_rtsp_media_factory_new();
    // 	GST_RTSP_LOWER_TRANS_UDP_MCAST);
    g_signal_connect(factory, "media-configure", (GCallback)AN::rtsp_media_configure, nullptr);

    mounts = gst_rtsp_server_get_mount_points(rtsp_server);
    gst_rtsp_mount_points_add_factory(mounts, "/anoi", factory);
    g_object_unref(mounts);

    rtsp_server_sid = gst_rtsp_server_attach(rtsp_server, nullptr);
    std::cout << "RTSP stream ready at rtsp://127.0.0.1:8554/anoi\n";

    g_main_loop_run(rtsp_loop);
}

void AN::RtspServer::stop_rtsp_server()
{
    g_source_remove(rtsp_server_sid);
    g_main_loop_quit(rtsp_loop);
}

void AN::RtspServer::push_frame(cv::Mat &frame)
{
    AN::rtsp_data_size = frame.total() * frame.elemSize();
    AN::rtsp_manager.push(frame);
}
// VideoWriter
AN::VideoWriter::VideoWriter(std::string &output_souce)
{
    std::cout << "[ " << __FUNCTION__ << " ]: " << output_souce << std::endl;
}
