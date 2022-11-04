#ifndef _VIDEO_WRITER_H_
#define _VIDEO_WRITER_H_

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
    class VideoWriter
    {
    public:
        VideoWriter(std::string &output_souce);

        // private:
    };
}

#endif // _VIDEO_WRITER_H_