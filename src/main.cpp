#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <deque>
#include <algorithm>

using namespace std;

vector<int> ReadInstance(string instance_path, int& page_quant)
{
    vector<int> page_refs;
    int number;
    ifstream fp("instances/" + instance_path);

    if(!fp){
        cerr << "Error opening file" << instance_path;
        return page_refs;
    }

    fp >> page_quant;

    while(fp >> number){
        page_refs.push_back(number);
    }

    fp.close();

    return page_refs;
}

// Algorithms, each returns the number of page misses that happened
// Assumes page_quant is always smaller than page_ref list
int FirstInFirstOut(vector<int>& page_refs, int page_quant)
{
    deque<int> page_deque;  //double-ended queue for iterators
    int page_misses = 0;

    // First fill up the deque with the first 'page_quant' values of page_refs vec
    page_deque.insert(page_deque.end(), page_refs.begin(), page_refs.begin() + page_quant);
    page_misses += page_quant;

    // Then go through page_refs checking it, starting from the element after page_quant elements from beginning
    for(int i = page_quant; i < (int)page_refs.size(); i++){
        // Check if it's found curr iter's inserted page in the instance sequence. If so, skip this iter
        if(find(page_deque.begin(), page_deque.end(), page_refs[i]) != page_deque.end()){
            continue;
        }

        // If not, remove front and push value to back of deque, and add one to page_misses, ofc
        page_deque.pop_front();
        page_deque.push_back(page_refs[i]);
        page_misses++;
    }

    return page_misses;
}

int OptimalAlgorithm(vector<int>& page_refs, int page_quant)
{

}

int LeastRecentlyUsed(vector<int>& page_refs, int page_quant)
{

}

int main(int argc, char *argv[])
{
    int page_quant;
    string instance_path;
    vector<int> page_refs;

    //cin >> instance_path; //TODO: Remember to change when finished debugging

    page_refs = ReadInstance(argv[1], page_quant);

    if(page_refs.empty()){
        return 1;
    }

    cout << "FIFO " << FirstInFirstOut(page_refs, page_quant) << '\n';
    //cout << "OTM " << OptimalAlgorithm(page_refs, page_quant) << '\n';
    //cout << "LRU " << LeastRecentlyUsed(page_refs, page_quant) << '\n';

    return 0;
}
