#include <list>
#include <unordered_map>
#include <iostream>

#define InsertPage(id)										  \
{															  \
	id.push_front(page);									  \
	hash_table.insert(pair<Page, PageIt<Page>>				  \
						  (page, {id.cbegin(), ListId::id})); \
															  \
}															  \

#define RemovePage(id)		\
{							\
	hash_table.erase(page);	\
	id.pop_back();			\
}							\

#define MovePage(src_id, new_id)		  \
{									 	  \
	new_id.push_front(page_it->first);	  \
	src_id.pop_back();				 	  \
	page_it->second.it = new_id.cbegin(); \
	page_it->second.id = ListId::new_id;  \
										  \
}

using namespace std;
using page_t = int;

enum class ListId
{
	T1, T2, B1, B2
};

template <typename Page>
class PageIt
{
	using ListIt = typename list<Page>::const_iterator;

	ListIt it;
	ListId id;

public:
	PageIt(ListIt it, ListId id): it(it), id(id) {};
};

template <typename Page>
class ARC
{
	unordered_map <Page, PageIt<Page>> hash_table;
	list<Page> T1, T2, B1, B2;
	size_t size, adapt_param, hits;

public:
	ARC(int sz): size(sz), adapt_param(0), hits(0) {};

	void InsertCache(const Page& page)
	{
		auto page_it = hash_table.find(page);

		if (page_it != hash_table.cend())
		{
			switch (page_it->second.id)
			{
				case ListId::T1: 	//CASE I//
					T1.erase (page_it->second.it);
					break;
				case ListId::T2:
					T2.erase (page_it->second.it);
					break;
				case ListId::B1:	//CASE II//
					adapt_param = min(size, adapt_param + max(B2.size() / B1.size(), 1));
					Replace();
					B1.erase (page_it->second.it);
					break;
				case ListId::B2:	//CASE III//
					adapt_param = max(0, adapt_param - max(B1.size() / B2.size(), 1));
					Replace();
					B2.erase (page_it->second.it);
					break;
			}

			T2.push_front(page_it->first);
			page_it->second.it = T2.cbegin();
			page_it->second.id = ListId::T2;
			++hits;
		}
		else						//CASE IV//
		{
			if (GetL1Size() == size)
			{
				if (T1.size() < size)
				{
					RemovePage(B1)
					Replace(page_it);
				}
				else RemovePage(T1)
			}
			else if (GetL1Size() < size && GetLSize() >= size)
			{
				if (GetLSize() == 2 * size) RemovePage(B1)
				Replace(page_it);
			}

			InsertPage(T1)
		}
	}

	void Replace (typename unordered_map <Page, PageIt<Page>>::const_iterator const& page_it)
	{
		bool expr = (page_it->second.id == ListId::B2) && 
					(T1.size() > adapt_param);

		if (T1.size() >= 1 && expr)	MovePage(T1, B1)
		else						MovePage(T2, B2)
	}

	size_t GetHits  () const { return hits; 					 }
	size_t GetL1Size() const { return T1.size()   + B1.size();   }
	size_t GetL2Size() const { return T2.size()   + B2.size();   }
	size_t GetLSize () const { return GetL1Size() + GetL2Size(); }
};

int main()
{
	ARC<page_t> cache{5};
	return 0;
}