#include "application.h"
#include <exception>
#include <signal.h>
#include <stdlib.h>

// #define WINDOW_NAME "EMoi"

class InterruptException : public std::exception
{
public:
    InterruptException(int s) : S(s) {}
    int S;
};

void sig_to_exception(int s)
{
    throw InterruptException(s);
}

int main(int argc, char **argv)
{
    AN::Application main_app;

    try
    {
        main_app.setup();
        main_app.loop();
    }
    catch (InterruptException &e)
    {
        std::cout << "Caught signal " << e.S << std::endl;
        main_app.shutdown();
        if (main_app.joinable())
            main_app.join();
        cv::destroyAllWindows();
    }
    // cv::Mat no_signal_frame = cv::imread("../no-signal.png");

    // cv::namedWindow(WINDOW_NAME, cv::WINDOW_KEEPRATIO);
    // if (config.app.is_fullscreen)
    //     cv::setWindowProperty(WINDOW_NAME, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    // else
    //     cv::setWindowProperty(WINDOW_NAME, cv::WND_PROP_ASPECT_RATIO, cv::WINDOW_KEEPRATIO);

    // if (config.app.single_track || config.director.is_control_camera || config.app.clientMode)
    // {
    //     on_mouse_params *params = new on_mouse_params(&final_data, &classes, &last_single_tracking_box);
    //     cv::setMouseCallback(WINDOW_NAME, onMouse, reinterpret_cast<void *>(params));
    // }

    // cv::namedWindow("EMoi", cv::WINDOW_KEEPRATIO);
    // cv::imshow("EMoi", no_signal_frame);
    // cv::waitKey();
    return 0;
}
