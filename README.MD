# interview code
Author : Reddy Su

## Environment Setting
 - client and server run on same machine, share same file system

## How to compile
 ``` 
 cmake .
 make
 ```
## How to use
 - First run server, key in the number of threads you want.
 - Then execute client and send the target time for which you want to check whether the files han been written.
 - The server would print the word NOT in Alphabetical order

 For Server:
 ```
 ./server 2
 ```

 For Client:
 ```
 ./client 1714649805
 ```
 then you will get server with 2 threads to check the file that has been changed after 1714649805.
 ## Time complexity analysis
 - For server
    - I calculate work effort of threads by the size of valid files. 
    - The time complexity for best solution of work balance is O(n log n)
    - I sort the file order based on filesize first, then assigned the largest file to the thread with least burden orderly. 
    - The thread work balance is gaurantee to be under size(Largest_file) - size(Smallest_file)
    - The english word check cost O(N)
 - For client
    - I write many codes on check whether the file is valid to be calculated.
    - First check the file is regular file or not(e.g. symbolic link)
    - Then check is it ASCII, UTF-8,  ISO-8859-1, or ASCII+Big5.
    - If A regular file is not on above list, I consider it as a binary file, hence is not a candidate.
    - Sent the path of candidates file to server through sockets as json format.
## Detail
    - To prevent the unstability of TCP, the client would send a size_t number representating size of message
    it would send
    - The server would check the file size it received is the same as that size_t number.
## What can be improve
    - remote file reading
        - Not releastic to count. First is safty issue(need to encrypt...), Secand, large file transferring is resource costing. 
        - Therefore, I only setting the environment to be on the same machine.
        - I thought to complete remote file reading, so I wrote sockets in my implementation. But after considering, I quit it.
    - Better testing
        - I only generate very few file for testing cases. Maybe can fetch web data, like wikipedia, as my testing file
