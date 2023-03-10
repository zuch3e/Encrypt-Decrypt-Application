#include <tchar.h>
#include <stdio.h>
#include <fstream>
#include "resource.h"
#include <windows.h>
#include <sys/stat.h>
#include <stdbool.h>
using namespace std;

/* Windows procedure */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

HWND textfield, button;
/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("EncryptDecryptProject");
HWND OpenBox,SaveBox,PassBox;
/* Main parameters where the paths of the files and the password are stored */
class obj
{
private:
    char dest1[500];
    char dest2[500];
    char password[500];
public:
    /*The main function for encryption / decryption */
    void encrypt_decrypt(bool isNotSaved,bool option,HWND hwnd) /* isNotSaved is a parameter to create a path for a saved file automatically */
    /* in case it is not specified, option decides whether to encrypt or decrypt */
    {
        char keyWord1[10],keyWord2[10]; /* 2 char arrays that will be concatenated to the original file's name if it is the case */
        size_t  n; /* memory block */
        int i=0;
        char buff[BUFSIZ]; /* Data block */
        /* swapping keyWord1 and keyWord2 in based on the operation required*/
        if(option)
        {
            strcpy(keyWord1,"Decrypted");
            strcpy(keyWord2,"Encrypted");
        }
        else
        {
            strcpy(keyWord2,"Decrypted");
            strcpy(keyWord1,"Encrypted");
        }

        if (isNotSaved)
        {
            /* copies the original file path to the new file */
            strcpy(dest2,dest1);
            char temp[20];
            strcpy(temp,strrchr(dest2,'.')); /* searches for the extension and copies it into temp */
            if(strstr(dest2+strlen(dest2)-strlen(temp)-9,keyWord1)) /*checks to see if the file already has 'Encrypted'/'Decrypted' at the end of it to avoid confusion */
            {
                strcpy(dest2+strlen(dest2)-strlen(temp)-9,keyWord2);
            }
            else /* In case it does not have 'Encrypted'/'Decrypted' at the end of it, appends the necessary keyWord*/
            {
                strcpy(strrchr(dest2,'.'),keyWord2);
            }
            strcat(dest2,temp); /* Appends the extension */
        }

        FILE    *in, *out; /* Using C file pointers because they are faster than fstream*/
        in = fopen( dest1, "rb" );
        out= fopen( dest2, "wb" );
        struct stat st;
        stat(dest1, &st); /* Check the original file's size using the stat() function and store it in the fileSize variable */
        off_t fileSize = st.st_size;

        if(fileSize>524288000) /* check if the file is larger than 500mb and give a notification to the user that it might take longer */
        {
            MessageBox(hwnd,"Large file, please wait..","Info",MB_OK);
        }
        /* the encryption/decryption algorithm */

        if(option)
        {
            while ( (n=fread(buff,1,BUFSIZ,in)) != 0 ) /* readig blocks of data the length of BUFSIZ into buff for faster execution time */
            {
                for(i=0; i<BUFSIZ; i++)
                {
                    buff[i]=(buff[i]+password[i%strlen(password)])%256; /* apply the algorithm to every character in the buff */
                }
                fwrite( buff, 1, n, out ); /* putting the blocks of altered data into the new file*/
            }
        }
        else
        {
            while ( (n=fread(buff,1,BUFSIZ,in)) != 0 )
            {
                for(i=0; i<BUFSIZ; i++) /* readig blocks of data the length of BUFSIZ into buff for faster execution time */
                {
                    if(buff[i]-password[i]<0)
                    {
                        buff[i]=(buff[i]-password[i%strlen(password)])+256;
                    }
                    else                                                    /* apply the algorithm to every character in the buff */
                    {
                        buff[i]=(buff[i]-password[i%strlen(password)]);
                    }
                }
                fwrite( buff, 1, n, out );/* putting the blocks of altered data into the new file*/
            }
        }

        /* closing the files*/
        fclose(in);
        fclose(out);
    }
    bool checkfile()
    {
        struct stat buffer;
        return (stat (dest1, &buffer) == 0);
    }
    /* This function builds the "Open File" Window and specifies the extensions allowed" */
    /* The open_file's path will be stored in filename and copied to dest1*/
    void open_file(HWND hwnd)
    {
        char filename[500];
        OPENFILENAME ofn;
        ZeroMemory(&ofn,sizeof(OPENFILENAME));
        ofn.lStructSize=sizeof(OPENFILENAME);
        ofn.hwndOwner=hwnd;
        ofn.lpstrFile=filename;
        ofn.lpstrFile[0]='\0';
        ofn.nMaxFile=500;
        ofn.lpstrFilter="All files\0*.*\0";
        ofn.nFilterIndex=1;
        GetOpenFileName(&ofn);
        strcpy(dest1,filename);
    }

