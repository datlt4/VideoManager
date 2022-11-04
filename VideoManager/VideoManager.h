#ifndef _VIDEO_MANAGER_H_
#define _VIDEO_MANAGER_H_

#include "VideoCapture.h"
#include "VideoWriter.h"
#include <chrono>

#define __YELLOW__ "\033[93m"
#define __MAGENTA__ "\033[95m"
#define __CYAN__ "\033[96m"
#define __BLUE__ "\033[94m"
#define __GREEN__ "\033[92m"
#define __RED__ "\033[91m"
#define __BOLD__ "\033[1m"
#define __UNDERLINE__ "\033[4m"
#define __ENDC__ "\033[0m"

#define NO_SIGNALE_IMAGE_PATH "../no-signal.png"
#define RETRY_CONNECT 5

namespace AN
{
    class VideoManager
    {
    private:
        std::vector<VideoCapture> vector_capture;
        int current_vector_capture_index = -1;
        int preview_vector_capture_index = -1;
        std::vector<VideoWriter> vector_writer;
        int current_vector_writer_index = -1;
        int preview_vector_writer_index = -1;
        cv::Mat first_frame;
        cv::Mat no_signal_mat;
        std::chrono::_V2::system_clock::time_point time_since_last_connect;
        
    public:
        VideoManager(std::vector<std::string> &input_sources, std::vector<std::string> &output_sources);
        bool is_opened();
        bool read(cv::Mat &image);
        void start_video_capture(int index);
        void start_video_writer();

        ~VideoManager()
        {
            std::cout << "[ " << __FUNCTION__ << " ]\n";
        }
    };
}

#endif // _VIDEO_MANAGER_H_