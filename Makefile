EXECUTABLE=demoinfo2

CPP_FILES=$(wildcard *.cpp)
OBJ_FILES=$(CPP_FILES:.cpp=.o)
PROTO_SRC_FILES=$(wildcard *.proto)
PROTO_CPP_FILES=$(addprefix generated_proto/,$(PROTO_SRC_FILES:.proto=.pb.cc))
PROTO_OBJ_FILES=$(PROTO_CPP_FILES:.cc=.o)

LD_FLAGS=-lsnappy -lprotobuf -L/usr/local/lib
CC_FLAGS=-I/usr/local/include
PROTOBUF_FLAGS=-I/usr/local/include

all: ${EXECUTABLE}

clean:
	rm -f ${EXECUTABLE}
	rm -f *.o
	rm -f generated_proto/*

generated_proto/%.pb.cc: %.proto
	protoc ${PROTO_SRC_FILES} ${PROTOBUF_FLAGS} -I. --cpp_out=generated_proto

${EXECUTABLE}: ${PROTO_OBJ_FILES} ${OBJ_FILES}
	g++ ${LD_FLAGS} -o $@ ${OBJ_FILES} ${PROTO_OBJ_FILES}

.cpp.o: ${CPP_FILES}
	g++ ${CC_FLAGS} -c -o $@ $<

.cc.o: ${PROTO_CPP_FILES}
	g++ ${CC_FLAGS} -c -o $@ $<

.PHONY: all clean