    /* This function builds the "Save File" Window and specifies the extensions allowed" */
    /* The save_file's path will be stored in filename and copied to dest2*/
    void save_file(HWND hwnd)
    {
        char filename[500];
        OPENFILENAME ofn;
        ZeroMemory(&ofn,sizeof(OPENFILENAME));
        ofn.lStructSize=sizeof(OPENFILENAME);
        ofn.hwndOwner=hwnd;
        ofn.lpstrFile=filename;
        ofn.lpstrFile[0]='\0';
        ofn.nMaxFile=500;
        ofn.lpstrFilter="All files\0*.*\0";
        ofn.nFilterIndex=1;
        GetSaveFileName(&ofn);
        strcpy(dest2,filename);
    }
    void warningmessages(HWND hwnd,bool parameter)
    {
        char user_message1[]="Encryption successful.";
        char user_message2[]="Encryption successful. File saved automatically in the same directory.";
        if (!parameter)
        {
            user_message1[0]=user_message2[0]='D';
            user_message1[1]=user_message2[1]='e';
        }
        if (strlen(dest1)) /* Checks if a file was selected */
        {
            if(checkfile())
            {
                if (password[0]!='\0') /* checks if the password is empty */
                {
                    if(dest2[0]!='\0') /* if there is no save path specified, call the encryption/decryption function and give info to the user */
                    {
                        encrypt_decrypt(false,parameter,hwnd);
                        MessageBox(hwnd,user_message1,"Info",MB_OK);
                    }
                    else
                    {
                        encrypt_decrypt(true,parameter,hwnd);
                        MessageBox(hwnd,user_message2,"Warning",MB_OK);
                    }
                }
                else
                {
                    MessageBox(hwnd,"Please set a password","Error",MB_OK);
                }
            }
            else
            {
                MessageBox(hwnd,"File does not exist","Error",MB_OK);
            }

        }
        else
        {
            MessageBox(hwnd,"Please open a file.","Error",MB_OK);
        }
    }
    void init1(HWND hwnd)
    {
        OpenBox= CreateWindow("EDIT",dest1,WS_CHILD|WS_VISIBLE|WS_HSCROLL|ES_AUTOHSCROLL|ES_MULTILINE,20,30,520,40,hwnd,NULL,NULL,NULL); /* Replaces the empty OpenBox with the path registered */
    }
    void init2(HWND hwnd)
    {
        SaveBox= CreateWindow("EDIT",dest2,WS_CHILD|WS_VISIBLE|WS_HSCROLL|ES_AUTOHSCROLL|ES_MULTILINE,20,90,520,40,hwnd,NULL,NULL,NULL); /* Replaces the empty SaveBox with the path registered */
    }
    void init3(HWND hwnd)
    {
        GetWindowText(OpenBox,dest1,500); /* Saves the password from the OpenBox in the dest1 variable */    //for when text is introduced manually
        GetWindowText(SaveBox,dest2,500); /* Saves the password from the SaveBox in the dest2 variable */
        GetWindowText(PassBox,password,500); /* Saves the password from the PassBox in the password variable */
    }
} parameters;

