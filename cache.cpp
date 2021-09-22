#include <list>
#include <unordered_map>
#include <iostream>


#define InsertPage(id)										  \
{															  \
	id.push_front(page);									  \
	hash_table.insert({page, {id.cbegin(), ListId::id}});	  \
															  \
}															  \

#define RemovePage(id)		\
{							\
	id.pop_back();			\
	hash_table.erase(page);	\
}							\

#define MovePage(src_id, new_id)	\
{									\
	RemovePage(src_id)				\
	InsertPage(new_id)				\
}									\

using namespace std;
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
	using ListIt = typename list<Page>::const_iterator;
public:
	ListIt it;
	ListId id;
	PageIt(ListIt it, ListId id): it(it), id(id) {};
};

//------------------------------------------------------------------------------------------------------------

template <typename Page>
class ARC
{
	unordered_map <Page, PageIt<Page>> hash_table;
	list<Page> T1, T2, B1, B2;
	size_t size, adapt_param, hits;

	void Replace (ListId page_id, Page const& page)
	{
		bool expr = (page_id == ListId::B2 &&
					T1.size() == adapt_param) || (T1.size() > adapt_param);

			if (T1.size() >= 1 && expr)
				MovePage(T1, B1)
			else
				MovePage(T2, B2)
	}


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
					adapt_param = min<size_t>(size, adapt_param + 
											  max<size_t>(B2.size() / B1.size(), 1));
					Replace (ListId::B1, page);
					B1.erase(page_it->second.it);
					break;
				case ListId::B2:	//CASE III//
					adapt_param = max<size_t>(0, adapt_param - 
											  max<size_t>(B1.size() / B2.size(), 1));
					Replace (ListId::B2, page);
					B2.erase(page_it->second.it);
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
					Replace(ListId::Null, page);
				}
				else RemovePage(T1)
			}
			else if (GetL1Size() < size && GetLSize() >= size)
			{
				if (GetLSize() == 2 * size) RemovePage(B2)
				Replace(ListId::Null, page);
			}

			InsertPage(T1)
		}
	}

	size_t GetHits  () const { return hits; 					 }
	size_t GetL1Size() const { return T1.size()   + B1.size();   }
	size_t GetL2Size() const { return T2.size()   + B2.size();   }
	size_t GetLSize () const { return GetL1Size() + GetL2Size(); }

	void PrintCache() const
	{
		cout << "T1: ";
		for (auto it: T1) cout << it << " ";

		cout << endl << "B1: ";
		for (auto it: B1) cout << it << " ";

		cout << endl << "T2: ";
		for (auto it: T2) cout << it << " ";

		cout << endl << "B2: ";
		for (auto it: B2) cout << it << " ";

		cout << endl;
	}
};

int main()
{
	ARC<page_t> cache {5};

	page_t page {};

	while(true)
	{
		cin >> page;
		cache.InsertCache(page);
		cache.PrintCache();
		cout << "Hits: " << cache.GetHits() << endl;
	}

	return 0;
}