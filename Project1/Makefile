CXX = g++
CXXFLAGS = -g -Wall -MMD
OBJECTS = Noah_Morton.o
DEPENDS = ${OBJECTS:.o=.d}    # substitutes ".o" with ".d"
EXEC = project1

${EXEC} : ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	-rm *.o *.d project1

TAGS: project1
	unitags -eR *
