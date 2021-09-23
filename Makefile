CPP=g++ -std=c++11 -DBOOST_LOG_DYN_LINK
SRC=./src

COIN_PATH=/opt/dist
LIB_COIN=-lCoinUtils

BOOST_PATH=/usr/local
LIB_BOOST=-lboost_program_options -lboost_log -lboost_log_setup -lboost_system -lboost_thread -lpthread -lboost_filesystem

alps: buildDirectory
	touch ./bin/alps
	$(CPP) $(SRC)/*.cpp -I$(COIN_PATH)/include -I$(BOOST_PATH)/include/ -L$(COIN_PATH)/lib/ -L$(BOOST_PATH)/lib/ $(LIB_COIN) $(LIB_BOOST) -o ./bin/alps

buildDirectory:
	mkdir -p ./bin
