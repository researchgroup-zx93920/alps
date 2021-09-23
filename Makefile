CPP=g++ -std=c++11 -DBOOST_LOG_DYN_LINK
SRC=./src

COIN_PATH=/home/mohitm3/cpp_libs/dist
LIB_COIN=-lCoinUtils

BOOST_INCLUDE_PATH=/home/mohitm3/cpp_libs/boost_1_62_0
BOOST_LIB_PATH=$(BOOST_INCLUDE_PATH)/stage
LIB_BOOST=-lboost_regex -lboost_program_options -lboost_log -lboost_log_setup -lboost_system -lboost_thread -lpthread -lboost_filesystem

alps: buildDirectory
	touch ./bin/alps
	$(CPP) $(SRC)/*.cpp -I$(COIN_PATH)/include -I$(BOOST_INCLUDE_PATH)/ -L$(COIN_PATH)/lib/ -L$(BOOST_LIB_PATH)/lib/ $(LIB_COIN) $(LIB_BOOST) -o ./bin/alps

buildDirectory:
	mkdir -p ./bin
