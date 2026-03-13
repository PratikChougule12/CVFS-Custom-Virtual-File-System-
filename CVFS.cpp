//////////////////////////////////////////////////////////
//
//  Header File Inclusion
//
//////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>

//////////////////////////////////////////////////////////
//
//  User Defined Macros
//
//////////////////////////////////////////////////////////

// Maximum file size that we allow in the project
#define MAXFILESIZE 50

#define MAXOPENFILES 20

#define MAXINODE 5

#define READ 1
#define WRITE 2
#define EXECUTE 4

#define START 0
#define CURRENT 1
#define END 2

#define EXECUTE_SUCCESS 0

#define REGULARFILE 1
#define SPECIALFILE 2

//////////////////////////////////////////////////////////
//
//  User Defined Macros for error handling
//
//////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

#define ERR_MAX_FILES_OPEN -8

//////////////////////////////////////////////////////////
//
//  User Defined Structures
//
//////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////
//
//  Structure Name :    BootBlock
//  Description :       Holds the information to boot the OS
//
//////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

//////////////////////////////////////////////////////////
//
//  Structure Name :    SuperBlock
//  Description :       Holds the information about the file system
//
//////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

//////////////////////////////////////////////////////////
//
//  Structure Name :    Inode
//  Description :       Holds the information about file
//
//////////////////////////////////////////////////////////

#pragma pack(1)
struct Inode
{
    char FileName[20];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int Permission;
    char *Buffer;
    struct Inode *next;
};

typedef struct Inode INODE;
typedef struct Inode * PINODE;
typedef struct Inode ** PPINODE;

//////////////////////////////////////////////////////////
//
//  Structure Name :    FileTable
//  Description :       Holds the information about opened file
//
//////////////////////////////////////////////////////////

struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    PINODE ptrinode;
};

typedef FileTable FILETABLE;
typedef FileTable * PFILETABLE;

//////////////////////////////////////////////////////////
//
//  Structure Name :    UAREA
//  Description :       Holds the information about process file
//
//////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];
};

//////////////////////////////////////////////////////////
//
//  Global variables or objects used in the project
//
//////////////////////////////////////////////////////////

BootBlock bootobj;
SuperBlock superobj;
UAREA uareaobj;

PINODE head = NULL;

//////////////////////////////////////////////////////////
//
//  Function Name :     InitialiseUAREA
//  Description :       It is used to initialise UAREA members
//  Author :            Pratik Babaso Chougule
//  Date :              13/01/2026
//
//////////////////////////////////////////////////////////

void InitialiseUAREA()
{
   strcpy(uareaobj.ProcessName,"Myexe");
   
   int i = 0;

   for(i = 0; i < MAXOPENFILES; i++)
   {
        uareaobj.UFDT[i] = NULL;
   }
    printf("CVFS : UAREA gets initialised succesfully\n");
}

//////////////////////////////////////////////////////////
//
//  Function Name :     InitialiseSuperBlock
//  Description :       It is used to initialise Super block members
//  Author :            Pratik Babaso Chougule
//  Date :              13/01/2026
//
//////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    printf("CVFS : Super block gets initialised succesfully\n");
}

//////////////////////////////////////////////////////////
//
//  Function Name :     CreateDILB
//  Description :       It is used to create Linkedlist of inodes
//  Author :            Pratik Babaso Chougule
//  Date :              13/01/2026
//
//////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    for(i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        strcpy(newn->FileName,"\0");
        newn->InodeNumber = i;
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->FileType = 0;
        newn->ReferenceCount = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if(temp == NULL)    // LL is empty
        {
            head = newn;
            temp = head;
        }
        else                // LL contains atleast 1 node
        {
            temp->next = newn;
            temp = temp->next;
        }
    }

    printf("CVFS : DILB created succesfully\n");
}

