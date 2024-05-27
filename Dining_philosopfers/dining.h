#pragma once
#include <mutex>

#include <atomic>
#include <vector>

namespace dining {
	
using Fork = std::mutex;

class Plate {
public:
	void Access();
private:
	std::atomic<bool> accessed_{ false };
};

class Table {
public:
	Table(size_t num_seats);
			
	inline Fork& LeftFork(size_t seat) {
		return forks_[seat % num_seats_];
	}

	inline Fork& RightFork(size_t seat) {
		return forks_[ToRight(seat)];
	}

	inline size_t ToRight(size_t seat) const {
		return (seat + 1) % num_seats_;
	}

	void AccessPlate(size_t seat) {
		plates_[seat].Access();
	}

private:
	size_t num_seats_;
	std::vector<Plate> plates_;
	std::vector<Fork> forks_;
};

} // namespace dining

