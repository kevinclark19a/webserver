
VERSION		:=	0.0.1

OUT		:=	webserver
SRC		:=	src
INT		:=	obj

WORKING_DIR	:=	/web/.$(OUT)@$(VERSION)

RES		:=	res

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
			-D WEBSERVER__N_WORKER_THREADS='20'			\
			$(DEFINE)

DIRECTORIES	:=	$(WORKING_DIR)	\
			$(INT)		\
			$(INT)/$(SOK)	\
			$(INT)/$(HTP)	\
			$(INT)/$(SER)	\
			$(INT)/$(LOG)	\
			$(INT)/$(WQU)	\
			$(INT)/$(HAS)

OBJECTS		:=	$(INT)/webserver.o		\
			$(INT)/rest_methods.o		\
			$(INT)/$(SOK)/socket_api.o	\
			$(INT)/$(HTP)/http_interface.o	\
			$(INT)/$(HTP)/http_response.o	\
			$(INT)/$(SER)/server_api.o	\
			$(INT)/$(LOG)/log_api.o		\
			$(INT)/$(WQU)/workqueue_api.o	\
			$(INT)/$(WQU)/queue.o		\
			$(INT)/$(HAS)/hash_map.o


INSTALL_PATH	?=	/usr/local

all: .once .client $(OUT)

debug: DEFINE += -D ECHO_LOGS
debug: GLOB_FLAGS += -D DEBUG_MODE -D ECHO_LOGS
debug: all

clean:
	rm -rf $(DIRECTORIES) $(OUT)

install: all
	mkdir -p $(INSTALL_PATH)/bin
	cp ./$(OUT) $(INSTALL_PATH)/bin/$(OUT)

uninstall:
	rm -rf $(INSTALL_PATH)/bin/$(OUT)

.client: $(WORKING_DIR)
	cp -vr ./$(RES) $(WORKING_DIR)/

.once:
	[ -d /web ] || sudo mkdir /web
	sudo chown -R $(USER) /web
	touch .once

$(DIRECTORIES):
	mkdir $@

$(OUT): $(DIRECTORIES) $(OBJECTS)
	$(COMPILE) $(GLOB_FLAGS) $(LIBS) $(OBJECTS) -o $@

$(INT)/webserver.o: $(SRC)/webserver.c
	$(COMPILE) $(GLOB_FLAGS) -c $< -o $@

$(INT)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(COMPILE) $(GLOB_FLAGS) -c $< -o $@

