#include "arc.hpp"

using page_t = int;

int main()
{
	size_t cache_size{}, page_count{};
	std::cin >> cache_size >> page_count;

	ARC<page_t> cache {cache_size};

	page_t page{};

	for (int i = 0; i < page_count; i++)
	{
		std::cin >> page;
		cache.InsertCache(page);
	}

	std::cout << cache.GetHits() << std::endl;

	return 0;
}