//////////////////////////////////////////////////////////
//
//  Function Name :     StartAuxillaryDataInitilisation
//  Description :       It is used to call all such functions which are
//                      used to initialise auxillary data
//  Author :            Pratik Babaso Chougule
//  Date :              13/01/2026
//
//////////////////////////////////////////////////////////

void StartAuxillaryDataInitilisation()
{
    strcpy(bootobj.Information,"Booting process of CVFS is done");

    printf("%s\n",bootobj.Information);

    InitialiseSuperBlock();

    CreateDILB();

    InitialiseUAREA();

    printf("CVFS : Auxillary data initialised succesfully\n");
}

//////////////////////////////////////////////////////////
//
//  Function Name :     DisplayHelp
//  Description :       It is used to display the help page
//  Author :            Pratik Babaso Chougule
//  Date :              14/01/2026
//
//////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("-----------------------------------------------\n");
    printf("--------------- CVFS Help Page ----------------\n");
    printf("-----------------------------------------------\n");

    printf("man    : It is used to display manual page\n");
    printf("clear  : It is used to clear the terminal\n");
    printf("creat  : It is used to create new file\n");
    printf("write  : It is used to write the data into file\n");
    printf("read   : It is used to read the data from the file\n");
    printf("stat   : It is used to display statistical information\n");
    printf("unlink : It is used to delete the file\n");
    printf("exit   : It is used to terminate CVFS\n");
    printf("ls     : It is used to display all the name of files from direcotory \n");

    printf("-----------------------------------------------\n");

}

//////////////////////////////////////////////////////////
//
//  Function Name :     ManPageDisplay
//  Description :       It is used to display man page
//  Author :            Pratik Babaso Chougule
//  Date :              14/01/2026
//
//////////////////////////////////////////////////////////

