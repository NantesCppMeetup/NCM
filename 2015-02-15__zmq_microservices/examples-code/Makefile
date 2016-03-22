EXAMPLES =			\
	01-hw-server		\
	02-hw-client		\
	03-pub-sub-server	\
	04-pub-sub-client	\
	05-A-task-source	\
	05-B-task-work		\
	05-C-task-sink		\
	06-A-rb-reply		\
	06-B-rb-worker		\
	06-C-rb-proxy-A		\
	06-C-rb-proxy-B		\

all: $(HELPERS) $(EXAMPLES)

ZMQOPTS = $(shell pkg-config libzmq --cflags --libs) -std=c++14

% : %.cc
	g++ $< -o $@ $(ZMQOPTS)

clean:
	rm -f $(EXAMPLES)
