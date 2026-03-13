# CVFS – Custom Virtual File System

## Overview
**CVFS (Custom Virtual File System)** is a simplified implementation of a UNIX-like file system developed using the C programming language. The project simulates the internal working of operating system file management by implementing core file system components and system call behavior in user space.

The system models important file system structures such as **Boot Block, Super Block, Inodes, File Table, and User File Descriptor Table (UFDT)**. These components work together to manage file metadata, maintain file descriptors, and perform file operations within the virtual environment.

CVFS provides an **interactive command-line shell** where users can perform file system operations similar to UNIX commands such as creating files, writing data, reading data, listing files, and deleting files. The system also includes a built-in **manual page system (`man`)** that displays command descriptions and usage information.

This project is designed for **educational purposes** to demonstrate how operating systems internally manage files and file descriptors.

---

## Features
- Virtual file system implemented in **C**
- Simulation of **inode-based file management**
- Implementation of **UFDT (User File Descriptor Table)**
- **Superblock and Boot block management**
- Interactive **command-line shell**
- Built-in **manual page system**
- File permission handling
- Basic file operations (create, read, write, delete)

---

## System Architecture

User Command Shell  
↓  
Command Parser  
↓  
User File Descriptor Table (UFDT)  
↓  
File Table  
↓  
Inode List (DILB)  
↓  
Super Block  
↓  
Boot Block  

---

## Supported Commands

| Command | Description |
|--------|-------------|
| creat filename permission | Create a new file |
| write fd | Write data into file |
| read fd size | Read data from file |
| ls | List all files |
| unlink filename | Delete file |
| man command | Display manual page |
| help | Show help page |
| clear | Clear terminal |
| exit | Terminate CVFS |

---

## Example Execution

### Creating File
```
CVFS : > creat Demo.txt 3
File gets successfully created with FD 3
```  

### Writing Data
```
CVFS : > write 3
Enter the data that you want to write :
Welcome to CVFS
```

### Reading Files
```
CVFS : > read 3 15
Data from file is : Welcome To CVFS
```

### Listing Files
```
CVFS : > ls<br/>
1 Demo.txt 15<br/>
```

### Deleting File
```
CVFS : > unlink Demo.txt<br/>
File gets successfully deleted<br/>
```


---

## Technologies Used
- **Programming Language:** C
- **Platform:** Linux / UNIX
- **Concepts:** Operating Systems, File Systems, Inodes, File Descriptors, Memory Management

---

## Future Improvements
- Implementation of `open()` system call  
- Implementation of `close()` system call  
- File seek functionality (`lseek`)  
- Directory management support  
- Persistent storage support  

---

## Author
**Pratik Babaso Chougule**
