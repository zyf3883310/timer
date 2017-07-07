#ifndef _TIMER_H_
#define _TIMER_H_
#include <thread>
#include <mutex>
#include <functional>
#include <map>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif 
class CTimer
{
public:
	static CTimer &Instance() {
		return instance_;
	}

	struct job_t {
		int id_;
		uint32_t interval_; //millisecond
		std::function<void()> handle_;
		uint64_t expiry_;
	};

	void Start();
	void Stop();
	bool Regist(int id, uint32_t interval, std::function<void()> handle, bool immediate = false);
	void UnRegist(int id);
protected:
	void Run();
private:
	uint64_t GetCurrentTime();
private:
	static void Thr_Hook();
	CTimer();
	CTimer(const CTimer &) = delete;
	CTimer& operator=(const CTimer &) = delete;

	static CTimer instance_;
	std::map<int, job_t> jobs_;
	std::mutex mtx_;
	std::thread *thr_;
	volatile bool run_;
};



#endif //_TIMER_H_
