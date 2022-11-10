#include "application.h"

#define WINDOW_NAME "EMoi"

AN::Application::Application() : shutdown_program{false}
{
    this->input_sources = std::vector<std::string>{"../desert-and-car-4K.mp4", "RTSP://192.168.120.100:8554/yourname", "../desert-and-car-4K-0.mp4", "rtsp://root:kikai1234@10.10.2.222/onvif-media/media.amp?profile=profile_1_h264&sessiontimeout=60&streamtype=unicast"};
    this->output_sources = std::vector<std::string>{"-123", "-456", "-789"};
    video_manager = std::unique_ptr<AN::VideoManager>(new AN::VideoManager(this->input_sources, this->output_sources));
}

void AN::Application::setup()
{
    video_manager->start_video_capture(0);
}

void AN::Application::shutdown()
{
    std::cout << __YELLOW__ << __BOLD__ << "[ " << __FUNCTION__ << " | " << __LINE__ << " ]: " << __ENDC__ << std::endl;
    this->shutdown_program = true;
}

bool AN::Application::joinable()
{
    std::cout << __YELLOW__ << __BOLD__ << "[ " << __FUNCTION__ << " | " << __LINE__ << " ]: " << __ENDC__ << std::endl;
    return t_cap.joinable() || t_show.joinable();
}

void AN::Application::join()
{
    std::cout << __YELLOW__ << __BOLD__ << "[ " << __FUNCTION__ << " | " << __LINE__ << " ]: " << __ENDC__ << std::endl;
    if (t_cap.joinable())
        t_cap.join();
    if (t_show.joinable())
        t_show.join();
}

std::vector<std::string> AN::Application::get_info_message(unsigned int process_delay_time, int width, int height)
{
    std::vector<std::string> messages;
    std::ostringstream oss;
    oss << width << " x " << height
        << " | FPS Capture: " << this->current_fps_cap
        << " | FPS Detection: " << this->current_fps_show
        << " | Delay: " << process_delay_time << "ms";
    messages.push_back(oss.str());
    return messages;
}

void AN::Application::draw_info_message(cv::Mat image, std::vector<std::string> &messages)
{
    for (int i = 0; i < messages.size(); ++i)
        cv::putText(image, messages[i], cv::Size(10, 30 + 30 * i), DEFAULT_FONT, 1.2, COLOR_GREEN, 2);
}

void AN::Application::loop()
{
    cv::namedWindow(WINDOW_NAME, cv::WINDOW_KEEPRATIO);
    cv::setWindowProperty(WINDOW_NAME, cv::WND_PROP_ASPECT_RATIO, cv::WINDOW_KEEPRATIO);
    t_cap = std::thread([&]()
                        {
                            while (!this->shutdown_program)
                            {
                                cv::Mat frame;
                                video_manager->read(frame);
                                pipeline_data pdata{frame};
                                std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
                                float time_sec = std::chrono::duration<double>(now - time_since_last_fps_cap_tick).count();
                                if (time_sec > 1)
                                {
                                    time_since_last_fps_cap_tick = now;
                                    current_fps_cap = fps_cap_counter / time_sec;
                                    fps_cap_counter = 0;
                                }
                                else
                                {
                                    ++fps_cap_counter;
                                }
                                pdata.time_cap = time_since_last_fps_cap_tick;
                                pdata.frame_width = frame.cols;
                                pdata.frame_height = frame.rows;
                                cap2show.send(pdata);
                            } });
    t_show = std::thread([&]()
                         {
                            while (!this->shutdown_program)
                            {
                                pipeline_data pdata = cap2show.receive();
                                std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
                                float time_sec = std::chrono::duration<double>(now - time_since_last_fps_show_tick).count();
                                if (time_sec > 1)
                                {
                                    time_since_last_fps_show_tick = now;
                                    current_fps_show = fps_show_counter / time_sec;
                                    fps_show_counter = 0;
                                }
                                else
                                {
                                    ++fps_show_counter;
                                }
                                unsigned int process_delay_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - pdata.time_cap).count();
                                std::vector<std::string> messages = get_info_message(process_delay_time, pdata.frame_width, pdata.frame_height);
                                draw_info_message(pdata.cap_frame, messages);

                                cv::imshow("EMoi", pdata.cap_frame);
                                cv::waitKey(1);
                            } });
    this->join();
}
