CC=g++
CFLAG=-O2
LIBOPT= -std=c++0x
SRC=DigitalWallet.cpp TransLog.cpp digitalWalletTestMain.cpp
EXE=run.x

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAG) -o $@ $(SRC) $(LIBOPT)

clean:
	@rm *.o