void ManPageDisplay(char Name[])
{
    if(strcmp("ls",Name) == 0)
    {
        printf("About : List information about the FILEs (the current directory by\n");
        printf("        default).  Sort entries alphabetically if none of -cftuvSUX nor\n");
        printf("        --sort is specified.\n");
        printf("        \n");
        printf("        \n");
        printf("Usage : ls\n");
        printf("        \n");
        printf("        \n");
        printf("Exit status:\n");
        printf("        0      if OK,\n");
        printf("        1      if minor problems (e.g., cannot access subdirectory),\n");
        printf("        2      if serious trouble (e.g., cannot access command-line argument).\n");
    }
    else if(strcmp("man",Name) == 0)
    {
        printf("About : man is the system's manual pager.  Each page argument given to man\n");
        printf("        is normally the name of a program, utility or function.  The\n");
        printf("        manual page associated with each of these arguments is then found\n");
        printf("        and displayed.  A section, if provided, will direct man to look\n");
        printf("        only in that section of the manual.  The default action is to\n");
        printf("        search in all of the available sections following a pre-defined\n");
        printf("        order (see DEFAULTS), and to show only the first page found, even\n");
        printf("        if page exists in several sections.e\n");
        printf("        \n");
        printf("        \n");
        printf("Usage : man command_name(It is the name of command)\n");
        printf("        \n");
        printf("        \n");
        printf("EXIT STATUS: \n");        
        printf("        0      Successful program execution.\n");
        printf("        1      Usage, syntax or configuration file error.\n");
        printf("        2      Operational error\n");
        printf("        3      A child process returned a non-zero exit status.\n");
        printf("        16     At least one of the pages/files/keywords didn't exist or wasn't matched. : \n");

    }
    else if(strcmp("exit",Name) == 0)
    {
        printf("About : The exit utility shall cause the shell to exit from its current\n");
        printf("        execution environment with the exit status specified by the\n");
        printf("        unsigned decimal integer n.  If the current execution environment\n");
        printf("        is a subshell environment, the shell shall exit from the subshell\n");
        printf("        environment with the specified exit status and continue in the\n");
        printf("        environment from which that subshell environment was invoked;\n");
        printf("        otherwise, the shell utility shall terminate with the specified\n");
        printf("        exit status. If n is specified, but its value is not between 0 and\n");
        printf("        255 inclusively, the exit status is undefined.\n");
        printf("        \n");
        printf("        \n");
        printf("        A trap on EXIT shall be executed before the shell terminates,\n");
        printf("        except when the exit utility is invoked in that trap itself, in\n");
        printf("        which case the shell shall exit immediately.\n");
        printf("        \n");
        printf("        \n");
        printf("Usage : exit\n");
        printf("        \n");
        printf("        \n");       
        printf("EXIT STATUS: \n");        
        printf("        The exit status shall be n, if specified, except that the behavior\n");
        printf("        is unspecified if n is not an unsigned decimal integer or is\n");
        printf("        greater than 255. Otherwise, the value shall be the exit value of\n");
        printf("        the last command executed, or zero if no command was executed..\n");
        printf("        When exit is executed in a trap action, the last command is\n");
        printf("        considered to be the command that executed immediately preceding\n");
        printf("        the trap action.\n");
    }
    else if(strcmp("clear",Name) == 0)
    {
        printf("About : clear clears your terminal's screen and its scrollback buffer, if\n");
        printf("        any.  clear retrieves the terminal type from the environment\n");
        printf("        variable TERM, then consults the terminfo terminal capability\n");
        printf("        database entry for that type to determine how to perform these\n");
        printf("        actions.\n");
        printf("        \n");
        printf("        \n");
        printf("        The capabilities to clear the screen and scrollback buffer are\n");
        printf("        named “clear” and “E3”, respectively.  The latter is a user-\n");
        printf("        defined capability, applying an extension mechanism introduced in\n");
        printf("        ncurses 5.0 (1999).\n");
        printf("        \n");
        printf("        \n");
        printf("Usage : clear\n");
    }
    else if(strcmp("creat",Name) == 0)
    {
        printf("About : The creat() system call creates a new file or rewrites an existing one.\n");
        printf("         It is effectively a specialized version of the open() system call for write-only operations.\n");
        printf("        \n");
        printf("        \n");
        printf("Usage : creat filename 3(permission)\n");
        printf("        \n");
        printf("        \n");
        printf("RETURN VALUE :");
        printf("         On Success: Returns a new file descriptor, which is a non-negative integer.\n");
        printf("         This descriptor will be the lowest-numbered file descriptor not currently open for the process.\n");
        printf("        \n");
        printf("         On Error: Returns -1. Additionally, the global variable errno is set to a specific positive value indicating the error.\n");
        
    }
    else if(strcmp("write",Name) == 0)
    {
        printf("About : write() is used to write data into a file. It accepts a file descriptor\n");
        printf("        from the user and the number of bytes that the user wants to write.\n");
        printf("        The function then writes the specified data into the file.\n");
        printf("        \n");
        printf("        \n");
        printf("Usage : write 2(file Descriptor) \n");
        printf("        \n");
        printf("        \n");
        printf("RETURN VALUE :");
        printf("         On success, the number of bytes  successfuly read is returned\n");
        printf("         On Error, -1 is returned, and errno is set to indicate the error.\n");

    }
    else if(strcmp("read",Name) == 0)
    {
       printf("About : read() is used to read data from a file.\n");
       printf("        It accepts a file descriptor, a buffer to store the data,\n");
       printf("        and the number of bytes to be read from the file.\n");
       printf("        The function reads the specified data and updates the\n");
       printf("        read offset accordingly.\n");
       printf("        \n");
       printf("        \n");
       printf("Usage : read 3(File Descriptor) 10(number of bytes)\n");
       printf("        \n");
       printf("        \n");
       printf("RETURN VALUE :");
       printf("         On success, the number of bytes written is returned\n");
       printf("         On Error, -1 is returned, and errno is set to indicate the error.\n");        
    }
    else if(strcmp("stat",Name) == 0)
    {
        printf("About : Display file or file system status\n");
        printf("Usage : clear\n");        
    }
    else if(strcmp("unlink",Name) == 0)
    {
        printf("unlink() deletes a name from the filesystem. If that name was the\n");
        printf("        last link to a file and no processes have the file open,\n");
        printf("        the file is deleted and the space it was using is made\n");
        printf("        available for reuse.\n");
        printf("        \n");
        printf("        \n");
        printf("Usage : unlink Demo.txt\n");
        printf("        \n");
        printf("        \n");
        printf("     VALUE :");
        printf("         On success, zero is returned\n");
        printf("         On Error, -1 is returned, and errno is set to indicate the error.\n");        
    }
    else
    {
        printf("No manual entry for %s\n",Name);
    }
}

