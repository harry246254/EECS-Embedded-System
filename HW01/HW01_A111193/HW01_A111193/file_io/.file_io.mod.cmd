savedcmd_/home/user/file_io/file_io.mod := printf '%s\n'   file_io.o | awk '!x[$$0]++ { print("/home/user/file_io/"$$0) }' > /home/user/file_io/file_io.mod
