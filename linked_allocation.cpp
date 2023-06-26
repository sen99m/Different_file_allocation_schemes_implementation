#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <cmath>
#define total_num_blocks 15
#define block_size 5 // each block can hold 5 character data string
using namespace std;
int free_blocks = total_num_blocks;
// each block is capable of holding 5 char only
typedef struct Block
{
    char data[5];
    struct Block* ptr_to_next_block;
}Block;

unordered_map<int, pair<int,Block*>> file_descriptor; // key is file id, first value holds content size for that file, second value in pair is a pointer to the starting block of the file
int calculate_space_utilized();
int write_file(Block* prev, int file_id);
int create(int file_id);
int read_file(int file_id);
void display();
int delete_file(int file_id);

int write_file(Block* prev, int file_id)
{
    string content;
    cin >> content;
    int content_size = content.length();
    int num_blocks_needed;
    if(prev==NULL)
        num_blocks_needed = ceil(content_size*1.0 / block_size * 1.0);
    else {
        int old_content_size = file_descriptor[file_id].first;
        int num_blocks_given = ceil(old_content_size*1.0 / block_size * 1.0);
        int total_content_size = old_content_size + content_size;
        int total_num_blocks_needed = ceil(total_content_size*1.0 / block_size * 1.0);
        num_blocks_needed = total_num_blocks_needed - num_blocks_given;
    }

    if (num_blocks_needed > free_blocks)
    { // no space avaialable to save the file
        cout << "Sorry could not allocate file with file id " << file_id << " due to space limitaion" << endl;
        return -1;
    }
    free_blocks = free_blocks - num_blocks_needed;
    int i,index = 0;
    if(prev!=NULL) {//check whether last block is empty or not,if empty allocate some data in the last block
        int old_content_size = file_descriptor[file_id].first;
        i = old_content_size%block_size;
        if(i!=0) {//last block has some space
            while(i < block_size && index < content_size)
                prev->data[i++] = content[index++];
        }
    }
    file_descriptor[file_id].first += content_size;

    while (index < content_size)
    {
        Block *curr = new Block;
        
        for (i = 0; i < block_size && index < content_size; i++)
        {
            curr->data[i] = content[index++];
        }

        if (i < block_size)
            cout << "Internal Fragmentation happned for this file with file id " << file_id << endl;

        if(prev==NULL)
            file_descriptor[file_id].second = curr;// for the starting block,storing it for future reference
        else
            prev->ptr_to_next_block = curr;
                
        prev = curr;
    }

    if (prev != NULL)
        prev->ptr_to_next_block = NULL;

    return 0;
}
int create(int file_id)
{
    cout << "\nFile creation initiated for file id " << file_id << endl;
    cout << "---------------------------------------------" << endl;
    if (file_descriptor.find(file_id) != file_descriptor.end())
    { // checking for duplicate file id
        string d;
        cin >> d;
        cout << "Sorry file could not be created due to duplicate file id" << endl;
        return -1;
    }

    return write_file(NULL, file_id);
}

int update(int file_id)
{
    cout << "\nFile updatation initiated for file id " << file_id << endl;
    cout << "---------------------------------------------" << endl;

    if (file_descriptor.find(file_id) == file_descriptor.end())
    { // checking if file is present or not
        string d;
        cin >> d;
        cout << "Sorry file is not present in memory" << endl;
        return -1;
    }

    Block *last_block = file_descriptor[file_id].second;

    while (last_block->ptr_to_next_block)
    {
        last_block = last_block->ptr_to_next_block;
    }
    return write_file(last_block,file_id);
}

void display() {
    cout << "\nDisplaying all files with contents" << endl;

    cout << "\nTotal number of files " << file_descriptor.size() << endl;
    int i;
    for(auto itr : file_descriptor) {
        int fid = itr.first;
        read_file(fid);
    }
}

int read_file(int file_id) {
    cout << "\nFile reading initiated for file id " << file_id << endl;
    cout << "---------------------------------------------" << endl;
    if (file_descriptor.find(file_id) == file_descriptor.end())
    {
        cout << "Sorry file is not present in memory" << endl;
        return -1;
    }
    Block *itr = file_descriptor[file_id].second;
    int file_size = file_descriptor[file_id].first;
    int j = 0;
    while(itr) {
        for(int i=0;i<block_size && j<file_size;i++) {
            cout << itr->data[i];
            j++;
        }
            

        itr = itr->ptr_to_next_block;
    }

    cout << endl;
    return 0;
}


int delete_file(int file_id)
{

    cout << "\nFile deletion initiated for file id " << file_id << endl;
    cout << "---------------------------------------------" << endl;
    auto it = file_descriptor.find(file_id);
    if (it == file_descriptor.end())
    { // checking if file is present or not
        cout << "Sorry file is not present in memory" << endl;
        return -1;
    }
    Block* start_block = file_descriptor[file_id].second;
    while (start_block != NULL)
    {
        Block *temp = start_block->ptr_to_next_block;
        delete(start_block);
        start_block = temp;
        free_blocks++;
    }

    file_descriptor.erase(it); // deleting from file_descriptor
    return 0;
}
int calculate_space_utilized() {
   // cout << endl;
    int used_blocks = total_num_blocks - free_blocks;
    int space_used = used_blocks * sizeof(block_size) + sizeof(file_descriptor);
    cout << "Blocks used is " << used_blocks << endl;
    cout << "Space utilized is " << space_used << " Bytes" << endl;
    //cout << "Space ratio is " << (total_num_blocks - free_blocks)/total_num_blocks << endl;
    printf("%f\n",((float)used_blocks/(float)total_num_blocks));
    return 0;
}

int main()
{
    cout << "My block structure size is " << sizeof(Block) << endl;
    cout << "My block size is " << block_size << " characters." << endl;
    clock_t start, end;
    double cpu_time_used;
    while (true)
    {
        int option;
        cin >> option;
        int fid;
        
        switch (option)
        {
            case 0: // create file
                cin>>fid;
                start = clock();
                create(fid);
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Time elapsed: %f seconds\n", cpu_time_used);
              //  display();
                calculate_space_utilized();
                break;
            case 1:
                //start = clock();
                cin>>fid;
                start = clock();
                read_file(fid);
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Time elapsed: %f seconds\n", cpu_time_used);
                
                break;
            case 2:
                cin>>fid;
                start = clock();
                update(fid);
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Time elapsed: %f seconds\n", cpu_time_used);
              //  display();
                calculate_space_utilized();
                break;
            case 3:
                cin>>fid;
                start = clock();
                delete_file(fid);
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Time elapsed: %f seconds\n", cpu_time_used);
               // display();
                calculate_space_utilized();
                break;
            default:
                return 0;
        }
        
    }
    return 0;
}