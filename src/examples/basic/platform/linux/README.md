# BUILDING
1. Create directory 'build'
```shell
foo@bar:basic/platform$ mkdir build
```
2. Goto 'build' directory
```shell
foo@bar:basic/platform$cd build
```
3. Invoke cmake command using CMakeList.txt file from directory above
```shell
foo@bar:basic/platform/build$ cmake ...
```
4. Build project
```shell
foo@bar:basic/platform/build$ cmake --build .
```
5. Run the application
```shell
foo@bar:basic/platform/build$ ./main
Timer tick: 0.001000 s
[100] ON|E
[100] ON|S
```

# Sending signals to application
1. Open another temirnal window and establish PID of the application process
```shell
foo@bar:basic/platform/build$ ps ax
...
  17777 ?        I      0:00 [kworker/3:2]
  17792 ?        I<     0:00 [kworker/u17:2]
  17804 pts/3    Rl+    0:05 ./main
  17806 pts/1    Ss     0:00 bash
  17813 pts/1    R+     0:00 ps ax
...  
```
2. Application accepts two Linux system signals: SIGUSR1 and SIGUSR2
```shell
foo@bar:basic/platform/build$ kill -SIGUSR2 17804
```
3. Go back to previous terminal. You should see:
```shell
Timer tick: 0.001000 s
[100] ON|E
[100] ON|S
[160969] ON|X
[160969] OFF|E
[160969] OFF|S
```