#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

using page_t = int;

template <typename Page>
class BeladiCache
{
	size_t size, hits;
	std::unordered_set<Page> cache;
	std::unordered_multimap<Page, size_t> pages_table;
	std::vector <Page> pages;

public:
	BeladiCache(size_t size, size_t page_cnt):
		size{2 * size}, hits{0}, cache(size),
		pages_table(page_cnt), pages(page_cnt) 
	{
		for (size_t i = 0; i < page_cnt; i++)
		{
			std::cin >> pages[i];
			pages_table.insert({pages[i], i});
		}
	}

	void InsertPages()
	{
		for (size_t i = 0; i < pages.size(); ++i)
		{
			auto page_it = cache.find(pages[i]);

			if (page_it == cache.cend())
			{
				if (cache.size() == size)
					DeleteLastUsedPage();
				cache.insert(pages[i]);
			}
			else hits++;
		}
	}

private:
	void DeleteLastUsedPage()
	{
		std::pair<typename std::unordered_multimap<Page, size_t>::iterator, size_t> lup;
		lup.second = -1;

		for (auto it = cache.begin(); it != cache.end(); it++)
		{
			auto page_it = pages_table.find(*it);

			if (page_it != pages_table.cend())
			{
				for (auto equal_keys = page_it; equal_keys != pages_table.cend(); equal_keys++)
					if (lup.second < page_it->second)
					{
						lup.first  = page_it;
						lup.second = page_it->second;
					}
			}
			else
			{
				cache.erase(*it);
				return;
			}
		}

		cache.erase(lup.first->first);
		pages_table.erase(lup.first);
	}
};

int main()
{
	size_t cache_size{}, page_count{};
	std::cin >> cache_size >> page_count;

	BeladiCache<page_t> cache{cache_size, page_count};

	cache.InsertPages();

	std::cout << "Wohoo compiled" << std::endl; 

	return 0;
}