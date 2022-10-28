#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=arm-linux-gnueabihf-gcc
CCC=arm-linux-gnueabihf-g++
CXX=arm-linux-gnueabihf-g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=VCAISH_usv-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/AllPthreadCreate.o \
	${OBJECTDIR}/AutoPilotData.o \
	${OBJECTDIR}/AutoPilotThread.o \
	${OBJECTDIR}/BoatData.o \
	${OBJECTDIR}/CanbusDataHandleThread.o \
	${OBJECTDIR}/ReadCan2Thread.o \
	${OBJECTDIR}/SamplingMonitorMode.o \
	${OBJECTDIR}/SerialDataHandleThread.o \
	${OBJECTDIR}/SerialPort.o \
	${OBJECTDIR}/SerialPort2.o \
	${OBJECTDIR}/SerialPort4.o \
	${OBJECTDIR}/TakingSampleByRemoteThread.o \
	${OBJECTDIR}/UltrasonicReadThread.o \
	${OBJECTDIR}/WashBottleThread.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/usv

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/usv: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/usv ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/AllPthreadCreate.o: AllPthreadCreate.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AllPthreadCreate.o AllPthreadCreate.c

${OBJECTDIR}/AutoPilotData.o: AutoPilotData.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AutoPilotData.o AutoPilotData.c

${OBJECTDIR}/AutoPilotThread.o: AutoPilotThread.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AutoPilotThread.o AutoPilotThread.c

${OBJECTDIR}/BoatData.o: BoatData.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BoatData.o BoatData.c

${OBJECTDIR}/CanbusDataHandleThread.o: CanbusDataHandleThread.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CanbusDataHandleThread.o CanbusDataHandleThread.c

${OBJECTDIR}/ReadCan2Thread.o: ReadCan2Thread.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ReadCan2Thread.o ReadCan2Thread.c

${OBJECTDIR}/SamplingMonitorMode.o: SamplingMonitorMode.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SamplingMonitorMode.o SamplingMonitorMode.c

${OBJECTDIR}/SerialDataHandleThread.o: SerialDataHandleThread.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SerialDataHandleThread.o SerialDataHandleThread.c

${OBJECTDIR}/SerialPort.o: SerialPort.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SerialPort.o SerialPort.c

${OBJECTDIR}/SerialPort2.o: SerialPort2.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SerialPort2.o SerialPort2.c

${OBJECTDIR}/SerialPort4.o: SerialPort4.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SerialPort4.o SerialPort4.c

${OBJECTDIR}/TakingSampleByRemoteThread.o: TakingSampleByRemoteThread.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TakingSampleByRemoteThread.o TakingSampleByRemoteThread.c

${OBJECTDIR}/UltrasonicReadThread.o: UltrasonicReadThread.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/UltrasonicReadThread.o UltrasonicReadThread.c

${OBJECTDIR}/WashBottleThread.o: WashBottleThread.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/WashBottleThread.o WashBottleThread.c

${OBJECTDIR}/main.o: main.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
