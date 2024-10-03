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

// Util func for optimal algorithm: Check, from all elements curr in deque has the longest time to be accessed
// from curr_index's element in page_refs. And return it's index in the deque
int CalcLongestToBeAccessed(vector<int>& page_refs, deque<int>& page_deque, int curr_index)
{
    int dist;
    int max_dist = 0;
    int index_to_replace = -1;

    for(int i = 0; i < (int)page_deque.size(); ++i){
        // Find the position of the next occurrence of this deque element in the vector, from curr_index's element
        // And if it's found it, save it in dist
        auto iter = find(page_refs.begin() + curr_index, page_refs.end(), page_deque[i]);
        dist = (iter != page_refs.end()) ? distance(page_refs.begin() + curr_index, iter) : page_refs.size();
        
        // Compare this dist with all calc'd and save the dist itself and the index of the ele in the deque
        if(dist > max_dist){
            max_dist = dist;
            index_to_replace = i;
        }
    }

    return index_to_replace;
}

// Algorithms, each returns the number of page misses that happened
// Assumes page_quant is always smaller than page_ref list
int FirstInFirstOut(vector<int>& page_refs, int page_quant)
{
    deque<int> page_deque;  // double-ended queue for iterators and extra pop funcs
    int page_misses = 0;

    // First fill up the deque with the first 'page_quant' values of page_refs vec
    page_deque.insert(page_deque.end(), page_refs.begin(), page_refs.begin() + page_quant);
    page_misses += page_quant;

    // Then go through page_refs checking it, starting from the element after page_quant elements from beginning
    for(int i = page_quant; i < (int)page_refs.size(); i++){
        // Check if it's found curr iter's number in the deque. If so, no page missed and skip to next iter
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
    deque<int> page_deque;
    int page_misses = 0;
    int index_to_replace;

    page_deque.insert(page_deque.end(), page_refs.begin(), page_refs.begin() + page_quant);
    page_misses += page_quant;

    for(int i = page_quant; i < (int)page_refs.size(); i++){
        if(find(page_deque.begin(), page_deque.end(), page_refs[i]) != page_deque.end()){
            continue;
        }

        // Get the deque index of the element that'll be removed
        index_to_replace = CalcLongestToBeAccessed(page_refs, page_deque, i);

        // Actually replace it
        if(index_to_replace != -1){
            page_deque[index_to_replace] = page_refs[i];
        }
        page_misses++;
    }

    return page_misses;
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
    cout << "OTM " << OptimalAlgorithm(page_refs, page_quant) << '\n';
    //cout << "LRU " << LeastRecentlyUsed(page_refs, page_quant) << '\n';

    return 0;
}
