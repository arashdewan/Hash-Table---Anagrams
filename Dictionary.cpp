
// Dictionary.cpp

// This is a correct but horribly inefficient implementation of Dictionary
// functionality.  Your job is to change DictionaryImpl to a more efficient
// correct implementation.  You can change it any way you like, subject to
// restrictions in the spec (e.g., that the only C++ library container you
// are allowed to use are vector, list, stack, and queue (and string); if you
// want anything fancier, implement it yourself).
#include "Dictionary.h"
#include <string>
#include <list>
#include <cctype>
#include <utility>  // for swap
#include <functional>
#include <algorithm>
using namespace std;

const int MAX = 50000;
void removeNonLetters(string& s);
void generateNextPermutation(string& permutation);

  // This class does the real work of the implementation.

class DictionaryImpl
{
  public:
    DictionaryImpl(int maxBuckets);
    ~DictionaryImpl();
    void insert(string word);
    void lookup(string letters, void callback(string)) const;
    unsigned int maphash(string word) const;

  private:
    int m_buckets;
    struct Bucket
    {
        list<string> m_words;
    };
    Bucket* m_hash[MAX];
};
//constructor
DictionaryImpl::DictionaryImpl(int maxBuckets) : m_buckets(maxBuckets)
{
    for(int i = 0; i < maxBuckets; i++)
        m_hash[i] = nullptr;
}
//destructor
DictionaryImpl::~DictionaryImpl()
{
    for (int i = 0; i < m_buckets; i++)
        delete m_hash[i];
}
//hash function
unsigned int DictionaryImpl::maphash(string word) const
{
    //fancy way to create a hash value for the word
    unsigned int hashval = hash<string>().operator()(word) % m_buckets;
    return hashval;
}
//insert function
void DictionaryImpl::insert(string word)
{
    //clean up word
    removeNonLetters(word);
    //sort the word to find it in the hash table
    string sorted = word;
    sort(sorted.begin(), sorted.end());
    unsigned int hashval = maphash(sorted);
    //to handle collision
    if (m_hash[hashval] != nullptr)
    {
        m_hash[hashval]->m_words.push_back(word);
        m_hash[hashval]->m_words.sort();
    }
    //if empty
    else
    {
        m_hash[hashval] = new Bucket;
        m_hash[hashval]->m_words.push_front(word);
    }
}
//lookup function
void DictionaryImpl::lookup(string letters, void callback(string)) const
{
    //clean up words
    removeNonLetters(letters);
    if (callback == nullptr && letters.empty())
        return;
    //sort to hash value
    string sorted = letters;
    sort(sorted.begin(), sorted.end());
    unsigned int hashval = maphash(sorted);
    //if no match end lookup
    if (m_hash[hashval] == nullptr)
        return;
    //iterate through all matches
    else
    {
        for(list<string>::iterator current = m_hash[hashval]->m_words.begin(); current!=m_hash[hashval]->m_words.end(); current++)
        {
            //sort current in list
            string sorted_current = *current;
            sort(sorted_current.begin(), sorted_current.end());
            //check to see if there is a match between the two (if it is an anagram)
            if (sorted_current == sorted)
                callback(*current);
        }
    }
}

void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());  // chop everything off from "to" to end.
}

  // Each successive call to this function generates the next permutation of the
  // characters in the parameter.  For example,
  //    string s = "eel";
  //    generateNextPermutation(s);  // now s == "ele"
  //    generateNextPermutation(s);  // now s == "lee"
  //    generateNextPermutation(s);  // now s == "eel"
  // You don't have to understand this function's implementation.
void generateNextPermutation(string& permutation)
{
    string::iterator last = permutation.end() - 1;
    string::iterator p;

    for (p = last; p != permutation.begin()  &&  *p <= *(p-1); p--)
        ;
    if (p != permutation.begin())
    {
        string::iterator q;
        for (q = p+1; q <= last  &&  *q > *(p-1); q++)
            ;
        swap(*(p-1), *(q-1));
    }
    for ( ; p < last; p++, last--)
        swap(*p, *last);
}

//******************** Dictionary functions ******************************

// These functions simply delegate to DictionaryImpl's functions
// You probably don't want to change any of this code

Dictionary::Dictionary(int maxBuckets)
{
    m_impl = new DictionaryImpl(maxBuckets);
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters,callback);
}

