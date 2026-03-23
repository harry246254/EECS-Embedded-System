savedcmd_/home/user/driver/demo.mod := printf '%s\n'   demo.o | awk '!x[$$0]++ { print("/home/user/driver/"$$0) }' > /home/user/driver/demo.mod
