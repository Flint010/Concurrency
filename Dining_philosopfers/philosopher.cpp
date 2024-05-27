#include "philosopher.h"

namespace dining {

Philosopher::Philosopher(Table& table, size_t seat) 
	: table_(table), seat_(seat), left_fork_(table.LeftFork(seat)), right_fork_(table.RightFork(seat)) {
}

void Philosopher::EatOneMoreTime()
{
	AcquireForks();
	Eat();
	ReleaseForks();
	Think();
}

void Philosopher::AcquireForks()
{
	// Deadlock
	//left_fork_.lock();
	//right_fork_.lock();
	//------------------------------------
	// STL 
	//std::lock(left_fork_, right_fork_);
	//-------------------------------------
	// Захват сначала меньшей по номеру вилки, затем большей
	// Освобождение сначала большей по номеру, затем меньшей

	if (seat_ < table_.ToRight(seat_)) {
		left_fork_.lock();
		right_fork_.lock();
	}
	else if (seat_ > table_.ToRight(seat_)) {
		right_fork_.lock();
		left_fork_.lock();
	}
}

void Philosopher::Eat()
{
	table_.AccessPlate(seat_);
	table_.AccessPlate(table_.ToRight(seat_));
	++eat_count_;
}

void Philosopher::ReleaseForks()
{
	/*left_fork_.unlock();
	right_fork_.unlock();*/

	if (seat_ < table_.ToRight(seat_)) {
		right_fork_.unlock();
		left_fork_.unlock();
	}
	else if (seat_ > table_.ToRight(seat_)) {
		left_fork_.unlock();
		right_fork_.unlock();
	}

}

void Philosopher::Think()
{
	std::this_thread::yield();
}

} // namespace dining