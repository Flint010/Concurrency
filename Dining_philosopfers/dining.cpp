#include "dining.h"
#include <cassert>

namespace dining {

	void Plate::Access()
	{
		assert(accessed_.exchange(true, std::memory_order_relaxed) == false, "Mutual exclusion violated false->true");

		std::this_thread::yield();

		assert(accessed_.exchange(false, std::memory_order_relaxed) == true, "Mutual exclusion violated true->false");

	}

	Table::Table(size_t num_seats)
		: num_seats_(num_seats), plates_(num_seats), forks_(num_seats_) {
	}

};

