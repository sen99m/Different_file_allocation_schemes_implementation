
# File Allocation Scheme Simulation
This project focuses on the implementation and simulation of different file allocation schemes: contiguous allocation, indexed allocation, linked allocation and modified contiguous allocation. The objective is to compare the space and time utilization of these schemes and analyze their efficiency in managing file storage.

## Team members
Madhurima Sen (224101034)  

Debarpan Jana (224101016)  

Mayukh Das (224101036)  
## References
Please refer to the Assignment-6 pdf Question number 1.
## Run Locally

Clone the project

```bash
  git clone https://github.com/sen99m/Different_file_allocation_schemes_implementation.git
```

Go to the project directory

go to terminal and run below commands.
For compiling use 
```bash
g++ contiguous.cpp -o ca 
g++ modified_contiguous.cpp -o ma
g++ linked_allocation.cpp -o la
g++ indexed_allocation.cpp -o ia
```
for executing type 
```bash
./ca<i2.txt >contiguous_report.txt
./ma<i2.txt >modified_contiguous_report.txt
./la<i1.txt >linked_report.txt
./ia<i1.txt >indexed_report.txt
```


