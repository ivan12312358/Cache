#include <list>
#include <unordered_map>
#include <iostream>

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
	ARC(size_t sz): size{sz/2 > 0 ? sz/2: 1}, hits{0}, adapt_param{0} {};

	void InsertCache(const Page& page)
	{
		auto page_it = hash_table.find(page);

		if (page_it != hash_table.cend())
		{
			PageInCache(page_it->second, page);

			hash_table.erase(page);
			T2.push_front(page);
			hash_table.insert({page, {T2.cbegin(), ListId::T2}});
			++hits;
		}
		else PageNotFound(page);
	}

	size_t GetHits  () const { return hits; 					 }
	size_t GetL1Size() const { return T1.size()   + B1.size();   }
	size_t GetL2Size() const { return T2.size()   + B2.size();   }
	size_t GetLSize () const { return GetL1Size() + GetL2Size(); }

private:
	void Replace(ListId page_id)
	{
		bool expr = (page_id == ListId::B2 &&
					T1.size() == adapt_param) || (T1.size() > adapt_param);

		if (T1.size() >= 1 && expr)
		{
			Page page = *(--T1.end());
			hash_table.erase (page);
			T1.pop_back();
			B1.push_front(page);
			hash_table.insert({page, {B1.cbegin(), ListId::B1}});
		}
		else
		{
			Page page = *(--T2.end());
			hash_table.erase (page);
			T2.pop_back();
			B2.push_front(page);
			hash_table.insert({page, {B2.cbegin(), ListId::B2}});
		}
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
				Replace (ListId::B1);
				B1.erase(page_it.it);
				break;
			case ListId::B2:
				adapt_param = std::max<long long>(0, adapt_param - 
										std::max<long long>(B1.size() / B2.size(), 1));
				Replace (ListId::B2);
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
				hash_table.erase(*(--B1.end()));
				B1.pop_back();
				Replace(ListId::Null);
			}
			else
			{
				hash_table.erase(*(--T1.end()));
				T1.pop_back();
			}
		}
		else if (GetL1Size() < size && GetLSize() >= size)
		{
			if (GetLSize() == 2 * size)
			{
				hash_table.erase(*(--B2.end()));
				B2.pop_back();
			}	
			Replace(ListId::Null);
		}

		T1.push_front(page);
		hash_table.insert({page, {T1.cbegin(), ListId::T1}});
	}
};

//------------------------------------------------------------------------------------------------------------
