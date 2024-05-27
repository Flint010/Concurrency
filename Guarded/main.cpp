#include <iostream>
#include <vector>

#include "Guarded.h"

int main() {

	Guarded<std::vector<int>> v;

	v->push_back(6);
	v->push_back(7);
	v->push_back(8);

	for (auto it = v->begin(); it != v->end(); it++) {
		std::cout << *it << " " << std::endl;
	}


	return 0;
}