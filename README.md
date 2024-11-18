# BattelShip

#run login.c file
gcc login.c -o login \
$(pkg-config --cflags --libs sdl2) \
$(pkg-config --cflags --libs sdl2_ttf) \
-I/opt/homebrew/opt/mysql-client/include \
-L/opt/homebrew/opt/mysql-client/lib \
-L/opt/homebrew/opt/openssl@3/lib \
-lmysqlclient -lssl -lcrypto

./login

================================================
gcc temp.c -o temp \
$(pkg-config --cflags --libs sdl2) \
$(pkg-config --cflags --libs sdl2_ttf) \
-I/opt/homebrew/opt/mysql-client/include \
-L/opt/homebrew/opt/mysql-client/lib \
-L/opt/homebrew/opt/openssl@3/lib \
-lmysqlclient -lssl -lcrypto

./temp