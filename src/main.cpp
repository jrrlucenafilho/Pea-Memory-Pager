#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <deque>
#include <algorithm>

using namespace std;

vector<int> ReadInstance(string instance_path, int& frame_quant)
{
    vector<int> page_refs;
    int number;
    ifstream fp(instance_path);

    if(!fp){
        cerr << "Error opening file " << instance_path << ' ';
        return page_refs;
    }

    fp >> frame_quant;

    while(fp >> number){
        page_refs.push_back(number);
    }

    fp.close();

    return page_refs;
}

// Util functions
// Util func for optimal algorithm: Check, from all elements curr in deque has the longest time to be accessed
// from curr_index's element in page_refs. And return it's index in the deque
int FurthestFrameForward(vector<int>& page_refs, deque<int>& page_deque, int curr_index)
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

// Util func for LRU: Does the same as FurthestFrameForward(). But for already-acessed frames in the sequence
// returning the index of the frame (in page_deque) that's furthest back (in page_refs sequence)
int FurthestFrameBackward(vector<int>& page_refs, deque<int>& page_deque, int curr_index)
{
    int dist;
    int max_dist = 0;
    int index_to_replace = -1;

    for(int i = 0; i < (int)page_deque.size(); i++){
        // Now use reverse iterators (easier for backwards checking) to search for the first occurrence of the value backwards
        auto iter = find(page_refs.rbegin() + (page_refs.size() - curr_index), page_refs.rend(), page_deque[i]);

        if(iter != page_refs.rend()){
            // Calc distance from curr_index to the first-found-position (backwards) of the deque[i] element
            dist = (iter - page_refs.rbegin()) - (page_refs.size() - curr_index);

            if(dist > max_dist){
                max_dist = dist;
                index_to_replace = i;
            }
        }
    }

    return index_to_replace;
}

// Algorithms, each returns the number of page misses that happened
int FirstInFirstOut(vector<int>& page_refs, int frame_quant)
{
    deque<int> page_deque;  // double-ended queue for iterators and extra pop funcs
    int page_misses = 0;

    // First fill up the deque with the first 'frame_quant' values of page_refs vec
    page_deque.insert(page_deque.end(), page_refs.begin(), page_refs.begin() + frame_quant);

    // Add up initial page_misses. If the page_refs sequence is shorter than frame_quant (rare edge case), add up it's size instead
    page_misses += min(frame_quant, (int)page_refs.size());

    // Then go through page_refs checking it, starting from the element after frame_quant elements from beginning
    for(int i = frame_quant; i < (int)page_refs.size(); i++){
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

int OptimalAlgorithm(vector<int>& page_refs, int frame_quant)
{
    deque<int> page_deque;
    int page_misses = 0;
    int index_to_replace;

    page_deque.insert(page_deque.end(), page_refs.begin(), page_refs.begin() + frame_quant);
    page_misses += min(frame_quant, (int)page_refs.size());

    for(int i = frame_quant; i < (int)page_refs.size(); i++){
        if(find(page_deque.begin(), page_deque.end(), page_refs[i]) != page_deque.end()){
            continue;
        }

        // Get the deque index of the element that'll be removed
        index_to_replace = FurthestFrameForward(page_refs, page_deque, i);

        // Actually replace it
        if(index_to_replace != -1){
            page_deque[index_to_replace] = page_refs[i];
        }
        page_misses++;
    }

    return page_misses;
}

int LeastRecentlyUsed(vector<int>& page_refs, int frame_quant)
{
    deque<int> page_deque;
    int page_misses = 0;
    int index_to_replace;

    page_deque.insert(page_deque.end(), page_refs.begin(), page_refs.begin() + frame_quant);
    page_misses += min(frame_quant, (int)page_refs.size());

    for(int i = frame_quant; i < (int)page_refs.size(); i++){
        if(find(page_deque.begin(), page_deque.end(), page_refs[i]) != page_deque.end()){
            continue;
        }

        index_to_replace = FurthestFrameBackward(page_refs, page_deque, i);

        if(index_to_replace != -1){
            page_deque[index_to_replace] = page_refs[i];
        }
        page_misses++;
    }

    return page_misses;
}

int main(void)
{
    int frame_quant;
    string instance_path;
    vector<int> page_refs;

    cin >> instance_path;

    page_refs = ReadInstance(instance_path, frame_quant);

    if(page_refs.empty()){
        cerr << "Sequence is empty!\n";
        return 1;
    }

    cout << "FIFO " << FirstInFirstOut(page_refs, frame_quant) << '\n';
    cout << "OTM " << OptimalAlgorithm(page_refs, frame_quant) << '\n';
    cout << "LRU " << LeastRecentlyUsed(page_refs, frame_quant) << '\n';

    return 0;
}
