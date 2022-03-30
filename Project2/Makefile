CXX = g++
CXXFLAGS = -g -Wall -MMD
OBJECTS = Noah_Morton.o
DEPENDS = ${OBJECTS:.o=.d}    # substitutes ".o" with ".d"
EXEC = project2

${EXEC} : ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

# Remove files created by compilation, also close processes/semaphores
# left open
clean:
	-rm *.o *.d project2
	ipcs -q | awk '{ print "ipcrm -q "$2}' | sh > /dev/null 2>&1;
	ipcs -m | awk '{ print "ipcrm -m "$2}' | sh > /dev/null 2>&1;
	ipcs -s | awk '{ print "ipcrm -s "$2}' | sh > /dev/null 2>&1;

TAGS: project2
	unitags -eR *
