#include "beladi.hpp"

using page_t = int;

int main()
{
	size_t cache_size{}, page_count{};
	std::cin >> cache_size >> page_count;

	BeladiCache<page_t> cache{cache_size, page_count};
	cache.InsertPages();

	std::cout << cache.GetHits() << std::endl;

	return 0;
}