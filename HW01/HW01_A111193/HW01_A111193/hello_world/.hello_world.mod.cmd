savedcmd_/home/user/hello_world/hello_world.mod := printf '%s\n'   hello_world.o | awk '!x[$$0]++ { print("/home/user/hello_world/"$$0) }' > /home/user/hello_world/hello_world.mod
