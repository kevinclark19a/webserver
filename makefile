
VERSION		:=	0.0.1

WORKING_DIR	:=	/web/.webserver

OUT		:=	webserver
SRC		:=	src
INT		:=	obj

SOK		:=	socket
HTP		:=	http
SER		:=	server
LOG		:=	logging
WQU		:=	workqueue
HAS		:=	hashmap

COMPILE		:=	gcc -std=c11 -Wall -Werror -I$(SRC)

LIBS		:=	-pthread

GLOB_FLAGS	:=	-D WEBSERVER__VERSION='"$(VERSION)"'			\
			-D WEBSERVER__PROGRAM_NAME='"$(OUT)"'			\
			-D WEBSERVER__WORKING_DIRECTORY='"$(WORKING_DIR)"'	\
			-D WEBSERVER__N_WORKER_THREADS='20'

DIRECTORIES	:=	$(WORKING_DIR)	\
			$(INT)		\
			$(INT)/$(SOK)	\
			$(INT)/$(HTP)	\
			$(INT)/$(SER)	\
			$(INT)/$(LOG)	\
			$(INT)/$(WQU)	\
			$(INT)/$(HAS)

OBJECTS		:=	$(INT)/webserver.o		\
			$(INT)/$(SOK)/socket_api.o	\
			$(INT)/$(HTP)/http_interface.o	\
			$(INT)/$(HTP)/rest_api.o	\
			$(INT)/$(HTP)/rest_methods.o	\
			$(INT)/$(SER)/server_api.o	\
			$(INT)/$(LOG)/log_api.o		\
			$(INT)/$(WQU)/workqueue_api.o	\
			$(INT)/$(WQU)/queue.o		\
			$(INT)/$(HAS)/hash_map.o


all: .before $(OUT)

debug: GLOB_FLAGS += -D DEBUG_MODE -D ECHO_LOGS
debug: all

clean:
	rm -rf $(DIRECTORIES) $(OUT)

.before:
	[ -d /web ] || sudo mkdir /web
	sudo chown -R $(USER) /web

$(DIRECTORIES):
	mkdir $@

$(OUT): $(DIRECTORIES) $(OBJECTS)
	$(COMPILE) $(GLOB_FLAGS) $(LIBS) $(OBJECTS) -o $@

$(INT)/webserver.o: $(SRC)/webserver.c
	$(COMPILE) $(GLOB_FLAGS) -c $< -o $@

$(INT)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(COMPILE) $(GLOB_FLAGS) -c $< -o $@

