server 
client ip

check user
list_by_owner       list all the files this user have
list_by_directory   list all files in current directory
list_info                 list a file's info
send                      send a file to server and  add it in directory compressed
remove                 remove a file or directory
create                    create directory
download              upload file uncompressed


make
./server
./client 127.0.0.1


example:
input the user
kkk
input the operation type
send
input the addr and name of file you want send
a.txt
input the addr of file you want add in the directory
root/
  adding: a.txt (stored 0%)
Send Success
input the operation type
list_by_owner
kkk:a.txt,root
input the operation type
create
input the addr of  directory you want to create
root/
input the name of  directory you want to create
dir1
input the operation type
list_by_owner
kkk:a.txt,dir1,root
input the operation type
send
input the addr and name of file you want send
b.txt
input the addr of file you want add in the directory
root/dir1/
  adding: b.txt (stored 0%)
Send Success
input the operation type
list_by_owner
kkk:a.txt,b.txt,dir1,root
input the operation type
list_by_directory
input the addr of directory you want to list
root/dir1/
b.txt,dir1
input the operation type
delete
wrong operation type: Success
input the operation type
remove
input the addr of file or directory you want to remove
root/dir1/
input the name of file or directory you want to remove
b.txt
input the operation type
list_by_owner
kkk:a.txt,dir1,root
input the operation type
download
input the addr of  directory you want to download
root/
input the name of  directory you want to download
a.txt
Receive Success
input the operation type
end

 
