#include "timer.h"

CTimer CTimer::instance_;

bool CTimer::Regist(int id, uint32_t interval, std::function<void()> handle, bool immediate) {
	std::unique_lock<std::mutex> lock(mtx_);
	auto itr = jobs_.find(id);
	if (jobs_.end() != itr) return false;

	job_t job = { id, interval*1000, handle };
	if (immediate) job.expiry_ = GetCurrentTime();
	else job.expiry_ = GetCurrentTime() + interval * 1000;

	jobs_[id] = job;
	return true;
}
void CTimer::UnRegist(int id) {
	std::unique_lock<std::mutex> lock(mtx_);
	jobs_.erase(id);
}

void CTimer::Run() {
	uint64_t current = 0;
	while (run_) {
		current = GetCurrentTime();
		{
			std::unique_lock<std::mutex> lock(mtx_);
			for (auto &itr : jobs_) {
				if (itr.second.expiry_ <= current) {
					itr.second.handle_();
					current = GetCurrentTime();
					itr.second.expiry_ = current + itr.second.interval_;
				}
			}
		}
#ifdef  _WIN32
		Sleep(1);
#else
		usleep(1);
#endif 
	}
}

void CTimer::Thr_Hook() {
	instance_.Run();
}

CTimer::CTimer()
: thr_(nullptr)
, run_(false) {

}

void CTimer::Start() {
	if (nullptr == thr_) {
		run_ = true;
		thr_ = new std::thread(Thr_Hook);
	}
}

void CTimer::Stop() {
	if (nullptr != thr_) {
		run_ = false;
		thr_->join();
		delete thr_;
		thr_ = nullptr;
	}
}

uint64_t CTimer::GetCurrentTime()
{
#ifdef _WIN32
	return ::GetTickCount64();
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	uint64_t seconds = tv.tv_sec;
	return seconds * 1000 + tv.tv_usec /1000  ;
#endif

}

