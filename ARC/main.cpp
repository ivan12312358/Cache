#include "arc.hpp"

using page_t = int;

int main()
{
	size_t cache_size{}, page_count{};
	std::cin >> cache_size >> page_count;

	if (std::cin.bad())
	{
		perror("Input Error:");
		return -1;
	}

	cache::ARC<page_t, page_t> cache {cache_size};

	page_t  page{}, key{};

	for (int i = 0; i < page_count; ++i)
	{
		std::cin >> page;
		key = page;

		if (std::cin.good())
		{
			if(!cache.LookUp(key))
				cache.InsertPage(page);
		}
	}

	std::cout << cache.GetHits() << std::endl;

	return 0;
}