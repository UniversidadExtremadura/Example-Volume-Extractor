# MRILibrary Example #

In this repository you can try an example for the Volume extractor in our MRILibrary

-----
## Making the example work ##
To compile and install the library you need some depencies listed below.

### Dependencies ###
- Opencv 2
- C++ compiler for c++11
- Cmake 3.5 or higher

## Installing the library ##
The first thing we have to do is to clone the repository.

```console
foo@bar:~$ git clone https://github.com/UniversidadExtremadura/Example-Volume-Extractor.git
```

After that, we have to navigate to the MRILibrary code, compile it, and install it.

```console
foo@bar:~$ cd MRILibrary/
foo@bar:~/MRILibrary$ mkdir build && cd build/
foo@bar:~/MRILibrary/build/$ cmake ..
foo@bar:~/MRILibrary/build/$ sudo make install
```
If everything goes right you have installed the library.

## Executing the example ##
Now we have to go to the example project, compile it and execute it.


```console
foo@bar:~$ cd example/
foo@bar:~/example$ mkdir build && cd build/
foo@bar:~/example/build/$ cmake ..
foo@bar:~/example/build/$ cd ..
foo@bar:~/example/$ ./build/TESTS
```