//////////////////////////////////////////////////////////
//
//  Function Name :     IsFileExist
//  Description :       It is used to check whether file is already exist or not
//  Input :             It accepts file name
//  Output :            It returns the true or false
//  Author :            Pratik Babaso Chougule
//  Date :              16/01/2026
//
//////////////////////////////////////////////////////////

bool IsFileExist(
                    char *name      // File name
                )
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if((strcmp(name,temp->FileName) == 0) && (temp->FileType == REGULARFILE))
        {
            bFlag = true;
            break;
        }
        temp = temp->next;
    }
    
    return bFlag;
}

//////////////////////////////////////////////////////////
//
//  Function Name :     CreateFile
//  Description :       It is used to create new regular file
//  Input :             It accepts file name and permissions
//  Output :            It returns the file descriptor
//  Author :            Pratik Babaso Chougule
//  Date :              16/01/2026
//
//////////////////////////////////////////////////////////

int CreateFile(
                    char *name,         // Name of new file
                    int permission      // Permission for that file
                )
{
    PINODE temp = head;
    int i = 0;

    printf("Total number of Inodes remaining : %d\n",superobj.FreeInodes);

    // If name is missing
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // If the permission value is wrong
    // permission -> 1 -> READ
    // permission -> 2 -> WRITE
    // permission -> 3 -> READ + WRITE
    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    // If the inodes are full
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    // If file is already present
    if(IsFileExist(name) == true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }

    // Search empty Inode
    while(temp != NULL)
    {
        if(temp -> FileType == 0)
        {
            break;    
        }
        temp = temp -> next;
    }
    
    if(temp == NULL)
    {
        printf("There is no inode\n");
        return ERR_NO_INODES;
    }

    // Search for empty UFDT entry
    // Note : 0,1,2 are reserved
    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    // UFDT is full
    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate ememory for file table
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // Initialise File table
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode = permission;
    
    // Connect File table with Inode
    uareaobj.UFDT[i]->ptrinode = temp;

    // Initialise elements of Inode
    strcpy(uareaobj.UFDT[i]->ptrinode->FileName,name);
    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE;
    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE;
    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;
    uareaobj.UFDT[i]->ptrinode->Permission = permission;

    // Allocate ememory for files data
    uareaobj.UFDT[i]->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);

    superobj.FreeInodes--;

    return i;   // File descriptor
}

//////////////////////////////////////////////////////////
//
//  Function Name :     LsFile()
//  Description :       It is used to list all files
//  Input :             Nothing
//  Output :            Nothing
//  Author :            Pratik Babaso Chougule
//  Date :              16/01/2026
//
//////////////////////////////////////////////////////////

// ls -l
void LsFile()
{
    PINODE temp = head;

    printf("-----------------------------------------------\n");
    printf("------------ CVFS Files Information -----------\n");
    printf("-----------------------------------------------\n");

    while(temp != NULL)
    {
        if(temp -> FileType != 0)
        {
            printf("%d\t%s\t%d\n",temp->InodeNumber,temp->FileName,temp->ActualFileSize);
        }
        
        temp = temp -> next;
    }
    
    printf("-----------------------------------------------\n");

}

