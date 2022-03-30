CXX = g++
CXXFLAGS = -g -Wall -MMD
OBJECTS = Noah_Morton.o
DEPENDS = ${OBJECTS:.o=.d}    # substitutes ".o" with ".d"
EXEC = project2

${EXEC} : ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	-rm *.o *.d project2

TAGS: project2
	unitags -eR *
