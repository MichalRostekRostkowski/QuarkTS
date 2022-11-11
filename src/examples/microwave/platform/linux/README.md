# BUILDING
1. Create directory 'build'
```shell
foo@bar:microwave/platform/linux$ mkdir build
```
2. Goto 'build' directory
```shell
foo@bar:microwave/platform/linux$cd build
```
3. Invoke cmake command using CMakeList.txt file from directory above
```shell
foo@bar:microwave/platform/linux/build$ cmake ...
```
4. Build project
```shell
foo@bar:microwave/platform/linux/build$ cmake --build .
```
5. Run the application
```shell
foo@bar:microwave/platform/linux/build$ ./main
Timer tick: 0.001000 s
[100] DOOR_CLOSED|E
[100] DOOR_CLOSED|S
[100] OFF|E
[100] OFF|S
```

# Sending signals to application
1. Goto msg_sender directory
```shell
foo@bar:microwave/platform/linux/platform/$ cd msg_sender
```
2. Run sender application
```shell
foo@bar:microwave/platform/linux/platform/build$ python3 sender.py
```
3. Using command line send signals
```shell
Signal sender, press key to send a signal
  1 - SYS_SIG_OPEN
  2 - SYS_SIG_CLOSE
  3 - SYS_SIG_TOAST
  4 - SYS_SIG_BAKE
  q - quit
>
```