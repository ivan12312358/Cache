#include <list>
#include <unordered_map>
#include <iostream>

using namespace std;

enum class ListId
{
	Null, T1, T2, B1, B2
};

template <typename Page>
class PageIt
{
	using ListIt = typename list<Page>::iterator;

	ListIt it;
	ListId num;

public:
	PageIt(ListIt it, ListId num): it(it), num(num) {};
};

template <typename Page>
class ARC
{
	unordered_map <Page, PageIt<Page>> hash_table;
	list<Page> T1, T2, B1, B2;
	size_t size;
	int adapt_param;
	int hits;

public:
//if remove from B1, B2 ==>> remove from table
//auto iterator = list.find(key)
//size(T1 + B1) === size(T2 + B2) === size
	ARC(int sz): size(sz), adapt_param(0), hits(0) {};

	void PageInsert(Page page, ListId id)
	{
		T1.push_front(page);

		hash_table.insert(pair<Page, PageIt<Page>>
							  (page, {T1.begin(), ListId::T1}));
/* 		{
			auto iter = hash_table.find(page);

			if(iter != hash_table.end())
				cout << "Found" << endl;
			else
				cout << "Not Found" << endl;
		}
 */	
	}

	void PageRemove(Page page, ListId id)
	{
		hash_table.erase(page);
		T1.pop_back();
	}
	
	void Replace()
	{

	}
};

int main()
{
	ARC<int> cache(5);

//	list<int> T1;

	cache.PageInsert(775, ListId::T1);
	cache.PageRemove(775, ListId::T1);

    return 0;
}