//////////////////////////////////////////////////////////
//
//  Function Name :     UnlinkFile()
//  Description :       It is used to delete the file
//  Input :             File name
//  Output :            Nothing
//  Author :            Pratik Babaso Chougule
//  Date :              22/01/2026
//
//////////////////////////////////////////////////////////

int UnlinkFile(
                    char *name
              )
{
    int i = 0;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Travel the UFDT
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName, name) == 0)
            {
                // Deallocate memory of Buffer
                free(uareaobj.UFDT[i]->ptrinode->Buffer);
                uareaobj.UFDT[i]->ptrinode->Buffer = NULL;

                // Reset all values of inode
                // Dont deallocate memmory of inode
                uareaobj.UFDT[i]->ptrinode->FileSize = 0;
                uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
                uareaobj.UFDT[i]->ptrinode->FileType = 0;
                uareaobj.UFDT[i]->ptrinode->ReferenceCount = 0;
                uareaobj.UFDT[i]->ptrinode->Permission = 0;

                memset(uareaobj.UFDT[i]->ptrinode->FileName, '\0', sizeof(uareaobj.UFDT[i]->ptrinode->FileName));

                // Dealloacte memory of file table
                free(uareaobj.UFDT[i]);

                // Set NULL to UFDT
                uareaobj.UFDT[i] = NULL;

                // // Increment free inodes count
                superobj.FreeInodes++;

                break;  // IMP
            }   // End of if
        }       // End of if
    }           // End of for

    return EXECUTE_SUCCESS;

}               // End of function

//////////////////////////////////////////////////////////
//
//  Function Name :     WriteFile()
//  Description :       It is used to write the data into the file
//  Input :             File descriptor
//                      Address of buffer which contains data
//                      Size of data that we want to write
//  Output :            Number of bytes succesfully written
//  Author :            Pratik Babaso Chougule
//  Date :              22/01/2026
//
//////////////////////////////////////////////////////////