int WINAPI WinMain (HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszArgument,int nCmdShow)
{
    HWND hwnd;               /* Handle for the window */
    MSG messages;            /* Where the messages are saved */
    WNDCLASSEX wincl;        /* Data structure for the window class */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* Windows called function */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL,IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (hThisInstance, MAKEINTRESOURCEA(MYICON1));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Default windows color as background color */
    wincl.hbrBackground = CreateSolidBrush(RGB(152, 228, 245));

    /* Quit the program if registering the window class fails */
    if (!RegisterClassEx (&wincl))
        return 0;

    hwnd = CreateWindowEx (
               0,
               szClassName,         /* Classname */
               _T("Encrypt/Decrypt @ Andrei Neleptcu"),       /* Title Text */
               WS_SYSMENU | WS_MINIMIZEBOX, /* Unsizable and minimizable window */
               CW_USEDEFAULT,       /* Default position */
               CW_USEDEFAULT,
               680,                 /* width and*/
               360,                 /* height in pixels */
               HWND_DESKTOP,        /* Consider the desktop the parent window and this program the child window */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

void createdoc()
{
    std::fstream doc("readme.txt", doc.out);
    doc<<"Encryption/Decryption application made by Neleptcu Daniel-Andrei @zuch3e"<<endl<<
"_________________________________________________________________________________"<<endl<<
"\\                                                                                                                                                               /"<<endl<<
"\\  This application is a personal project which encrypts/decrypts any type of file.                      /"<<endl<<
"\\                                                                                                                                                               /"<<endl<<
"\\  To use the application you have to do the following:                                                                   /"<<endl<<
"\\                  1. Open a file you want to encrypt/decrypt using the 'Open File' button.                 /"<<endl<<
"\\                  2. Choose a location to save the altered file using the 'Save File' button.                  /"<<endl<<
"\\                           2.1. You can choose not to save the file and a file with the same name           /"<<endl<<
"\\                                   as the original file will be created in the directory of the original              /"<<endl<<
"\\                                   file + the text ""Encrypted""/""Decrypted"" depending on the operation.             /"<<endl<<
"\\                   3. Choose a strong password to encrypt your file with. If you want to decrypt      /"<<endl<<
"\\                        your file after encrypting it, you can only do it by using the same password      /"<<endl<<
"\\                        used for encryption.                                                                                                    /"<<endl<<
"\\                   4. Now you can choose to Encrypt/Decrypt your file by choosing one of the 2       /"<<endl<<
"\\                        buttons displayed.                                                                                                        /"<<endl<<
"\\                   *. If you choose a file larger than 500Mb, you will receive a popup that informs    /"<<endl<<
"\\                       you about the process as it may take longer to complete. If the program is        /"<<endl<<
"\\                       ""Not Responding"", it is just taking longer due to the file's size.                                 /"<<endl<<
"\\________________________________________________________________________________/";

    system("start notepad readme.txt");

}
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
case WM_CTLCOLORSTATIC:
    {
        SetTextColor((HDC)wParam, RGB(59, 61, 61));
        SetBkMode((HDC)wParam, TRANSPARENT);
        return GetClassLongPtr(hwnd,GCLP_HBRBACKGROUND);
    }break;
    case WM_CTLCOLORBTN:
    {
        SetBkColor((HDC)wParam, RGB(59, 61, 61));
        SetBkMode((HDC)wParam, TRANSPARENT);
        return GetClassLongPtr(hwnd,GCLP_HBRBACKGROUND);
    }break;
    case WM_CREATE:
        OpenBox= CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|WS_HSCROLL|ES_AUTOHSCROLL|ES_MULTILINE,20,30,520,40,hwnd,NULL,NULL,NULL); /* OpenBox path message */
        SaveBox= CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|WS_HSCROLL|ES_AUTOHSCROLL|ES_MULTILINE,20,90,520,40,hwnd,NULL,NULL,NULL); /* SaveBox path message */
        PassBox= CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL|ES_PASSWORD,20,170,640,20,hwnd,NULL,NULL,NULL); /* Password Box path message */

        CreateWindow("STATIC","Password",WS_CHILD|WS_VISIBLE|ES_CENTER,18,150,88,20,hwnd,NULL,NULL,NULL); /* Password Lbel message */
        CreateWindow("BUTTON","Open File",WS_VISIBLE|WS_CHILD|WS_BORDER,560,30,100,40,hwnd, (HMENU) 1, NULL, NULL); /* OpenBox button */
        CreateWindow("BUTTON","Save File",WS_VISIBLE|WS_CHILD|WS_BORDER,560,90,100,40,hwnd, (HMENU) 2, NULL, NULL); /* SaveBox button */

        CreateWindow("BUTTON","Encrypt",WS_VISIBLE|WS_CHILD|WS_BORDER,160,220,140,80,hwnd, (HMENU) 3, NULL, NULL); /* Encrypt button */
        CreateWindow("BUTTON","Decrypt",WS_VISIBLE|WS_CHILD|WS_BORDER,380,220,140,80,hwnd, (HMENU) 4, NULL, NULL); /* Decrypt button */
        CreateWindow("BUTTON","?",WS_VISIBLE|WS_CHILD|WS_BORDER,650,305,20,20,hwnd, (HMENU) 5, NULL, NULL); /* Documentation button */

        break;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case 1:
        {
            parameters.open_file(hwnd); /* Calls the function that saves the path of the file to be opened in dest1 */
            DestroyWindow(OpenBox); /*Destroys the initial window and creates one with the selected path*/
            parameters.init1(hwnd); /* */
            break;
        }

        case 2:
        {
            parameters.save_file(hwnd); /* Calls the function that saves the path of the file to be saved in dest2*/
            DestroyWindow(SaveBox); /*Destroys the initial window and creates one with the selected path*/
            parameters.init2(hwnd);
            break;
        }
        case 3:
        {

            parameters.init3(hwnd);
            parameters.warningmessages(hwnd,true);
            break;
        }
        case 4:
        {
            parameters.init3(hwnd);
            parameters.warningmessages(hwnd,false);
            break;
        }
        case 5:
        {
            createdoc();
            break;
        }
        }
        break;
    }
    case WM_DESTROY:
        DeleteObject((HBRUSH)GetClassLong(hwnd,GCLP_HBRBACKGROUND));
        PostQuitMessage (0);       /* exit the program by pressing the X button */
        break;
    default:                      /* other messages */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
