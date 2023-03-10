# Encrypt-Decrypt-Application
A C++ Vigenere's cypher based application for encrypting/decrypting any files.

Encryption/Decryption application made by Neleptcu Daniel-Andrei @zuch3e
_____________________________________________________________________________________________________________
\                                                                                                           /
\     This application is a personal project which encrypts/decrypts any type of file.                      /
\                                                                                                           /
\      To use the application you have to do the following:                                                 /
\                1. Open a file you want to encrypt/decrypt using the 'Open File' button.                   /
\                2. Choose a location to save the altered file using the 'Save File' button.                /
\                         2.1. You can choose not to save the file and a file with the same name            /
\                              as the original file will be created in the directory of the original        /
\                              file + the text "Encrypted"/"Decrypted" depending on the operation.          /
\                3. Choose a strong password to encrypt your file with. If you want to decrypt              /
\                   your file after encrypting it, you can only do it by using the same password            /
\                   used for encryption.                                                                    /
\                4. Now you can choose to Encrypt/Decrypt your file by choosing one of the 2                /
\                   buttons displayed.                                                                      /
\                *. If you choose a file larger than 500Mb, you will receive a popup that informs           /
\                   you about the process as it may take longer to complete. If the program is              /
\                  "Not Responding", it is just taking longer due to the file's size.                       /
\___________________________________________________________________________________________________________/


  This application is built in C++ and the UI is made with the WIN32 api.
  The encryption algorithm is inspired from Vigenere's cypher. Vigenere's cypher requires an alphabet and a key to encrypt/decrypt files. Consider an array containing the alphabet of length 'x'. Now, from each element of the array continue the alphabet downwards. After completing with the last letter of the original array you will start over until you reach the length 'x'.  Next, we will label the rows and columns using our alphabet.If we take the array "ABCDEFG" we end up with the following array of arrays ( a matrix ):

     A   B   C   D   E   F   G 
_______________________________
A  | A   B   C   D   E   F   G 
B  | B   C   D   E   F   G   A
C  | C   D   E   F   G   A   B
D  | D   E   F   G   A   B   C
E  | E   F   G   A   B   C   D 
F  | F   G   A   B   C   D   E
G  | G   A   B   C   D   E   F
Next, we will consider the text "CDA" and the key "EGB". Take the first letter, namely "C" and "E". If we look at the intersection of the C column and E row, we will end up with the letter G. Doing the same for the remaining 2 we end up with "GCB". Doing the same process backwards knowing the key we end up with the text.

Inspired by this, I initially started with text files and I considered the whole ASCII table as my alphabet, then incremented every character in my initial text by a character in the password :
Taking the text "ABC" and the password "123". If we add every character together we end up with the encrypted text "rtv" and if we subtract the characters from "123" we end up with the original text.
    
Then i applied this principle to binary files, incrementing their hex code: every hex block has 2 hexadecimal characters meaning 256 values, just as the extended ascii character set, so i applied the same principle, making sure to take modulus where needed and account for negative values.
After testing the algorithm and seeing it work, i went on to look at an efficient way to process this data, as a text file with 'n' characters has n+1 bytes. A text file with just 2Mb has over 2 million characters in it. Having tried storing this in a single string ended up quickly capping the string's capacity to hold characters, not being able to work on files larger than 2.5-3Mb in size. Trying process one character from the original file ended up working but it was an extremely slow process and resourceful one as it was constantly accessing files to read from and write to. I ended up compromising between the speed and the stability of both versions and used a smaller char array with the size of BUFSIZ- which differs from system to system (512/1024/0248...) which proved to be the solution i was looking for.
    
With the algorithm itself being done i built a simple interface to make choosing files/setting the password or repeated running easier.  
