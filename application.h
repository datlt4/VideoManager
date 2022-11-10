#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <vector>
#include <opencv2/opencv.hpp>
#include "VideoManager.h"
#include <mutex>
#include <thread>
#include <queue>
#include <chrono>
#include <memory>
#include <atomic>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

#define DEFAULT_FONT cv::FONT_HERSHEY_COMPLEX_SMALL
const cv::Scalar COLOR_GREEN(50, 255, 0);

namespace AN
{
    // TODO: Minimize, optimize this
    struct pipeline_data
    {
        cv::Mat cap_frame;
        std::chrono::steady_clock::time_point time_cap;
        int frame_width;
        int frame_height;
        pipeline_data(){};
        pipeline_data(cv::Mat frame) : cap_frame(frame){};
    };

    template <typename T>
    class send_one_replaceable_object
    {
        std::atomic<T *> a_ptr = {nullptr};

    public:
        void send(T const &_obj)
        {
            T *new_ptr = new T;
            *new_ptr = _obj;
            // TODO: The `unique_ptr` prevents a scary memory leak, why?
            std::unique_ptr<T> old_ptr(a_ptr.exchange(new_ptr));
        }

        T receive()
        {
            std::unique_ptr<T> ptr;
            do
            {
                while (!a_ptr)
                    std::this_thread::sleep_for(std::chrono::milliseconds(3));
                ptr.reset(a_ptr.exchange(nullptr));
            } while (!ptr);
            return *ptr;
        }
    };

    class Application
    {
    public:
        Application();
        void setup();
        void loop();
        void shutdown();
        bool joinable();
        void join();

    private:
        std::vector<std::string> input_sources;
        std::vector<std::string> output_sources;
        std::unique_ptr<AN::VideoManager> video_manager;

        // FPS compute
        std::atomic<int> fps_cap_counter{0}, fps_show_counter{0};
        std::atomic<int> current_fps_cap{0}, current_fps_show{0};
        std::chrono::steady_clock::time_point time_since_last_fps_cap_tick, time_since_last_fps_show_tick;

        // threads
        std::thread t_cap, t_show;

        // pipeline
        send_one_replaceable_object<pipeline_data> cap2show;

        // flags
        bool shutdown_program;

        // messages
        std::vector<std::string> get_info_message(unsigned int process_delay_time, int width, int height);
        void draw_info_message(cv::Mat image, std::vector<std::string> &messages);
    };
}

#endif // _APPLICATION_H_
