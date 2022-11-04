#include "application.h"

#define WINDOW_NAME "EMoi"

AN::Application::Application()
{
    this->input_sources = std::vector<std::string>{"../desert-and-car-4K.mp4", "../desert-and-car-4K-0.mp4", "rtsp://root:kikai1234@10.10.2.222/onvif-media/media.amp?profile=profile_1_h264&sessiontimeout=60&streamtype=unicast"};
    this->output_sources = std::vector<std::string>{"-123", "-456", "-789"};
    video_manager = std::unique_ptr<AN::VideoManager>(new AN::VideoManager(this->input_sources, this->output_sources));
}

void AN::Application::setup()
{
    video_manager->start_video_capture(0);
}

void AN::Application::loop()
{
    cv::namedWindow(WINDOW_NAME, cv::WINDOW_KEEPRATIO);
    cv::setWindowProperty(WINDOW_NAME, cv::WND_PROP_ASPECT_RATIO, cv::WINDOW_KEEPRATIO);
    while (1)
    {
        cv::Mat frame;
        video_manager->read(frame);
        // cv::imwrite("test.jpg", frame);
        cv::imshow("EMoi", frame);
        cv::waitKey(1);

        // std::cout << "[ COL ]: " << frame.cols << " [ ROW ]: " << frame.rows << " [ C ]: " << frame.channels() << std::endl;
    }
}