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
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

namespace AN
{
    // TODO: Minimize, optimize this
    struct pipeline_data
    {
        cv::Mat cap_frame;
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

    private:
        std::vector<std::string> input_sources;
        std::vector<std::string> output_sources;
        std::unique_ptr<AN::VideoManager> video_manager;
    };
}

#endif // _APPLICATION_H_