int WriteFile(
                    int fd,
                    char *data,
                    int size
            )
{
    printf("File descriptor : %d\n",fd);
    printf("Data that we want to write : %s\n",data);
    printf("Number of bytes that we want to write : %d\n",size);

    // Invalid FD
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // FD points to NULL
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // There is no permission to write
    if(uareaobj.UFDT[fd]->ptrinode->Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insufficient space
    if((MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // Write the data into the file
    strncpy(uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->WriteOffset, data, size);

    // Update the write offset
    uareaobj.UFDT[fd]->WriteOffset = uareaobj.UFDT[fd]->WriteOffset + size;

    // Update the actual file size
    uareaobj.UFDT[fd]->ptrinode->ActualFileSize = uareaobj.UFDT[fd]->ptrinode->ActualFileSize + size;

    return size;
}

//////////////////////////////////////////////////////////
//
//  Function Name :     ReadFile()
//  Description :       It is used to read the data from the file
//  Input :             File descriptor
//                      Address of empty buffer
//                      Size of data that we want to read
//  Output :            Number of bytes succesfully read
//  Author :            Pratik Babaso Chougule
//  Date :              22/01/2026
//
//////////////////////////////////////////////////////////

int ReadFile(
                int fd,
                char *data,
                int size
            )
{

    //  Invaid FD
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(data == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(size <= 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Filter for permission
    if(uareaobj.UFDT[fd]->ptrinode->Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insuuficeint data
    if((MAXFILESIZE - uareaobj.UFDT[fd]->ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    // Read the data
    strncpy(data,uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->ReadOffset, size);

    // Update the read offset
    uareaobj.UFDT[fd]->ReadOffset = uareaobj.UFDT[fd]->ReadOffset + size;

    return size;
}

//////////////////////////////////////////////////////////
//
//  Entry Point function of the project
//
//////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    char Command[5][20] = {{'\0'}};
    char InputBuffer[MAXFILESIZE] = {'\0'};

    char *EmptyBuffer = NULL;

    int iCount = 0;
    int iRet = 0;

    StartAuxillaryDataInitilisation();

    printf("-----------------------------------------------\n");
    printf("------------ CVFS Files Information -----------\n");
    printf("-----------------------------------------------\n");
    
    // Infinite Listening Shell
    while(1)
    {
        fflush(stdin);

        strcpy(str,"");

        printf("\nCVFS : > ");
        fgets(str,sizeof(str),stdin);
        
        iCount = sscanf(str,"%s %s %s %s %s",Command[0],Command[1],Command[2],Command[3], Command[4]);

        fflush(stdin);

        if(iCount == 1)
        {
            // CVFS : > exit
            if(strcmp("exit",Command[0]) == 0)
            {
                printf("Thank you for using CVFS\n");
                printf("Deallocating all the allocated resources\n");

                break;
            }
            // CVFS : > ls
            else if(strcmp("ls",Command[0]) == 0)
            {
                LsFile();
            }
            // CVFS : > help
            else if(strcmp("help",Command[0]) == 0)
            {
                DisplayHelp();
            }
            // CVFS : > clear
            else if(strcmp("clear",Command[0]) == 0)
            {
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
            }
            else
            {
                printf("Command not found\n");
            }
        } // End of else if 1
        else if(iCount == 2)
        {
            // CVFS : > man ls
            if(strcmp("man",Command[0]) == 0)
            {
                ManPageDisplay(Command[1]);
            }
            // CVFS : > unlink Demo.txt
            else if(strcmp("unlink",Command[0]) == 0)
            {
                iRet = UnlinkFile(Command[1]);
            
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameter\n");
                }

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : Unable to delete as there is no such file");
                }

                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File gets succesfully deleted\n");
                }
            }
            // CVFS : > write 2
            else if(strcmp("write",Command[0]) == 0)
            {
                printf("Enter the data that you want to write : \n");
                fgets(InputBuffer,MAXFILESIZE,stdin);

                iRet = WriteFile(atoi(Command[1]), InputBuffer, strlen(InputBuffer)-1);
            
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters \n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : There is no such file\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Unable to write as there is no permission\n");
                }
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error : Unable to write as there is no space\n");
                }
                else
                {
                    printf("%d bytes gets succesfully written\n",iRet);
                }
            }
            else
            {
                printf("There is no such command\n");
                printf("Please refer help option to get more information\n");
            }
        } // End of else if 2
        else if(iCount == 3)
        {
            // CVFS : > creat Ganesh.txt 3
            if(strcmp("creat",Command[0]) == 0)
            {
                iRet = CreateFile(Command[1],atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Unable to create the file as parameters are invalid\n");
                    printf("Please refer man page\n");
                }

                if(iRet == ERR_NO_INODES)
                {
                    printf("Error : Unable to create file as there is no inode\n");
                }

                if(iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : Unable to create file because the file is already present\n");
                }

                if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Unable to create file\n");
                    printf("Max opened files limit reached\n");
                }

                printf("File gets succesfully created with FD %d\n",iRet);
            } 
            // CVFS : > read 3 10
            if(strcmp("read",Command[0]) == 0)
            {
                EmptyBuffer = (char *)malloc(sizeof(atoi(Command[2])));

                iRet = ReadFile(atoi(Command[1]), EmptyBuffer, atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameter\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File not exist\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Permission denied\n");
                }
                else if(iRet == ERR_INSUFFICIENT_DATA)
                {
                    printf("Error : Insufficient data\n");
                }
                else
                {
                    printf("Read operation is succesful\n");
                    printf("Data from file is : %s\n",EmptyBuffer);

                    free(EmptyBuffer);
                }
            }
            else
            {
                printf("There is no such command\n");
                printf("Please refer help option to get more information\n");
            }
        } // End of else if 3
        else if(iCount == 4)
        {

        } // End of else if 4
        else
        {
            printf("Command not found\n");
            printf("Please refer help option to get more information\n");
        } // End of else
    } // End of while

    return 0;
} // End of main