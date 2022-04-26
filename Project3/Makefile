CXX = g++
CXXFLAGS = -g -Wall -MMD
OBJECTS = Noah_Morton.o
DEPENDS = ${OBJECTS:.o=.d}    # substitutes ".o" with ".d"
EXEC = project3

${EXEC} : ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	-rm *.o *.d project3
	-ipcs -q | awk '{ print "ipcrm -q "$2}' | sh > /dev/null 2>&1;
	-ipcs -m | awk '{ print "ipcrm -m "$2}' | sh > /dev/null 2>&1;
	-ipcs -s | awk '{ print "ipcrm -s "$2}' | sh > /dev/null 2>&1;


TAGS: project3
	unitags -eR *
