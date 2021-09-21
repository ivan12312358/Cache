#include <list>
#include <unordered_map>
#include <iostream>

#define InsertPage(id)										 \
{															 \
	id.push_front(page);									 \
	hash_table.insert(pair<Page, PageIt<Page>>				 \
						  (page, {id.begin(), ListId::id})); \
															 \
}															 \

#define RemovePage(id)		\
{							\
	hash_table.erase(page);	\
	id.pop_back();			\
}							\

using namespace std;
using page = int;

enum class ListId
{
	T1, T2, B1, B2
};

template <typename Page>
class PageIt
{
	using ListIt = typename list<Page>::iterator;

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

	void CacheInsert(const Page& page)
	{
		if (hash_table.find(page) != hash_table.cend())
		{

		}
		else		//CASE IV//
		{
			if (GetL1Size() == size)
			{
				if (T1.size() < size)
				{
					RemovePage(B1);
					Replace();
				}
				else RemovePage(T1);
			}
			else if (GetL1Size() < size && GetLSize() >= size)
			{
				if (GetLSize() == 2 * size) RemovePage(B1);
				Replace();
			}

			InsertPage(T1);
		}
	}

	void Replace()
	{

	}

	size_t GetHits  () const { return hits; }
	size_t GetL1Size() const { return T1.size()   + B1.size();   }
	size_t GetL2Size() const { return T2.size()   + B2.size();   }
	size_t GetLSize () const { return GetL1Size() + GetL2Size(); }

//size(T1 + B1) === size(T2 + B2) === size
};

int main()
{
	ARC<page> cache {5};

    return 0;
}