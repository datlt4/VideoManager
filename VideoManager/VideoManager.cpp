#include "VideoManager.h"

// VideoManager
AN::VideoManager::VideoManager(std::vector<std::string> &input_sources, std::vector<std::string> &output_sources) : no_signal_mat{cv::imread(NO_SIGNALE_IMAGE_PATH)}
{
	for (std::string &src : input_sources)
	{
		vector_capture.push_back(AN::VideoCapture(src));
	}

	for (std::string &src : output_sources)
	{
		vector_writer.push_back(AN::VideoWriter(src));
	}
	current_vector_writer_index = 0;
}

bool AN::VideoManager::is_opened()
{
	return vector_capture[current_vector_capture_index].is_opened();
}

bool AN::VideoManager::read(cv::Mat &image)
{
	cv::Mat read_frame;
	if (vector_capture[current_vector_capture_index].read(read_frame))
	{
		if (vector_capture[current_vector_capture_index].get_height_frame() > 0 && vector_capture[current_vector_capture_index].get_width_frame() > 0)
		{
			cv::resize(read_frame, image, cv::Size2i(vector_capture[current_vector_capture_index].get_width_frame(), vector_capture[current_vector_capture_index].get_height_frame()));
		}
		else
		{
			read_frame.copyTo(image);
		}
		unsigned int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time_since_last_connect).count();
		std::cout << __GREEN__ << elapsed << __ENDC__ << std::endl;
	}
	else
	{
		if (vector_capture[current_vector_capture_index].get_height_frame() > 0 && vector_capture[current_vector_capture_index].get_width_frame() > 0)
		{
			cv::resize(this->no_signal_mat, image, cv::Size2i(vector_capture[current_vector_capture_index].get_width_frame(), vector_capture[current_vector_capture_index].get_height_frame()));
		}
		else
		{
			no_signal_mat.copyTo(image);
		}
	}
}

void AN::VideoManager::start_video_capture(int index)
{
	current_vector_capture_index = index;
	if (current_vector_capture_index < 0 || current_vector_capture_index >= vector_capture.size())
		std::cerr << __RED__ << __BOLD__ << "[ " << __LINE__ << " ]: " << __ENDC__ << __RED__ << "Index out of range" << __ENDC__ << std::endl;
	else
		std::cout << __GREEN__ << "[ " << __LINE__ << " ]: " << __ENDC__ << __GREEN__ << "OK" << __ENDC__ << std::endl;
	vector_capture[current_vector_capture_index].create_connection();
	time_since_last_connect = std::chrono::high_resolution_clock::now();
}
