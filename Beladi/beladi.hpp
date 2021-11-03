#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

template <typename Page>
class BeladiCache
{
	size_t size, hits;
	std::unordered_set<Page> cache;
	std::unordered_multimap<Page, size_t> pages_table;
	std::vector <Page> pages;

public:
	BeladiCache(size_t size, size_t page_cnt): 
		size{size}, hits{0}, 
		cache{}, pages_table{}, pages{}
	{
		pages.resize(page_cnt);

		for (size_t i = 0; i < pages.size(); ++i)
			std::cin >> pages[i];

		for (long long i = pages.size() - 1; i >= 0; --i)
			pages_table.insert({pages[i], i});
	}


	void InsertPages()
	{
		for (size_t i = 0; i != pages.size(); ++i)
		{
			auto page_it = cache.find(pages[i]);
			if  (page_it == cache.cend())
			{
				if (cache.size() == size)
					DeleteLastUsedPage();

				cache.insert(pages[i]);
			}
			else hits++;

			auto table_it = pages_table.find(pages[i]);
			pages_table.erase(table_it);
		}
	}

	size_t GetHits() const { return hits; }

private:
	void DeleteLastUsedPage()
	{
		std::pair<Page, size_t> lup{};
		lup.second = 0;

		for (auto it = cache.begin(), it_end = cache.end(); it != it_end; ++it)
		{
			auto page_it = pages_table.find(*it);

			if (page_it != pages_table.cend())
			{
				if (lup.second < page_it->second)
					lup = *page_it;
			}
			else
			{
				cache.erase(*it);
				return;
			}
		}

		cache.erase(lup.first);
	}
};