#include <list>
#include <unordered_map>
#include <iostream>


#define INSERT_PAGE(id, page_)								\
{															\
	id.push_front(page_);									\
	hash_table.insert({page_, {id.cbegin(), ListId::id}});	\
}															\

#define REMOVE_LAST_PAGE(id)			\
{										\
	hash_table.erase(*(--id.end()));	\
	id.pop_back();						\
}										\

#define MOVE_LAST_PAGE(src_id, new_id)		\
{											\
	hash_table.erase  (*(--src_id.end()));	\
	INSERT_PAGE(new_id, *(--src_id.end()))	\
	src_id.pop_back();						\
}											\

using page_t = int;

//------------------------------------------------------------------------------------------------------------

enum class ListId
{
	Null, T1, T2, B1, B2
};

//------------------------------------------------------------------------------------------------------------

template <typename Page>
class PageIt
{
	using ListIt = typename std::list<Page>::const_iterator;
public:
	ListIt it;
	ListId id;
	PageIt(ListIt it, ListId id): it(it), id(id) {};
};

//------------------------------------------------------------------------------------------------------------

template <typename Page, typename PageNum = int>
class ARC
{
	std::unordered_map <PageNum, PageIt<Page>> hash_table;
	std::list<Page> T1, T2, B1, B2;
	size_t size, hits;
	long long adapt_param;

public:
	ARC(size_t sz): size{sz}, hits{0}, adapt_param{0} {};

	void InsertCache(const Page& page)
	{
		auto page_it = hash_table.find(page);

		if (page_it != hash_table.cend())
		{
			PageInCache(page_it->second, page);

			hash_table.erase(page);
			INSERT_PAGE(T2, page)
			++hits;
		}
		else PageNotFound(page);
	}

	size_t GetHits  () const { return hits; 					 }
	size_t GetL1Size() const { return T1.size()   + B1.size();   }
	size_t GetL2Size() const { return T2.size()   + B2.size();   }
	size_t GetLSize () const { return GetL1Size() + GetL2Size(); }

	void PrintCache() const
	{
		std::cout << "Hash Table: ";
		for (auto it: hash_table) std::cout << it.first << " ";

		std::cout << std::endl << "T1: ";
		for (auto it: T1) std::cout << it << " ";

		std::cout << std::endl << "B1: ";
		for (auto it: B1) std::cout << it << " ";

		std::cout << std::endl << "T2: ";
		for (auto it: T2) std::cout << it << " ";

		std::cout << std::endl << "B2: ";
		for (auto it: B2) std::cout << it << " ";

		std::cout << std::endl;
	}

private:
	void Replace(ListId page_id, Page const& page)
	{
		bool expr = (page_id == ListId::B2 &&
					T1.size() == adapt_param) || (T1.size() > adapt_param);

		if (T1.size() >= 1 && expr)
			MOVE_LAST_PAGE(T1, B1)
		else
			MOVE_LAST_PAGE(T2, B2)
	}

	void PageInCache(const PageIt<Page>& page_it, const Page& page)
	{
		switch (page_it.id)
		{
			case ListId::T1:
				T1.erase(page_it.it);
				break;
			case ListId::T2:
				T2.erase(page_it.it);
				break;
			case ListId::B1:
				adapt_param = std::min<long long>(size, adapt_param + 
										std::max<long long>(B2.size() / B1.size(), 1));
				Replace (ListId::B1, page);
				B1.erase(page_it.it);
				break;
			case ListId::B2:
				adapt_param = std::max<long long>(0, adapt_param - 
										std::max<long long>(B1.size() / B2.size(), 1));
				Replace (ListId::B2, page);
				B2.erase(page_it.it);
				break;
		}
	}

	void PageNotFound(const Page& page)
	{
		if (GetL1Size() == size)
		{
			if (T1.size() < size)
			{
				REMOVE_LAST_PAGE(B1)
				Replace(ListId::Null, page);
			}
			else REMOVE_LAST_PAGE(T1)
		}
		else if (GetL1Size() < size && GetLSize() >= size)
		{
			if (GetLSize() == 2 * size) 
				REMOVE_LAST_PAGE(B2)
			
			Replace(ListId::Null, page);
		}

		INSERT_PAGE(T1, page)
	}
};

int main()
{
	size_t cache_size{};

	std::cin >> cache_size;

	ARC<page_t> cache {cache_size};

	page_t page{};
	size_t page_count{};

	std::cin >> page_count;

	for (int i = 0; i < page_count; i++)
	{
		std::cin >> page;
		cache.InsertCache(page);
	}

	std::cout << "Hits: " << cache.GetHits() << std::endl;

	return 0;
}